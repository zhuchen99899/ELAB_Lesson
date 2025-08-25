/**
 * @file state_machine.c
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "state_machine.h"
#include "../../elab/common/elab_assert.h"

ELAB_TAG("StateMachine");
// 预留事件数组，用于定义状态机的基本事件类型。
// 每个事件由信号类型和附加数据组成，初始值为0。
sm_event_t sm_reserved_event[] =
{
	{ SM_EMPTY_SIG, 0 },// 空事件信号
	{ SM_ENTRY_SIG, 0 },// 状态进入信号
	{ SM_EXIT_SIG,  0 },// 状态退出信号
	{ SM_INIT_SIG,  0 },// 状态初始化信号
	{ SM_USER_SIG,  0 },// 用户自定义信号
};


/** 
 * @brief 状态机构造函数，用于初始化状态机对象。
 * 
 * @param me 指向状态机对象的指针。
 * @param init 初始状态处理函数。
 */
void fsm_ctor(sm_t *me, sm_state_handler_t init)
{
	elab_assert(0 != me);
	elab_assert(0 != init);

	me->state = 0;     // 当前状态设为未定义（等待初始化）
	me->temp  = init; // 设置初始状态处理函数（等待 fsm_init 触发）
}

/**
 * @brief 状态机初始化函数，用于执行状态机的初始状态
 * 
 * @param me 指向状态机对象的指针。
 * @param e 事件对象指针
 * @return sm_ret_t 
 * @note fsm_init() 正式进入该状态，确保触发一次 SM_ENTRY_SIG，符合 UML 状态机规范。
 */
sm_ret_t  fsm_init(sm_t *me, sm_event_t *e)
{
	sm_ret_t ret;

	elab_assert(0 != me); 
	elab_assert(0 != me->temp);
// 调用临时状态处理函数，传入初始化事件（通常e为空或为SM_INIT_SIG）
	ret = (me->temp)(me, e);

	if(ret != SM_RET_TRAN) // 如果状态没有转换，不需要继续初始化
	{
	return ret;
		
	}
	// 如果初始状态处理函数触发状态转换，则将当前状态设置为初始状态。
	SM_ENTRY(me, me->temp); 

	me->state = me->temp;// 正式更新当前状态
	return ret; // 返回状态初始化结果
}

/**
 * @brief 状态机事件分发函数，用于处理事件并执行状态转换。
 * 
 * @param me 状态机
 * @param e  事件
 * @note 跳转时自动执行退出→进入→状态更新流程。
 * 所有状态跳转都通过 SM_TRAN(me, target) 设置 me->temp，并返回 SM_RET_TRAN。
 * 若返回 SM_RET_HANDLED，表示已处理但不跳转。
 * 若返回 SM_RET_UNHANDLED，在 FSM 中会被忽略（不处理超状态，HSM才会用到）。
 */
void fsm_dispatch(sm_t *me, sm_event_t *e)
{
	sm_ret_t ret;

	elab_assert(me->state == me->temp); // 保证状态一致（无非法跳转）


	ret = (me->temp)(me, e);// 调用当前状态处理函数处理事件。


	if(SM_RET_TRAN == ret)// 如果状态处理函数返回状态转换信号
	{
		SM_EXIT(me, me->state);// 退出当前状态（发送SM_EXIT_SIG）
		SM_ENTRY(me, me->temp); // 进入目标状态（发送SM_ENTRY_SIG）
		me->state = me->temp;// 更新状态机的当前状态为目标状态
	}
}

/**
 * @brief hsm根状态
 * 
 * @param me  状态机对象
 * @param e 事件 
 * @return sm_ret_t 
 * @note  * 这是所有状态的根状态（最高层），当状态机处于顶层时调用。
 * 该函数一般不处理任何事件，直接返回忽略信号。
 */
sm_ret_t hsm_top(sm_t *me, const sm_event_t *e)
{
	(void)me;
	(void)e;
	return SM_IGNORE();
}

