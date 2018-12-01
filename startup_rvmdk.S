// startup_rvmdk.S for Tiva-C ARM Coretex-M4 MCU
//定义堆栈内存变量
Stack   EQU     0x00000100 ; 定义Stack(栈)数值为0x100 (256Byte)
Heap    EQU     0x00000000 ; 定义Heap数值为0x0 (0Byte)

//Stack内存空间
        AREA    STACK, NOINIT, READWRITE, ALIGN=3 ; 定义Stack内存空间,不初始化,可读写,按8字节对齐
StackMem ; 开始分配内存给Stack
        SPACE   Stack ; 分配Stack大小的内存空间,初始化为0
__initial_sp ; 标记栈顶地址的值

//Heap内存空间
        AREA    HEAP, NOINIT, READWRITE, ALIGN=3 ; 定义Heap内存空间,不初始化,可读写,按8字节对齐
__heap_base ; 标记堆顶地址的值
HeapMem ; 开始分配内存给Heap
        SPACE   Heap ; 分配Heap大小的内存空间,初始化为0
__heap_limit ; 标记堆结尾地址的值
        PRESERVE8 ; 指定堆栈8字节对齐

//RESET内存空间
        AREA    RESET, CODE, READONLY ; 定义RESET内存空间,将代码放入空间内,只读
        THUMB ; Thumb指令
        EXTERN  SysTickHandler ; 声明SysTick外部中断控制器符号在外部定义
///向量表初始化
        EXPORT  __Vectors ; 在外部没有定义__Vectors时导出该符号
