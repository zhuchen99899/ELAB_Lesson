# example1:  子状态机处理特殊问题，父状态机处理共性

```
             +---------+
             |   top   |---------------------------+
             +---------+                           |
                 |                                 |
                 |                                 |
                 |                                 |
                 v                                 v  
             +---------+     CHAR_SIG           +---------+
             |   s0    |----------------------->|   s1    |
             +---------+                        +---------+
                 |                           	   | 
                 v                                 v
             +---------+                        +---------+
             |   s2    |                        |   s3    |
             +---------+                        +---------+

```

## ✅ 测试代码

```c
sm_t hsm;
static sm_ret_t init(sm_t *me,sm_event_t const *e);
static sm_ret_t top(sm_t *me,sm_event_t const *e);

static sm_ret_t s0(sm_t *me,sm_event_t const *e); //s0
static sm_ret_t s1(sm_t *me,sm_event_t const *e);//s1
static sm_ret_t s2(sm_t *me,sm_event_t const *e);//s2
static sm_ret_t s3(sm_t *me,sm_event_t const *e);//s3

static sm_ret_t init(sm_t *me,sm_event_t const *e)
{
    elog_error("init");
    (void) me; (void) *e;
    return SM_TRAN(me, top);
}
static sm_ret_t top(sm_t *me,sm_event_t const *e)
{
switch (e->sig)
{
case SM_ENTRY_SIG:
    printf("top entry\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_EXIT_SIG:
    printf("top exit\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_INIT_SIG:
    printf("top init\n");
    return SM_TRAN(me, s0);
    /* code */
}
return SM_SUPER(me, hsm_top);
}

static sm_ret_t s0(sm_t *me,sm_event_t const *e)
{

switch (e->sig)
{
case SM_ENTRY_SIG:
    printf("s0 entry\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_EXIT_SIG:
    printf("s0 exit\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_INIT_SIG:
    printf("s0 init\n");
    return SM_TRAN(me, s2);
    /* code */
case CHAR_SIG:
    printf("s0 char\n");
    return SM_TRAN(me, s1);
    /* code */
}
return SM_SUPER(me, hsm_top);
}

static sm_ret_t s1(sm_t *me,sm_event_t const *e)
{

switch (e->sig)
{
case SM_ENTRY_SIG:
    printf("s1 entry\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_EXIT_SIG:
    printf("s1 exit\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_INIT_SIG:
    printf("s1 init\n");
    return SM_TRAN(me, s3);
    /* code */
case CHAR_SIG:
    printf("s1 char\n");
    return SM_TRAN(me, s0);
    /* code */
}
return SM_SUPER(me, hsm_top);
}


static sm_ret_t s2(sm_t *me,sm_event_t const *e)
{

switch (e->sig) {
    case SM_ENTRY_SIG:
    printf("s2 entry\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_EXIT_SIG:
    printf("s2 exit\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
}
return SM_SUPER(me, s0);

}

static sm_ret_t s3(sm_t *me,sm_event_t const*e)
{
switch (e->sig) {
    case SM_ENTRY_SIG:
    printf("s3 entry\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_EXIT_SIG:
    printf("s3 exit\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
}
return SM_SUPER(me, s1);
}


void main(void){
    hsm_ctor(&hsm, init);
    hsm_init(&hsm, NULL);
    elog_debug("hsm_init_finish");
    // // 模拟事件驱动
    sm_event_t event = { .sig = CHAR_SIG,0 };

    hsm_dispatch(&hsm, &event);
    while(1)
    {
        
        
    }
    
}
```

## ✅ 运行结果

> top entry
> top init
> s0 entry
> s0 init
> s2 entry
> [D/ringbuf_handle L:163,T:44] hsm_init_finish
> s0 char
> s2 exit
> s0 exit
> s1 entry
> s1 init
> s3 entry

## 注意点和理解

