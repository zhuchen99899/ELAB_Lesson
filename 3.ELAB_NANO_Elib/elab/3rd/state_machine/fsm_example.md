> [!CAUTION]
>
> 注意初始状态第一次信号必须要触发 SM_TRAN()，否则会触发dispatch函数中的宏`assert(*me*->state == *me*->temp)`
>
> 如果初始状态第一次触发不能触发SM_TRAN()，则不能作为初始状态，建议新建一个init状态。
>
> 在下面红绿灯的案例中因为第一次就会触发SM_TRAN，所以可以将red_light作为初始状态。
>
> 在制冷加热器中，由于`idle`开始的状态不能触发SM_TRAN(初始化时，信号中的event温度为25，不能触发转换)。则不能使用idle作为初始状态进行初始化，需要新建一个状态。

# FSM 有限状态机demo1红绿灯：


红绿灯案例

```c
#include <stdio.h>
#include "state_machine.h"

// 定义状态处理函数
sm_ret_t red_light(sm_t *me, const sm_event_t *e);
sm_ret_t green_light(sm_t *me, const sm_event_t *e);
sm_ret_t yellow_light(sm_t *me, const sm_event_t *e);

// 状态机对象
sm_t traffic_light;
#define EVENT_TIMER_EXPIRED 1
// 红灯状态处理函数
sm_ret_t red_light(sm_t *me, const sm_event_t *e)
{
    switch (e->sig) {
        case EVENT_TIMER_EXPIRED:
            printf("红灯 -> 绿灯\n");
            return SM_TRAN(me, green_light);  // 使用宏触发状态转换
        case SM_ENTRY_SIG:
            printf("进入红灯状态\n");
            return SM_HANDLED();  // 使用宏标记事件已处理
        case SM_EXIT_SIG:
            printf("退出红灯状态\n");
            return SM_HANDLED();  // 使用宏标记事件已处理
        default:
            return SM_IGNORE();   // 使用宏标记事件忽略
    }
}

// 绿灯状态处理函数
sm_ret_t green_light(sm_t *me, const sm_event_t *e)
{
    switch (e->sig) {
        case EVENT_TIMER_EXPIRED:
            printf("绿灯 -> 黄灯\n");
            return SM_TRAN(me, yellow_light);  // 使用宏触发状态转换
        case SM_ENTRY_SIG:
            printf("进入绿灯状态\n");
            return SM_HANDLED();  // 使用宏标记事件已处理
        case SM_EXIT_SIG:
            printf("退出绿灯状态\n");
            return SM_HANDLED();  // 使用宏标记事件已处理
        default:
            return SM_IGNORE();   // 使用宏标记事件忽略
    }
}

// 黄灯状态处理函数
sm_ret_t yellow_light(sm_t *me, const sm_event_t *e)
{
    switch (e->sig) {
        case EVENT_TIMER_EXPIRED:
            printf("黄灯 -> 红灯\n");
            return SM_TRAN(me, red_light);  // 使用宏触发状态转换
        case SM_ENTRY_SIG:
            printf("进入黄灯状态\n");
            return SM_HANDLED();  // 使用宏标记事件已处理
        case SM_EXIT_SIG:
            printf("退出黄灯状态\n");
            return SM_HANDLED();  // 使用宏标记事件已处理
        default:
            return SM_IGNORE();   // 使用宏标记事件忽略
    }
}

// 主函数
int main()
{
    // 初始化状态机，设置初始状态为红灯
    fsm_ctor(&traffic_light, red_light);

    // 创建事件对象
    sm_event_t timer_event = { EVENT_TIMER_EXPIRED, 0,0 };

    // 初始化状态机
    fsm_init(&traffic_light, &timer_event);

    // 模拟状态机事件分发
    for (int i = 0; i < 6; i++) {
        fsm_dispatch(&traffic_light, &timer_event); // 分发定时器事件
    }

    return 0;
}
```



> 红灯 -> 绿灯
> 进入绿灯状态
> 绿灯 -> 黄灯
> 退出绿灯状态
> 进入黄灯状态
> 黄灯 -> 红灯
> 退出黄灯状态
> 进入红灯状态
> 红灯 -> 绿灯
> 退出红灯状态
> 进入绿灯状态
> 绿灯 -> 黄灯
> 退出绿灯状态
> 进入黄灯状态
> 黄灯 -> 红灯
> 退出黄灯状态
> 进入红灯状态
> 红灯 -> 绿灯
> 退出红灯状态
> 进入绿灯状态

# FSM有限状态机DEMO2, 附带event参数，制冷加热器:

加热冷却器案例。

