	.file	"epoll.cpp"
	.section	.text.startup,"ax",@progbits
	.align	16, 0x90
	.type	__cxx_global_var_init,@function
__cxx_global_var_init:                  # @__cxx_global_var_init
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Ltmp2:
	.cfi_def_cfa_offset 16
.Ltmp3:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp4:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	leaq	_ZStL8__ioinit, %rdi
	callq	_ZNSt8ios_base4InitC1Ev
	leaq	_ZNSt8ios_base4InitD1Ev, %rdi
	leaq	_ZStL8__ioinit, %rsi
	leaq	__dso_handle, %rdx
	callq	__cxa_atexit
	movl	%eax, -4(%rbp)          # 4-byte Spill
	addq	$16, %rsp
	popq	%rbp
	ret
.Ltmp5:
	.size	__cxx_global_var_init, .Ltmp5-__cxx_global_var_init
	.cfi_endproc

	.text
	.globl	_Z14SetNonblockingi
	.align	16, 0x90
	.type	_Z14SetNonblockingi,@function
_Z14SetNonblockingi:                    # @_Z14SetNonblockingi
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Ltmp8:
	.cfi_def_cfa_offset 16
.Ltmp9:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp10:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	$3, %esi
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %edi
	movb	$0, %al
	callq	fcntl
	movl	$4, %esi
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %eax
	orl	$2048, %eax             # imm = 0x800
	movl	%eax, -12(%rbp)
	movl	-4(%rbp), %edi
	movl	-12(%rbp), %edx
	movb	$0, %al
	callq	fcntl
	movl	-8(%rbp), %edx
	movl	%eax, -16(%rbp)         # 4-byte Spill
	movl	%edx, %eax
	addq	$16, %rsp
	popq	%rbp
	ret
.Ltmp11:
	.size	_Z14SetNonblockingi, .Ltmp11-_Z14SetNonblockingi
	.cfi_endproc

	.globl	_Z5addfdiib
	.align	16, 0x90
	.type	_Z5addfdiib,@function
_Z5addfdiib:                            # @_Z5addfdiib
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Ltmp14:
	.cfi_def_cfa_offset 16
.Ltmp15:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp16:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movb	%dl, %al
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	andb	$1, %al
	movb	%al, -9(%rbp)
	movl	-8(%rbp), %edx
	movl	%edx, -20(%rbp)
	movl	$1, -24(%rbp)
	testb	$1, -9(%rbp)
	je	.LBB2_2
# BB#1:
	movl	-24(%rbp), %eax
	orl	$2147483648, %eax       # imm = 0x80000000
	movl	%eax, -24(%rbp)
.LBB2_2:
	movl	$1, %esi
	leaq	-24(%rbp), %rcx
	movl	-4(%rbp), %edi
	movl	-8(%rbp), %edx
	callq	epoll_ctl
	movl	-8(%rbp), %edi
	movl	%eax, -28(%rbp)         # 4-byte Spill
	callq	_Z14SetNonblockingi
	movl	%eax, -32(%rbp)         # 4-byte Spill
	addq	$32, %rsp
	popq	%rbp
	ret
.Ltmp17:
	.size	_Z5addfdiib, .Ltmp17-_Z5addfdiib
	.cfi_endproc

	.globl	_Z17print_Client_InfoP11sockaddr_in
	.align	16, 0x90
	.type	_Z17print_Client_InfoP11sockaddr_in,@function
_Z17print_Client_InfoP11sockaddr_in:    # @_Z17print_Client_InfoP11sockaddr_in
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Ltmp20:
	.cfi_def_cfa_offset 16
