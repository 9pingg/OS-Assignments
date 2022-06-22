default:	compile assemble link
compile:	main.c funC.c funcA.c	
	gcc -c main.c funcA.c funC.c 
assemble:	funcB.asm
	nasm -f elf64 funcB.asm -o B.o
link:	B.o funC.o funcA.o main.o
	gcc B.o funC.o funcA.o main.o -no-pie
run:	
	./a.out	
clean:
	rm a.out *.o 