/**
 * @brief hsm 构造函数
 * 
 * @param me 事件状态机
 * @param init 初始状态
 */
void hsm_ctor(sm_t *me, sm_state_handler_t init)
{
    elab_assert(0 != me);
    elab_assert(0 != init);

    me->state = hsm_top;  // 初始化状态为顶层状态
    me->temp  = init;     // 初始化临时状态为传入的初始状态
	
}

/**
 * @brief 层次状态机初始化
 * 
 * @param me 事件状态机
 * @param e 事件
 */
void hsm_init(sm_t *me, sm_event_t *e)
{
	sm_ret_t ret;
	int8_t ip;

	sm_state_handler_t path[SM_MAX_NEST_DEPTH];// 用于存储状态路径
	sm_state_handler_t t = me->state;// 当前状态，初始为顶层

	elab_assert(0 != me);
	elab_assert(0 != me->temp);
	elab_assert(hsm_top == t);// 确保当前状态为顶层状态
	ret = (me->temp)(me, e);// 调用初始状态处理函数
	elab_assert(SM_RET_TRAN == ret); //初始条件必须触发状态转换

	do
	{
		ip = 0;// 初始化路径索引
		path[0] = me->temp;// 将目标状态存入路径数组
		
		SM_TRIG(me, me->temp,SM_EMPTY_SIG);// 发送空事件信号，检查父状态

		 // 持续触发状态函数，直到找到最顶层的父状态
		while( t != me->temp ) 
		{
			path[++ip] = me->temp;// 记录路径上的状态
			SM_TRIG(me, me->temp,SM_EMPTY_SIG);// 触发当前临时状态的处理函数，发送空信号 SM_EMPTY_SIG
			t = me->temp;  // 更新 t 的值为当前临时状态
		}
		 // 将路径数组的第一个状态赋值给临时状态
		me->temp = path[0]; // 重置临时状态为目标状态 即第一个初始状态
		
		elab_assert(ip < SM_MAX_NEST_DEPTH);// 确保路径索引未超过最大深度限制
		do
		{
			SM_ENTRY(me, path[ip--]); // 按从顶到底的顺序调用入口动作

		}while(ip >= 0);
		t = path[0]; 

	}while(SM_RET_TRAN == SM_TRIG(me, t, SM_INIT_SIG));// 递归初始化子状态

	me->temp = t;//还原
	me->state = me->temp; //当前进行过初始化，当前状态进入到初始化状态

}

/**
 * @brief 查找两个状态之间的路径，确定状态转换时需要退出和进入的状态序列。
 * 
 * @param me 指向状态机对象的指针
 * @param t  目标状态处理函数（即状态转换的目标状态）
 * @param s  源状态处理函数（即当前所在的状态）
 * @param path 用于存储从源状态到目标状态之间路径的数组
 * @return uint8_t 返回路径中需要进入的状态的数量（路径深度-1），即进入动作的执行顺序索引
 * 
 * @note 
 * 这个函数是层次状态机（HSM）核心，用于找到从当前状态s到目标状态t的最近公共祖先（LCA，Lowest Common Ancestor），
 * 并构造状态转换过程中需要依次退出的状态和进入的状态路径。
 * 
 * 它根据以下规则查找：
 * 1. 如果源状态等于目标状态，直接退出源状态；
 * 2. 通过触发SM_EMPTY_SIG事件，查询状态的上层父状态（超状态）；
 * 3. 通过比较父状态判断最近公共祖先，并记录路径中需要退出和进入的状态；
 * 4. 最后返回路径深度，方便在调用函数中正确调用退出和进入动作。
 * 
 * 具体流程如下：
 * - 先判断源状态和目标状态是否相同（self transition），直接退出源状态；
 * - 查找目标状态的所有父状态，找到与源状态父状态的交集；
 * - 如果找不到最近公共祖先，则分别从源状态和目标状态逐级退出和进入，确保状态转换的正确执行；
 * - path数组会记录进入路径，供调用者执行SM_ENTRY动作；
 */
