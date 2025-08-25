/**
 * @file state_machine.h
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H


#include "../../elab/common/elab_def.h"
#ifdef __cplusplus
extern "C" {
#endif





//层次状态机最大嵌套深度
#ifndef SM_MAX_NEST_DEPTH
#define SM_MAX_NEST_DEPTH 		6
#endif

typedef int32_t sm_sig_t;

/**
 * @brief 状态机事件
 * 
 */
typedef struct _event_s{

    sm_sig_t sig; // 事件信号（标识事件类型）
    void *event;// 事件附加参数（可传指针/结构体）
}sm_event_t;


/**
 * @bref 状态处理函数返回值, 指示事件被怎么处理了
 */
typedef uint8_t sm_ret_t;


/**状态机返回值 */
enum
{
	SM_RET_HANDLED=0,
	SM_RET_IGNORE,
	SM_RET_UNHANDLED,

	SM_RET_TRAN,
	SM_RET_SUPER,
};



typedef struct sm_s sm_t;
//状态处理函数类型
typedef sm_ret_t (*sm_state_handler_t)(sm_t *fsm,sm_event_t const *e);

/**
 * @brief 状态机
 * 
 */
struct sm_s{
    sm_state_handler_t state;// 当前状态处理函数
    sm_state_handler_t temp;// 用于跳转前暂存目标状态（状态变更过程用）
};


#define SM_RET_CAST(x) 	( (sm_ret_t)(x) )

#define SM_HANDLED() 		SM_RET_CAST(SM_RET_HANDLED)
#define SM_IGNORE() 		SM_RET_CAST(SM_RET_IGNORE)
#define SM_UNHANDLED() 	SM_RET_CAST(SM_RET_UNHANDLED)

#define SM_TRAN(me, target) \
			((me)->temp = (target), SM_RET_CAST(SM_RET_TRAN))
#define SM_SUPER(me, super) \
			((me)->temp = (super), SM_RET_CAST(SM_RET_SUPER))

enum sm_reserved_sig
{
	SM_EMPTY_SIG = -5,
	SM_ENTRY_SIG = -4,
	SM_EXIT_SIG  = -3,
	SM_INIT_SIG  = -2,
	SM_USER_SIG  = -1,
};

//预留事件
extern sm_event_t sm_reserved_event[];

//状态机触发 进入 退出 宏
#define SM_TRIG(me, state, sig) 	((state)(me, &sm_reserved_event[5 + (sig)]))
#define SM_ENTRY(me, state) 		SM_TRIG(me, state, SM_ENTRY_SIG)
#define SM_EXIT(me, state) 		SM_TRIG(me, state, SM_EXIT_SIG)




void fsm_ctor(sm_t *me, sm_state_handler_t init);
sm_ret_t  fsm_init(sm_t *me, sm_event_t *e); 
void fsm_dispatch(sm_t *me, sm_event_t *e);


void hsm_ctor(sm_t *me, sm_state_handler_t init);
sm_ret_t hsm_top(sm_t *me, const sm_event_t *e);
void hsm_init(sm_t *me, sm_event_t *e);
void hsm_dispatch(sm_t *me, sm_event_t *e);



#ifdef __cplusplus
}
#endif

#endif // __STATE_MACHINE_H