.Ltmp21:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp22:
	.cfi_def_cfa_register %rbp
	subq	$96, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	movzwl	2(%rdi), %edi
	callq	ntohl
	leaq	-48(%rbp), %rcx
	movl	$0, %esi
	movabsq	$20, %rdx
	movl	%eax, %eax
	movl	%eax, %r8d
	movq	%r8, -16(%rbp)
	movq	%rcx, %r8
	movq	%r8, %rdi
	movq	%rcx, -64(%rbp)         # 8-byte Spill
	callq	memset
	movq	-8(%rbp), %rcx
	movl	4(%rcx), %eax
	movl	%eax, -56(%rbp)
	movl	-56(%rbp), %edi
	callq	inet_ntoa
	leaq	_ZSt4cout, %rdi
	leaq	.L.str, %rsi
	movq	-64(%rbp), %rcx         # 8-byte Reload
	movq	%rdi, -72(%rbp)         # 8-byte Spill
	movq	%rcx, %rdi
	movq	%rsi, -80(%rbp)         # 8-byte Spill
	movq	%rax, %rsi
	callq	strcpy
	movq	-72(%rbp), %rdi         # 8-byte Reload
	movq	-80(%rbp), %rsi         # 8-byte Reload
	movq	%rax, -88(%rbp)         # 8-byte Spill
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	leaq	.L.str1, %rsi
	movq	%rax, %rdi
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	leaq	-48(%rbp), %rsi
	movq	%rax, %rdi
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	leaq	.L.str2, %rsi
	movq	%rax, %rdi
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-16(%rbp), %rsi
	movq	%rax, %rdi
	callq	_ZNSolsEm
	leaq	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %rsi
	movq	%rax, %rdi
	callq	_ZNSolsEPFRSoS_E
	movq	%rax, -96(%rbp)         # 8-byte Spill
	addq	$96, %rsp
	popq	%rbp
	ret
.Ltmp23:
	.size	_Z17print_Client_InfoP11sockaddr_in, .Ltmp23-_Z17print_Client_InfoP11sockaddr_in
	.cfi_endproc

	.globl	_Z12epollHandlerP11epoll_eventiii
	.align	16, 0x90
	.type	_Z12epollHandlerP11epoll_eventiii,@function
_Z12epollHandlerP11epoll_eventiii:      # @_Z12epollHandlerP11epoll_eventiii
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Ltmp26:
	.cfi_def_cfa_offset 16
.Ltmp27:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp28:
	.cfi_def_cfa_register %rbp
	subq	$4176, %rsp             # imm = 0x1050
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movl	%ecx, -20(%rbp)
	movl	$0, -4132(%rbp)