static uint8_t hsm_find_path(sm_t *me,
						sm_state_handler_t t,
						sm_state_handler_t s,
						sm_state_handler_t path[SM_MAX_NEST_DEPTH])
{
	signed char ip = -1;  // path索引，用于记录进入路径状态数
	signed char iq;
	sm_ret_t ret;

	/* (a) 如果源状态和目标状态相同，属于自转，直接退出当前状态 */
	if( s == t)
	{
		SM_EXIT(me, s);  // 发送退出信号给源状态
		ip = 0;          // 进入路径深度0，表示无进入动作

		goto hsm_find_path_end;
	}

	// 查询目标状态的父状态（通过发送空事件SM_EMPTY_SIG）
	SM_TRIG(me, t, SM_EMPTY_SIG);
	t = me->temp; // t更新为父状态

	/* (b) 如果源状态等于目标状态的父状态，说明目标是源的子状态 */
	if( s == t )
	{
		ip = 0;
		goto hsm_find_path_end;
	}

	// 查询源状态的父状态
	SM_TRIG(me, s, SM_EMPTY_SIG);

	/* (c) 如果源状态和目标状态的父状态相同，说明两者是兄弟状态 */
	if(me->temp == t)
	{
		SM_EXIT(me, s);
		ip = 0;
		goto hsm_find_path_end;
	}

	/* (d) 如果源状态的父状态等于path[0]，继续退出 */
	if( me->temp == path[0] )
	{
		SM_EXIT(me, s);
		goto hsm_find_path_end;
	}

	/* (e) 否则，沿着目标状态的父状态路径向上搜索，构造进入路径 */
	ip = 1;
	iq = 0;
	path[1] = t;         // path[1]是目标状态的父状态
	t = me->temp;        // t更新为父状态的父状态

	// 递归沿父状态链，直到找到源状态s或到顶层
	ret = SM_TRIG(me, path[1], SM_EMPTY_SIG);
	while(SM_RET_SUPER == ret)
	{
		path[++ip] = me->temp;  // 记录路径状态
		if(s == me->temp)       // 找到源状态作为父状态，LCA找到
		{
			iq = 1;
			elab_assert(ip < SM_MAX_NEST_DEPTH);
			ip--;  // 调整路径深度

			ret = SM_RET_HANDLED; // 标记已处理
		}
		else
		{
			ret = SM_TRIG(me, me->temp, SM_EMPTY_SIG); // 继续沿父状态链向上搜索
		}
	}

	// 如果没有找到LCA，则尝试退出源状态的父状态直到匹配目标状态的父状态路径
	if(0 == iq)
	{
		elab_assert(ip < SM_MAX_NEST_DEPTH);

		SM_EXIT(me, s);  // 退出源状态

		iq = ip;
		ret = SM_RET_IGNORE; /* LCA未找到 */
		do
		{
			s = path[iq];
			/* 判断是否为LCA */
			if(t == s)
			{
				ret = SM_RET_HANDLED;

				ip = iq - 1;
				iq = -1;
			}
			else
			{
				iq--; /* 尝试更低层父状态 */
			}
		}while(iq >= 0);

		/* 如果仍未找到LCA */
		if( SM_RET_HANDLED != ret )
		{
			/* 尝试遍历源状态的所有父状态和目标状态的父状态 */
			ret = SM_RET_IGNORE;
			do
			{
				if(SM_RET_HANDLED  == SM_EXIT(me, t))
				{
					SM_TRIG(me, t, SM_EMPTY_SIG);
				}
				t = me->temp;
				iq = ip;
				do
				{
					s = path[iq];
					if( t == s)
					{
						ip = iq -1;
						iq = -1;

						ret = SM_RET_HANDLED; /* 找到LCA，跳出 */
					}
					else
					{
						iq--;
					}
				}while(iq >= 0);
			}while(SM_RET_HANDLED != ret);
		}
	}

hsm_find_path_end:
	return ip;  // 返回进入路径深度（索引）
}

