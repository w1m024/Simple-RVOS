	.equ	STACK_SIZE, 1024
	.equ	MAXNUM_CPU, 8

	.global	_start

	.text
_start:
	csrr	t0, mhartid
	mv	tp, t0
	bnez	t0, park

	# 初始化bss
	la	a0, _bss_start
	la	a1, _bss_end
	bgeu	a0, a1, 2f 


1:	
	sw zero, (a0)
	addi	a0, a0, 4
	bltu	a0, a1, 1b

2:
	# 初始化栈
	slli	t0, t0, 10
	la	sp, stacks + STACK_SIZE
	add	sp, sp, t0

#ifdef CONFIG_SYSCALL
	# PMP配置，兼容QEMU在U模式下运行的要求
	li	t0, 0xffffffff
	csrw	pmpaddr0, t0 # 表示地址范围的结束
	li	t0, 0xf # bit 0 (R): 1 - 允许读
				# bit 1 (W): 1 - 允许写
				# bit 2 (X): 1 - 允许执行
				# bit 3-4 (A): 11 - NAPOT (Naturally Aligned Power-of-2 regions) 模式
				# bit 5-6 (Reserved): 0
				# bit 7 (L): 0 - 未锁定
	csrw	pmpcfg0, t0 # 允许 U 模式访问所有物理内存。
#endif

#ifdef CONFIG_SYSCALL
	li	t0, 3 << 11	#这里没有设置 MPIE 位。因为即使 MPIE=0，mret 返回到 U 模式后，
					# M 模式的中断仍然是全局使能的 (MIE 不受影响)。
	csrc	mstatus, t0
#else
	# start_kernel末尾会调用mret切换到第一个任务，准备好mstatus
	li	t0, 3 << 11 | 1 << 7	# 3 << 11: 设置 mstatus.MPP 为 3 (Machine mode)
								# 1 << 7:  设置 mstatus.MPIE 为 1 (Machine Previous Interrupt Enable)
	csrs	mstatus, t0
#endif

	j	start_kernel
park:
	wfi
	j park
	
.balign 16
stacks:
	.skip	STACK_SIZE * MAXNUM_CPU

	.end