```c
#define TEMPERATURE_CHANGED 1

// 状态处理函数声明
sm_ret_t cooling(sm_t *me, const sm_event_t *e);
sm_ret_t heating(sm_t *me, const sm_event_t *e);
sm_ret_t idle(sm_t *me, const sm_event_t *e);

// 状态机对象
static sm_t temperature_fsm;

// 冷却状态处理函数
sm_ret_t cooling(sm_t *me, const sm_event_t *e) {
    switch (e->sig) {
        case SM_ENTRY_SIG:
            elog_info("进入冷却状态\n");
            return SM_HANDLED();
        case SM_EXIT_SIG:
            elog_info("退出冷却状态\n");
            return SM_HANDLED();
        case TEMPERATURE_CHANGED: {
            int temp = *(int *)e->event; // 从事件中获取温度数据
            if (temp <= 25) {
                elog_info("冷却 -> 空闲，当前温度: %d\n", temp);
                return SM_TRAN(me, idle); // 切换到空闲状态
            }
            elog_info("冷却中，当前温度: %d\n", temp);
            return SM_HANDLED();
        }
        default:
            return SM_IGNORE();
    }
}

// 加热状态处理函数
sm_ret_t heating(sm_t *me, const sm_event_t *e) {
    switch (e->sig) {
        case SM_ENTRY_SIG:
            elog_info("进入加热状态\n");
            return SM_HANDLED();
        case SM_EXIT_SIG:
            elog_info("退出加热状态\n");
            return SM_HANDLED();
        case TEMPERATURE_CHANGED: {
            int temp = *(int *)e->event; // 从事件中获取温度数据
            if (temp >= 20) {
                elog_info("加热 -> 空闲，当前温度: %d\n", temp);
                return SM_TRAN(me, idle); // 切换到空闲状态
            }
            elog_info("加热中，当前温度: %d\n", temp);
            return SM_HANDLED();
        }
        default:
            return SM_IGNORE();
    }
}

// 空闲状态处理函数
sm_ret_t idle(sm_t *me, const sm_event_t *e) {
    switch (e->sig) {
        case SM_ENTRY_SIG:
            elog_info("进入空闲状态\n");
            return SM_HANDLED();
        case SM_EXIT_SIG:
            elog_info("退出空闲状态\n");
            return SM_HANDLED();
        case TEMPERATURE_CHANGED: {
            int temp = *(int *)e->event; // 从事件中获取温度数据
            if (temp > 30) {
                elog_info("空闲 -> 冷却，当前温度: %d\n", temp);
                return SM_TRAN(me, cooling); // 切换到冷却状态
            } else if (temp < 15) {
                elog_info("空闲 -> 加热，当前温度: %d\n", temp);
                return SM_TRAN(me, heating); // 切换到加热状态
            }
            elog_info("空闲中，当前温度: %d\n", temp);
            return SM_TRAN(me, idle); // 切换到空闲
        }
        default:
            return SM_IGNORE();
    }
}

sm_ret_t init(sm_t *me, const sm_event_t *e) {
    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        elog_info("进入初始状态\n");
        return SM_HANDLED();
    case SM_EXIT_SIG:
        elog_info("退出空闲状态\n");
        return SM_HANDLED();
    case TEMPERATURE_CHANGED: 
        return SM_TRAN(me, idle); // 切换到空闲
    break;
    default:
        return SM_IGNORE();
    }
    
}

void main()
{
    
        static int temperature = 25;
    sm_event_t temp_event = { TEMPERATURE_CHANGED, &temperature ,0}; // 定义温度事件

    // 初始化状态机，设置初始状态为空闲
    fsm_ctor(&temperature_fsm, init);

    // 初始化状态机
    fsm_init(&temperature_fsm, &temp_event);

    int temperatures[] = { 35, 28, 22, 18, 12, 20, 25 };
    for (int i = 0; i < 7; i++) {
        temperature = temperatures[i];
        elog_info("温度变化: %d\n", temperature);
        fsm_dispatch(&temperature_fsm, &temp_event);
    }
while(1){
    
    
}
    
}
```

> 输出：进入空闲状态
>
> 温度变化: 35
>
> 空闲 -> 冷却，当前温度: 35
>
> 退出空闲状态
>
> 进入冷却状态
>
> 温度变化: 28
>
> 冷却中，当前温度: 28
>
> 温度变化: 22
>
> 冷却 -> 空闲，当前温度: 22
>
> 退出冷却状态
>
> 进入空闲状态
>
> 温度变化: 18
>
> 空闲中，当前温度: 18
>
>  退出空闲状态
>
> 进入空闲状态
>
> 温度变化: 12
>
> 空闲 -> 加热，当前温度: 12
>
> 退出空闲状态
>
> 进入加热状态
>
> 温度变化: 20
>
> 加热 -> 空闲，当前温度: 20
>
> 退出加热状态
>
>  进入空闲状态
>
> 温度变化: 25
>
> 空闲中，当前温度: 25
>
> 退出空闲状态
>
> 进入空闲状态