/**
 * @brief 层次状态机事件分发函数，处理事件并执行状态转换（包括父状态的处理）。
 * 
 * @param me 指向状态机对象
 * @param e 事件指针
 * 
 * @note 
 * - 该函数实现了层次状态机的事件传递机制：事件从当前状态向上递归传递到父状态，直到被处理；
 * - 处理状态转换时，会正确执行退出动作和进入动作，支持多层嵌套状态转换；
 * - 通过调用 hsm_find_path，计算退出和进入的状态路径，保证状态转换的正确执行顺序；
 * - 支持初始化子状态（处理SM_INIT_SIG信号），实现状态层次嵌套的初始化。
 * 当你在状态运行过程中通过 SM_TRAN() 触发状态跳转，比如从 A1a 跳到 B1b，新的目标状态可能也有初始子状态。
 * hsm_init 仅会初始化 根节点下子状态，如root->A->A1->A1a
 * 而跳转到的B 可能没有初始化过子状态，这时dispatch会处理
 * 
 * 就像操作系统进程有“主线程”和“子线程”，你启动应用（hsm_init）会启动整个进程树，
 * 但当你从界面跳转（hsm_dispatch → 状态跳转）时，
 * 每个子界面也有自己的默认子界面，这些也要初始化。
 */
void hsm_dispatch(sm_t *me, sm_event_t *e)
{
	sm_state_handler_t t = me->state;  // 当前状态
	sm_state_handler_t s;

	sm_ret_t ret;

	// 保证当前状态稳定（temp和state一致）
	elab_assert(me->state == me->temp);

	// 事件递归传递，直到有状态处理事件或者到顶层被忽略
	do
	{
		s = me->temp;
		ret = s(me, e); 	// 当前状态处理事件
		if(SM_RET_UNHANDLED == ret)
		{
			ret = SM_TRIG(me, s, SM_EMPTY_SIG); // 事件未处理，发送空事件给父状态尝试处理
		}
	}while(SM_RET_SUPER == ret);  // 若返回SM_RET_SUPER，继续向上层状态传递事件

	// 如果发生状态转换
	if(SM_RET_TRAN == ret)
	{
		sm_state_handler_t path[SM_MAX_NEST_DEPTH];
		signed char ip = -1;

		path[0] = me->temp; 	// 转换的目标状态
		path[1] = t; 			// 源状态

		// 退出当前状态到转移源状态
		for( ; s != t; t = me->temp)
		{	
			ret = SM_EXIT(me, t);
			if(SM_RET_HANDLED == ret)
			{
				SM_TRIG(me, t, SM_EMPTY_SIG); // 退出状态后尝试进入父状态处理
			}
		}

		// 找出退出和进入的路径，ip为路径深度
		ip = hsm_find_path(me, path[0], s, path);

		// 按路径从父到子顺序进入新状态
		for(; ip>=0; ip--)
		{
			SM_ENTRY(me, path[ip]);
		}

		t = path[0];
		me->temp = t;

		// 递归初始化子状态，处理SM_INIT_SIG信号
		while( SM_RET_TRAN == SM_TRIG(me, t, SM_INIT_SIG) )
		{
			ip = 0;
			path[0] = me->temp;

			SM_TRIG(me, me->temp, SM_EMPTY_SIG);
			while(t != me->temp)
			{	
				path[++ip] = me->temp;
				SM_TRIG(me, me->temp, SM_EMPTY_SIG);
			}
			me->temp = path[0];

			elab_assert(ip < SM_MAX_NEST_DEPTH);

			do
			{
				SM_ENTRY(me, path[ip--]);
			}while(ip >= 0);

			t = path[0];
		} // end while

	} // end if

	me->temp = t;  // 更新临时状态为当前状态
	me->state = t; // 更新当前状态
}

