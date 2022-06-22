To compile and link the program use 'make'.

Following commands are used.
    gcc -c main.c funcA.c funC.c // compile all the c files
	nasm -f elf64 funcB.asm -o B.o // compile assembly file
	gcc B.o funC.o funcA.o main.o -no-pie // link all the object files

to run the executable use 'make run' or ./a.out
program asks the user to enter a number, to pass as a 64 bit integer to funcB() (assembly func)
for ex: enter 97 -> 'a' is printed.



To get the desired output:

    1) funcA is called in main using A()

    2) In A() user is asked to enter a number(64bit) in decimal which is passed to the assembly function B.
    
    3)In func B the 64 bit number is interpreted as a 8byte ascii string and ascii characters are printed,
        using: 
        and rax, 0xFF ; rax gets the lower 8 bits of the 64 bit integer stored in rax. then    
                      ; (for full implmentation see the code) value stored in rax, is stored in an array
                      ; after each loop first 8 bits of the integer are stored in an array ,
                      ; and the integer is right shifted by 8 bits in every iteration. After that shr is used
        shr rax, 8      ;which right shifts the 64 bit integer by 8
        ; after storing the ascii chracters in the array it is printed by write system call

    4) Now when the printing of the array is done, the stack is manipulated in a way that on 'ret' the control passes to func C() not func A() which was the caller function.
    this is done by storing the address of C in base pointer and pushing it on to the stack then calling ret.
    mov rbp, $C
    push rbp
    ret 
    5)
    After ret in B(), control passes to C() function which uses the exit system call to terminate the program, so control doesnt pass to main function again program ends in C() function.
System calls used:
    write() ->  arguments rax = 1, rdi = 1(stdout), rsi = location of the buffer holding the msg to print, 
    rdx = size of the buffer
    exit() -> arguments rax = 60, rdi = 0

function definition of function B written in assembly is, void B(int64_t);
A calls B by, B(num); where num = int64_t