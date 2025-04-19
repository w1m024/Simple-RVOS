#include "include/platform.h"

    .equ    STACK_SIZE, 1024
    .global _start

    .text
_start:
    csrr    t0, mhartid
    mv  tp, t0  
    bnez    t0, park    #停止hart ID非0的hart

    slli    t0, t0, 10  # 根据hartID计算栈的偏移
    la  sp,stacks + STACK_SIZE  # 初始化sp到栈顶

    add sp, sp, t0

    j   start_kernel

park:
    wfi
    j   park

.balign 16
stacks:
    .skip   STACK_SIZE * MAXNUM_CPU

    .end