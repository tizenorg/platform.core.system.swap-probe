
#if defined(__x86_64__) || defined(_M_X64)
    /* x86 64-bit ----------------------------------------------- */

#elif defined(__i386) || defined(_M_IX86)

.text
.global _da_call_original
.type _da_call_original, @function
_da_call_original:
		push %ebp
		mov  %esp, %ebp

	#local esp
		sub  $0x40, %esp

		push %ebx
		push %ecx
		push %edx

		movl 0xc(%ebp),%ecx  #args
		movl 0x10(%ebp), %ebx #args_count
		addl $0x01, %ebx #count + 1

		#(args_count + 1) * 4 add to esp
		shl  $2, %ebx
		sub  %ebx, %esp

	#for (i = 0; i <= arg; i++) {
		movl $0, %eax
		jmp  for_i_cmp
		for_i:
			mov  (%ecx), %edx
			mov  %edx, (%esp)
			add  $4, %esp
			add  $4, %ecx
			add  $4, %eax
		for_i_cmp:
		cmpl %ebx, %eax
		jl   for_i
	#}

	#shift esp
		sub  %ebx, %esp

		movl 8(%ebp), %eax  #funcp
		call *(%eax)

	#restore esp
		add  %ebx, %esp

		pop  %edx
		pop  %ecx
		pop  %ebx

	#restore local esp
		add  $0x40, %esp
		pop  %ebp
		ret

#elif defined(__arm__)
.text
.global _da_call_original
.type _da_call_original, %function

_da_call_original:
	push {fp, lr}
	push {r5, r6, r7, r8}
	#local stack
	add  fp, sp, #4
	sub  sp, sp, #20

	str  r0, [fp, #-12]
	str  r1, [fp, #-16]
	str  r2, [fp, #-20]

	#args
	ldr  r5, [fp, #-16]
	#args_count
	ldr  r6, [fp, #-20]
	#args++
	add  r6, #1

	#stack prepare
	#stack size
	lsl  r6, r6, #2
	sub  sp, sp, r6
	sub  sp, sp, #16

#	for (i = 4; i <= arg; i++) {
	mov  r7, #16
	b    for_i_cmp
	for_i:
		ldr  r8, [r5, r7]
		str  r8, [sp, r7]
		add  r7, r7, #4
	for_i_cmp:
	cmp  r7, r6
	blt  for_i
#	}

	#shift stack
	add  sp, sp, #16
	#first 4 args
	ldr  r0, [r5, #0 ]
	ldr  r1, [r5, #4 ]
	ldr  r2, [r5, #8 ]
	ldr  r3, [r5, #12]

	ldr  r5, [fp, #-12]
	ldr  r5, [r5]
	blx  r5

	#stack restore
	add  sp, sp, r6

	#local stack restore
	add  sp, sp, #20
	pop  {r5, r6, r7, r8}
	pop  {fp, pc}
#endif
