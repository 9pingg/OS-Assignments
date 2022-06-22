default:	pre compile assemble link 
pre:	main.c
	gcc -E main.c -o main.i
compile:	main.i
	gcc -S main.i -o main.s
assemble:	main.s
	gcc -c main.s -o main.o
link:	main.o
	gcc main.o -o main -lpthread
run:	main
	./main
clean:
	rm main.i main.s main.o main
	clear	