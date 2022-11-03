# --------------------------------------------------------------------
# dc.s
#
# Desk Calculator (dc) (x86-64)
# 
# Student ID: 2022xxxx
# --------------------------------------------------------------------

    .equ   BUFFERSIZE, 32
    .equ   EOF, -1

# --------------------------------------------------------------------

.section ".rodata"

scanfFormat:
     .asciz "%s"

# --------------------------------------------------------------------

    .section ".data"

# --------------------------------------------------------------------

    .section ".bss"

buffer:
    .skip  BUFFERSIZE

stackBase:
    .skip  4
# --------------------------------------------------------------------
    .section ".text"

    # -------------------------------------------------------------
    # int powerfunc(int base, int exponent)
    # Runs the power function.  Returns result.
    # -------------------------------------------------------------
    
    .globl	powerfunc
    .type   powerfunc, @function

    # base is stored in %rdi
    # exponent is stored in %rsi

powerfunc:

    ret            # return result

# -------------------------------------------------------------
# int main(void)
# Runs desk calculator program.  Returns 0.
# -------------------------------------------------------------

    .text
    .globl  main
    .type   main, @function

main:

    pushq   %rbp
    movq    %rsp, %rbp

    # char buffer[BUFFERSIZE]
    subq    $BUFFERSIZE, %rsp

    # Note %rsp must be 16-B aligned before call

.input:
    
    # while (1) {
    # scanf("%s", buffer)
    # %al must be set to 0 before scanf call
    leaq    scanfFormat, %rdi
    leaq    -BUFFERSIZE(%rbp), %rsi
    movb    $0, %al
    call    scanf

    # check if user input == EOF
    cmp	    $EOF, %eax
    je	    .quit

    # 	if (!isdigit(buffer[0])) {
    #		if (buffer[0] == 'p') {
    #			if (stack.peek() == NULL) {
    #				printf("Stack is empty!\n");
    #			} else {
    #				printf("%d\n", (int)stack.peek());
    #			}
    #		} else if (buffer[0] == 'q') {
    #			return 0;
    #		} else if (buffer[0] == '+') {
    #			int a, b;
    #			if (stack.peek() == NULL) {
    #				printf("Stack is empty!\n");
    #				continue;
    #			}
    #			a = (int)stack.pop();
    #			if (stack.peek() == NULL) {
    #				printf("Stack is empty!\n");
    #				stack.push(a);
    #				continue;
    #			}
    #			b = (int)stack.pop();
    #			res = a + b;
    # 			stack.push(res);
    #		} else if (buffer[0] == '-') {
    #			/* ... */
    #		} else if (buffer[0] == '^') {
    #			/* ... func() ... */
    #		} else if { /* ... and so on ... */
    # 	} else { /* the first no. is a digit */
    #		int no = atoi(buffer);
    #		stack.push(no);
    # 	}
    # }

.quit:	
    # return 0
    movq    $0, %rax
    addq    $BUFFERSIZE, %rsp
    movq    %rbp, %rsp
    popq    %rbp
    ret