.LBB4_1:                                # =>This Loop Header: Depth=1
                                        #     Child Loop BB4_6 Depth 2
	movl	-4132(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jge	.LBB4_15
# BB#2:                                 #   in Loop: Header=BB4_1 Depth=1
	movslq	-4132(%rbp), %rax
	movq	-8(%rbp), %rcx
	imulq	$12, %rax, %rax
	addq	%rax, %rcx
	movl	4(%rcx), %edx
	movl	%edx, -4136(%rbp)
	movl	-4136(%rbp), %edx
	cmpl	-20(%rbp), %edx
	jne	.LBB4_4
# BB#3:                                 #   in Loop: Header=BB4_1 Depth=1
	movl	-20(%rbp), %edi
	callq	_Z21Get_socket_for_clienti
	movl	$1, %edx
	movl	%eax, -4140(%rbp)
	movl	-16(%rbp), %edi
	movl	-4140(%rbp), %esi
	callq	_Z5addfdiib
	jmp	.LBB4_13
.LBB4_4:                                #   in Loop: Header=BB4_1 Depth=1
	movslq	-4132(%rbp), %rax
	movq	-8(%rbp), %rcx
	imulq	$12, %rax, %rax
	addq	%rax, %rcx
	movl	(%rcx), %edx
	andl	$1, %edx
	cmpl	$0, %edx
	je	.LBB4_12
# BB#5:                                 #   in Loop: Header=BB4_1 Depth=1
	movl	$0, %esi
	movabsq	$1024, %rdx             # imm = 0x400
	leaq	-4128(%rbp), %rax
	movq	%rax, %rdi
	callq	memset
	movl	$0, -4144(%rbp)
.LBB4_6:                                #   Parent Loop BB4_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movabsq	$1023, %rdx             # imm = 0x3FF
	movl	$0, %ecx
	leaq	-4128(%rbp), %rax
	movl	-4136(%rbp), %edi
	movslq	-4144(%rbp), %rsi
	addq	%rsi, %rax
	movq	%rax, %rsi
	callq	recv
	movq	%rax, -4152(%rbp)
	cmpq	$0, %rax
	jle	.LBB4_8
# BB#7:                                 #   in Loop: Header=BB4_6 Depth=2
	leaq	_ZSt4cout, %rdi
	leaq	-4128(%rbp), %rsi
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	leaq	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %rsi
	movq	%rax, %rdi
	callq	_ZNSolsEPFRSoS_E
	movq	-4152(%rbp), %rsi
	movslq	-4144(%rbp), %rdi
	addq	%rsi, %rdi
	movl	%edi, %ecx
	movl	%ecx, -4144(%rbp)
	movq	%rax, -4160(%rbp)       # 8-byte Spill
	jmp	.LBB4_6
.LBB4_8:                                #   in Loop: Header=BB4_1 Depth=1
	cmpq	$-1, -4152(%rbp)
	jne	.LBB4_11
# BB#9:                                 #   in Loop: Header=BB4_1 Depth=1
	callq	__errno_location
	cmpl	$11, (%rax)
	je	.LBB4_11
# BB#10:                                #   in Loop: Header=BB4_1 Depth=1
	leaq	_ZSt4cout, %rdi
	leaq	.L.str3, %rsi
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	leaq	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %rsi
	movq	%rax, %rdi
	callq	_ZNSolsEPFRSoS_E
	movq	%rax, -4168(%rbp)       # 8-byte Spill
.LBB4_11:                               #   in Loop: Header=BB4_1 Depth=1
	jmp	.LBB4_12
.LBB4_12:                               #   in Loop: Header=BB4_1 Depth=1
	jmp	.LBB4_13
.LBB4_13:                               #   in Loop: Header=BB4_1 Depth=1
	jmp	.LBB4_14
.LBB4_14:                               #   in Loop: Header=BB4_1 Depth=1
	movl	-4132(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -4132(%rbp)
	jmp	.LBB4_1
.LBB4_15:
	addq	$4176, %rsp             # imm = 0x1050
	popq	%rbp
	ret
.Ltmp29:
	.size	_Z12epollHandlerP11epoll_eventiii, .Ltmp29-_Z12epollHandlerP11epoll_eventiii
	.cfi_endproc

	.section	.text.startup,"ax",@progbits
	.align	16, 0x90
	.type	_GLOBAL__I_a,@function
_GLOBAL__I_a:                           # @_GLOBAL__I_a
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Ltmp32:
	.cfi_def_cfa_offset 16
.Ltmp33:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp34:
	.cfi_def_cfa_register %rbp
	callq	__cxx_global_var_init
	popq	%rbp
	ret
.Ltmp35:
	.size	_GLOBAL__I_a, .Ltmp35-_GLOBAL__I_a
	.cfi_endproc

	.type	_ZStL8__ioinit,@object  # @_ZStL8__ioinit
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"client:"
	.size	.L.str, 8

	.type	.L.str1,@object         # @.str1
.L.str1:
	.asciz	"ip:"
	.size	.L.str1, 4

	.type	.L.str2,@object         # @.str2
.L.str2:
	.asciz	"port:"
	.size	.L.str2, 6

	.type	.L.str3,@object         # @.str3
.L.str3:
	.asciz	"read error"
	.size	.L.str3, 11

	.section	.init_array,"aw",@init_array
	.align	8
	.quad	_GLOBAL__I_a

	.ident	"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"
	.section	".note.GNU-stack","",@progbits