> [!CAUTION]
>
> - 和fsm一样，初始化时必须要有个启动函数 init，里面必须要做转换操作
>
> - 初始化规则1：`hsm_init` 会初始化所有附带 `SM_INIT_SIG`信号的状态机，`SM_INIT_SIG`信号必须`trans` 转移到其子状态机
>
> - 初始化规则2：hsm_init执行的初始化路径只有一条，以根状态 top 依次 查找init信号的路径(这个例子里为`top->S0->S2`，所以状态机初始化后会从`top`依次执行到`S2`(只执行`entry`)。此时状态机初始化完毕。在这个例子里状态机初始化后，停在了子状态机`S2`
>
> - 响应信号规则1：如果当前子状态机没有此信号将会传给其父状态机，如果父状态机没有则依次传递父状态机的父状态机，此传递的最大递归深度由`state_machine`.h的`SM_MAX_NEST_DEPTH`宏决定。
>
> - 响应信号规则2：子状态机和父状态机的定义由状态机中的SM_SUPER决定，SM_SUPER表示这个状态机的父状态机
>
>   所以在这个例子中S2没有响应`CHAR_SIG`,则将信号传递给`S1`，`S1`可以响应则响应改信号，发送状态变化。
>
> - `dispatch`规则:转换到另一条状态机时，会触发`dispatch`的初始化机制，直接进入另一个状态机(如果这个状态机有`SM_INIT_SIG`的传递)的底层的子状态机(在这个例子里，就是`S0`触发后 转到`S1` ，然后触发`init`，一直触发到`S3`，最终停在这条路径的最底层状态`S3`)



> 理解：把这个理解为可以继承的状态机，子状态若处理不了的信号，则父状态可以处理，当有大量相同的重复的事情时，不需要用fsm状态机来完成，会复杂很多倍。这时候要有抽象思想，将相同的事情抽象为父状态，独特的作为子状态。
>
> 通过init路径会把子状态机作为主要状态。子状态作为层次状态机的一个主要状态，每次切换状态机，初始化机制会强制从父亲状态一路切换到底层子状态(如果定义了`SM_INIT_SIG`)。

# example 2： 菜单页面 

这是层次状态机（HSM）**应用非常广泛且效果明显**的场景之一！
我们来构造一个典型的：**带配置菜单的嵌入式 UI 状态机（支持 ESC 键逐级返回）**。

------

## 🧠 场景需求描述（真实场景）

你在嵌入式系统（如工业仪表、智能家电）中，有以下界面结构：

```
arduino复制编辑Top
 ├── HomeScreen
 └── Menu
      ├── DisplaySettings
      │     ├── Brightness
      │     └── Contrast
      └── NetworkSettings
            ├── WiFi
            └── Ethernet
```

- 用户在 Home 按下 `Menu` 进入 `Menu`
- 用方向键进入 `DisplaySettings -> Brightness`
- 按下 `ESC` 键逐级返回上一级菜单，最终返回 `HomeScreen`

------

## ✅ HSM 的强大体现：

| 功能点                       | HSM 优势             |
| ---------------------------- | -------------------- |
| 每个子菜单共用 ESC 返回逻辑  | 放在父状态 `Menu` 中 |
| 子菜单又有独立逻辑（如滑条） | 放在具体状态         |
| 多级嵌套菜单                 | 自动维护父子关系     |

## ✅ 示例代码：

```c

/**
 *  ├── HomeScreen
    └── Menu
      ├── DisplaySettings
      │     ├── Brightness
      │     └── Contrast
      └── NetworkSettings
            ├── WiFi
            └── Ethernet
 * 
 */

sm_t hsm;
enum
{
    SIG_MENU=0, //切到菜单
    SIG_ENTER_DISPLAY, 
    SIG_ENTER_BRIGHTNESS,
    SIG_ENTER_WIFI,
    SIG_DOWN, //同级别 向下菜单切换
    SIG_UP,  //同级别 向上菜单切换
    SIG_ESC
};

static sm_ret_t init(sm_t *me, sm_event_t const *e);
static sm_ret_t begin_state(sm_t *me, sm_event_t const *e);
static sm_ret_t home_state(sm_t *me, sm_event_t const *e);
static sm_ret_t Menu_state(sm_t *me, sm_event_t const *e);
static sm_ret_t DisplaySettings_state(sm_t *me, sm_event_t const *e);
static sm_ret_t Brightness_state(sm_t *me, sm_event_t const *e);
static sm_ret_t Contrast_state(sm_t *me, sm_event_t const *e);
static sm_ret_t NetworkSettings_state(sm_t *me, sm_event_t const *e);
static sm_ret_t WIFI_state(sm_t *me, sm_event_t const *e);
static sm_ret_t Ethernet_state(sm_t *me, sm_event_t const *e);


static sm_ret_t init(sm_t *me, sm_event_t const *e)
{
    (void)me;(void)e;
    return SM_TRAN(me, begin_state);
}
static sm_ret_t begin_state(sm_t *me, sm_event_t const *e)
{
switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("begine entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("begine  exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_INIT_SIG:
        printf("begine init\r\n");
        return SM_TRAN(me, home_state);
    /* code */
    }
return SM_SUPER(me, hsm_top);

}

static sm_ret_t home_state(sm_t *me, sm_event_t const *e)
{
    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("home_State entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("home_State exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_MENU:
        printf("go Menu\r\n");
        return SM_TRAN(me, Menu_state);
    /* code */
    }
return SM_SUPER(me, begin_state);

}

//Menu 父状态，统一处理ESC返回
static sm_ret_t Menu_state(sm_t *me, sm_event_t const *e)
{
    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("Menu_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("Menu_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_ESC:
        printf("back home\r\n");
        return SM_TRAN(me,begin_state); // 统一 ESC 返回首页
    case SIG_ENTER_DISPLAY:
        printf("go DisplaySettings\r\n");
        return SM_TRAN(me,DisplaySettings_state);
    /* code */
    }
return SM_SUPER(me, home_state);


}


static sm_ret_t DisplaySettings_state(sm_t *me, sm_event_t const *e)
{
    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("DisplaySettings_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("DisplaySettings_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_DOWN:
        printf("go NetworkSettings\r\n");
        return SM_TRAN(me, NetworkSettings_state);
    case SIG_ENTER_BRIGHTNESS:
        printf("go Brightness\r\n");
        return SM_TRAN(me, Brightness_state);
    /* code */
    }
return SM_SUPER(me, Menu_state);


}

static sm_ret_t NetworkSettings_state(sm_t *me, sm_event_t const *e)
{


    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("NetworkSettings_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("NetworkSettings_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_UP:
        printf("go DisplaySettings\r\n");
        return SM_TRAN(me, DisplaySettings_state);
    case SIG_ENTER_WIFI:
        printf("go WIFI\r\n");
        return SM_TRAN(me, WIFI_state);
        
    /* code */
    }
return SM_SUPER(me, Menu_state);

}

static sm_ret_t Brightness_state(sm_t *me, sm_event_t const *e)
{


    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("Brightness_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("Brightness_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_DOWN:
        printf("go Contrast_state\r\n");
        return SM_TRAN(me, Contrast_state);
        
    /* code */
    }
return SM_SUPER(me, DisplaySettings_state);
}

static sm_ret_t Contrast_state(sm_t *me, sm_event_t const *e)
{

    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("Contrast_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("Contrast_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_UP:
        printf("go Brightness_state\r\n");
        return SM_TRAN(me, Brightness_state);
        
    /* code */
    }
return SM_SUPER(me, DisplaySettings_state);
}

static sm_ret_t WIFI_state(sm_t *me, sm_event_t const *e)
{

    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("WIFI_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("WIFI_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_DOWN:
        printf("go Ethernet_state\r\n");
        return SM_TRAN(me, Ethernet_state);
        
    /* code */
    }
return SM_SUPER(me, NetworkSettings_state);
}

static sm_ret_t Ethernet_state(sm_t *me, sm_event_t const *e)
{

        switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("Ethernet_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("Ethernet_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_UP:
        printf("go WIFI_state\r\n");
        return SM_TRAN(me, WIFI_state);
    /* code */
    }
return SM_SUPER(me, NetworkSettings_state);
}

void main(void)
{
    hsm_ctor(&hsm,init);
    hsm_init(&hsm,NULL);


    sm_event_t event = { .sig = SIG_MENU,0 };

    printf("sig_menu\r\n");
    hsm_dispatch(&hsm, &event);//进入菜单

    event.sig = SIG_ENTER_DISPLAY;//显示设置
    printf("sig_enter\r\n");
    hsm_dispatch(&hsm, &event);

    
    event.sig = SIG_DOWN;//显示设置->网络设置
    printf("sig_down\r\n");
    hsm_dispatch(&hsm, &event);


    event.sig = SIG_ENTER_WIFI;//wifi
    printf("sig_enter\r\n");
    hsm_dispatch(&hsm, &event);


    event.sig = SIG_DOWN;// wifi->ethernet
    printf("sig_down\r\n");
    hsm_dispatch(&hsm, &event);//wifi->ethernet

    event.sig = SIG_ESC;//回到主页
    printf("sig_esc\r\n");
    hsm_dispatch(&hsm, &event);//wifi->ethernet
}
```

## ✅ 运行结果：

> begine entry
> begine init
> home_State entry
> sig_menu
> go Menu
> Menu_state entry
> sig_enter
> go DisplaySettings
> DisplaySettings_state entry
> sig_down
> go NetworkSettings
> DisplaySettings_state exit
> NetworkSettings_state entry
> sig_enter
> go WIFI
> WIFI_state entry
> sig_down
> go Ethernet_state
> WIFI_state exit
> Ethernet_state entry
> sig_esc
> back home
> Ethernet_state exit
> NetworkSettings_state exit
> Menu_state exit
> home_State exit
> begine init
> home_State entry

## 注意点和理解 

> [!CAUTION]
>
> 在这种模型中，菜单页面需要停留在父状态上(菜单，显示设置，网络设置)，也要停留在子状态上，就不要在父状态中定义`SM_INIT_SIG`，否则状态机无法停留在这些父状态上
>
> 根节点最好不要是homepage，否则最终回退时，只会出现
>
> .... 
>
> sig_esc
>
> back home
> Ethernet_state exit
> NetworkSettings_state exit
> Menu_state exit
>
> 而主页进入状态将不会再次出发，即不会出现home_State entry
>
> 这是由于在dispatch中  所有节点的共父节点，不会再次进入，这里采用的办法是再定义一个主页的父节点，并设置`SM_INIT_SIG`做跳转

## ✅ HSM 优势总结

| 功能点                     | 如果用普通 FSM           | 用 HSM 实现            |
| -------------------------- | ------------------------ | ---------------------- |
| ESC 返回上级菜单           | 所有子状态都要手动写 ESC | 在父状态 Menu 统一处理 |
| 子菜单入口顺序定义（INIT） | 必须编码嵌套跳转         | INIT_SIG 自然表达      |
| 多级菜单跳转清晰           | 复杂难维护               | 层级建模逻辑清楚       |
| 子状态行为复用             | 代码重复严重             | supper 自动继承父逻辑  |

## ✅ 优化：

状态机代码过长，可以使用表驱动进行优化，比如同一级别的页面，放置到数组中。



# 其他应用场景

### 1. **电源管理系统（Power Management System）**

- **顶层状态**：运行（Running）、待机（Standby）、睡眠（Sleep）
- **子状态**：
  - 在“运行”状态下可以细分为“正常运行”、“高性能模式”、“省电模式”
- **用途**：根据负载、温度、用户操作等自动切换状态，提升系统能效。

------

### 2. **通信协议栈（如蓝牙、Zigbee、Modbus等）**

- **顶层状态**：未连接（Disconnected）、连接中（Connecting）、已连接（Connected）、错误处理
- **子状态**（Connected下）：
  - 握手中（Handshake）、认证（Authentication）、数据传输（Data Transfer）
- **用途**：清晰划分协议各阶段，方便调试与状态转移管理。

------

### 3. **嵌入式图形用户界面（GUI Navigation）**

- **顶层状态**：主界面（Home）、设置界面（Settings）、工作界面（Working）
- **子状态**（Settings下）：
  - 网络设置、显示设置、语言设置等
- **用途**：管理用户界面的多层菜单逻辑和动画转场。

------

### 4. **机器人控制系统**

- **顶层状态**：初始化（Init）、待命（Idle）、任务执行（Executing）、错误处理（Error）
- **子状态**（Executing下）：
  - 导航中、识别中、抓取中、回收中等
- **用途**：实现任务调度、异常处理和行为复用。

------

### 5. **工业设备控制（如电机驱动、PLC 控制）**

- **顶层状态**：初始化、自检、运行、报警、停止
- **子状态**：
  - “运行”中可进一步分为“启动中”、“正常运行”、“过载运行”等
- **用途**：实现系统运行生命周期中的状态转换和错误处理。

------

### 6. **多任务协作系统（如自动售货机）**

- **顶层状态**：待机、接收指令、执行命令、错误处理
- **子状态**：
  - 执行命令中可能分为：投币处理、商品选择、商品出货等
- **用途**：清晰管理各业务子流程，便于维护和扩展。

------
### 7. **汽车CAN协议、诊断、会话层**

------
### 小结：层次状态机的优势

- **模块清晰**：父状态复用子状态逻辑，结构更清晰。
- **逻辑复用**：子状态可以复用于多个父状态。
- **易于扩展和维护**：增加新功能时只需在某一状态下扩展，不必重写整个状态机。
