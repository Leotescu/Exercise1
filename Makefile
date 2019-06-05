all : main

main : main.o
	gcc -o main main.o -lpthread

main.o : main.c
	gcc -c main.c -lpthread

clean:
	rm main *.o