__Vectors ; 标记向量表开始
        DCD     StackMem + Stack            ; 栈顶地址
        DCD     Reset_Handler               ; 复位
        DCD     NmiSR                       ; NMI
        DCD     FaultISR                    ; 硬件错误
        DCD     IntDefaultHandler           ; MPU错误
        DCD     IntDefaultHandler           ; 总线错误
        DCD     IntDefaultHandler           ; 使用错误
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     IntDefaultHandler           ; SVCall
        DCD     IntDefaultHandler           ; 调试监控
        DCD     0                           ; 保留
        DCD     IntDefaultHandler           ; PendSV
        DCD     IntDefaultHandler           ; SysTick
        DCD     IntDefaultHandler           ; GPIO Port A
        DCD     IntDefaultHandler           ; GPIO Port B
        DCD     IntDefaultHandler           ; GPIO Port C
        DCD     IntDefaultHandler           ; GPIO Port D
        DCD     IntDefaultHandler           ; GPIO Port E
        DCD     IntDefaultHandler           ; UART0 Rx and Tx
        DCD     IntDefaultHandler           ; UART1 Rx and Tx
        DCD     IntDefaultHandler           ; SSI0 Rx and Tx
        DCD     IntDefaultHandler           ; I2C0 Master and Slave
        DCD     IntDefaultHandler           ; PWM Fault
        DCD     IntDefaultHandler           ; PWM Generator 0
        DCD     IntDefaultHandler           ; PWM Generator 1
        DCD     IntDefaultHandler           ; PWM Generator 2
        DCD     IntDefaultHandler           ; Quadrature Encoder 0
        DCD     IntDefaultHandler           ; ADC Sequence 0
        DCD     IntDefaultHandler           ; ADC Sequence 1
        DCD     IntDefaultHandler           ; ADC Sequence 2
        DCD     IntDefaultHandler           ; ADC Sequence 3
        DCD     IntDefaultHandler           ; Watchdog timer
        DCD     IntDefaultHandler           ; Timer 0 subtimer A
        DCD     IntDefaultHandler           ; Timer 0 subtimer B
        DCD     IntDefaultHandler           ; Timer 1 subtimer A
        DCD     IntDefaultHandler           ; Timer 1 subtimer B
        DCD     IntDefaultHandler           ; Timer 2 subtimer A
        DCD     IntDefaultHandler           ; Timer 2 subtimer B
        DCD     IntDefaultHandler           ; Analog Comparator 0
        DCD     IntDefaultHandler           ; Analog Comparator 1
        DCD     IntDefaultHandler           ; Analog Comparator 2
        DCD     IntDefaultHandler           ; System Control (PLL, OSC, BO)
        DCD     IntDefaultHandler           ; FLASH Control
        DCD     IntDefaultHandler           ; GPIO Port F
        DCD     IntDefaultHandler           ; GPIO Port G
        DCD     IntDefaultHandler           ; GPIO Port H
        DCD     IntDefaultHandler           ; UART2 Rx and Tx
        DCD     IntDefaultHandler           ; SSI1 Rx and Tx
        DCD     IntDefaultHandler           ; Timer 3 subtimer A
        DCD     IntDefaultHandler           ; Timer 3 subtimer B
        DCD     IntDefaultHandler           ; I2C1 Master and Slave
        DCD     IntDefaultHandler           ; Quadrature Encoder 1
        DCD     IntDefaultHandler           ; CAN0
        DCD     IntDefaultHandler           ; CAN1
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     IntDefaultHandler           ; Hibernate
        DCD     IntDefaultHandler           ; USB0
        DCD     IntDefaultHandler           ; PWM Generator 3
        DCD     IntDefaultHandler           ; uDMA Software Transfer
        DCD     IntDefaultHandler           ; uDMA Error
        DCD     IntDefaultHandler           ; ADC1 Sequence 0
        DCD     IntDefaultHandler           ; ADC1 Sequence 1
        DCD     IntDefaultHandler           ; ADC1 Sequence 2
        DCD     IntDefaultHandler           ; ADC1 Sequence 3
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     IntDefaultHandler           ; GPIO Port J
        DCD     IntDefaultHandler           ; GPIO Port K
        DCD     IntDefaultHandler           ; GPIO Port L
        DCD     IntDefaultHandler           ; SSI2 Rx and Tx
        DCD     IntDefaultHandler           ; SSI3 Rx and Tx
        DCD     IntDefaultHandler           ; UART3 Rx and Tx
        DCD     IntDefaultHandler           ; UART4 Rx and Tx
        DCD     IntDefaultHandler           ; UART5 Rx and Tx
        DCD     IntDefaultHandler           ; UART6 Rx and Tx
        DCD     IntDefaultHandler           ; UART7 Rx and Tx
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     IntDefaultHandler           ; I2C2 Master and Slave
        DCD     IntDefaultHandler           ; I2C3 Master and Slave
        DCD     IntDefaultHandler           ; Timer 4 subtimer A
        DCD     IntDefaultHandler           ; Timer 4 subtimer B
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     0                           ; 保留
        DCD     IntDefaultHandler           ; Timer 5 subtimer A
        DCD     IntDefaultHandler           ; Timer 5 subtimer B
        DCD     IntDefaultHandler           ; Wide Timer 0 subtimer A
        DCD     IntDefaultHandler           ; Wide Timer 0 subtimer B
        DCD     IntDefaultHandler           ; Wide Timer 1 subtimer A
        DCD     IntDefaultHandler           ; Wide Timer 1 subtimer B
        DCD     IntDefaultHandler           ; Wide Timer 2 subtimer A
        DCD     IntDefaultHandler           ; Wide Timer 2 subtimer B
        DCD     IntDefaultHandler           ; Wide Timer 3 subtimer A
        DCD     IntDefaultHandler           ; Wide Timer 3 subtimer B
        DCD     IntDefaultHandler           ; Wide Timer 4 subtimer A
        DCD     IntDefaultHandler           ; Wide Timer 4 subtimer B
        DCD     IntDefaultHandler           ; Wide Timer 5 subtimer A
        DCD     IntDefaultHandler           ; Wide Timer 5 subtimer B
        DCD     IntDefaultHandler           ; FPU
        DCD     IntDefaultHandler           ; PECI 0
        DCD     IntDefaultHandler           ; LPC 0
        DCD     IntDefaultHandler           ; I2C4 Master and Slave
        DCD     IntDefaultHandler           ; I2C5 Master and Slave
        DCD     IntDefaultHandler           ; GPIO Port M
        DCD     IntDefaultHandler           ; GPIO Port N
        DCD     IntDefaultHandler           ; Quadrature Encoder 2
        DCD     IntDefaultHandler           ; Fan 0
        DCD     0                           ; 保留
        DCD     IntDefaultHandler           ; GPIO Port P (Summary or P0)
        DCD     IntDefaultHandler           ; GPIO Port P1
        DCD     IntDefaultHandler           ; GPIO Port P2
        DCD     IntDefaultHandler           ; GPIO Port P3
        DCD     IntDefaultHandler           ; GPIO Port P4
        DCD     IntDefaultHandler           ; GPIO Port P5
        DCD     IntDefaultHandler           ; GPIO Port P6
        DCD     IntDefaultHandler           ; GPIO Port P7
        DCD     IntDefaultHandler           ; GPIO Port Q (Summary or Q0)
        DCD     IntDefaultHandler           ; GPIO Port Q1
        DCD     IntDefaultHandler           ; GPIO Port Q2
        DCD     IntDefaultHandler           ; GPIO Port Q3
        DCD     IntDefaultHandler           ; GPIO Port Q4
        DCD     IntDefaultHandler           ; GPIO Port Q5
        DCD     IntDefaultHandler           ; GPIO Port Q6
        DCD     IntDefaultHandler           ; GPIO Port Q7
        DCD     IntDefaultHandler           ; GPIO Port R
        DCD     IntDefaultHandler           ; GPIO Port S
        DCD     IntDefaultHandler           ; PWM 1 Generator 0
        DCD     IntDefaultHandler           ; PWM 1 Generator 1
        DCD     IntDefaultHandler           ; PWM 1 Generator 2
        DCD     IntDefaultHandler           ; PWM 1 Generator 3
        DCD     IntDefaultHandler           ; PWM 1 Fault
