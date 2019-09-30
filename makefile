# makefile for teststack
# the filename must be either Makefile or makefile


main:   main.o command.o screen.o signalHandler.o
	gcc main.o command.o screen.o signalHandler.o -o main

main.o: main.c
	gcc -c main.c

command.o: command.h command.c
	gcc -c command.c

screen.o: screen.h screen.c
	gcc -c screen.c

signalHandler.o: signalHandler.h signalHandler.c
	gcc -c signalHandler.c

clean: 
	rm *.o