///复位标记初始化
        EXPORT  Reset_Handler ; 在外部没有定义Reset_Handler时导出该符号
Reset_Handler ; 标记复位开始
        ; 启用浮点数支持
        MOVW    R0, #0xED88 ; 设置R0为16位常量0xED88
        MOVT    R0, #0xE000 ; 设置R0的高16位为0xE000, 此时R0为0xE000ED88
        LDR     R1, [R0] ; 读取R0的值并存入R1
        ORR     R1, #0x00F00000 ; 将R1的值按位和0x00F00000取或并存入R1
        STR     R1, [R0] ; 将R1的值存入R0 (0xE0F0ED88)
///调用主程序
        IMPORT  __main ; 调用__main库
        B       __main ; 跳转到__main标记
///中断标记初始化
NmiSR ; 标记NmiSR开始
        B       NmiSR ; 跳转到NmiSR标记
FaultISR ; 标记FaultISR开始
        B       FaultISR ; 跳转到FaultISR标记
IntDefaultHandler ; 标记IntDefaultHandler开始
        B       IntDefaultHandler ; 跳转到IntDefaultHandler标记
        ALIGN ; 对齐字节数

//.text内存空间
        AREA    |.text|, CODE, READONLY ; 定义.text内存空间,将代码放入空间内,只读
    IF :DEF: __MICROLIB ; 判断是否定义__MICROLIB
        EXPORT  __initial_sp ; 在外部没有定义__initial_sp时导出该符号
        EXPORT  __heap_base ; 在外部没有定义__heap_base时导出该符号
        EXPORT  __heap_limit ; 在外部没有定义__heap_limit时导出该符号
    ELSE
        IMPORT  __use_two_region_memory ; 调用__use_two_region_memory库
        EXPORT  __user_initial_stackheap ; 在外部没有定义__user_initial_stackheap时导出该符号
__user_initial_stackheap ; 标记__user_initial_stackheap开始
        LDR     R0, =HeapMem ; 将R0设置为HeapMem(堆头地址)的值
        LDR     R1, =(StackMem + Stack) ; 将R1设置为StackMem + Stack(栈尾地址)的值
        LDR     R2, =(HeapMem + Heap) ; 将R2设置为HeapMem + Heap(堆尾地址)的值
        LDR     R3, =StackMem ; 将R2设置为StackMem(栈头地址)的值
        BX      LR ; 跳转到LR寄存器的地址值的位置
    ENDIF
        ALIGN ; 对齐字节数

        END ; 汇编启动代码结束
