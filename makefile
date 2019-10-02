# makefile for teststack
# the filename must be either Makefile or makefile


main:   main.o command.o screen.o signalHandler.o builtinCommands.o executoner.o
	gcc main.o command.o screen.o signalHandler.o builtinCommands.o executoner.o -o main

main.o: main.c
	gcc -c main.c

command.o: command.h command.c
	gcc -c command.c

screen.o: screen.h screen.c
	gcc -c screen.c

signalHandler.o: signalHandler.h signalHandler.c
	gcc -c signalHandler.c

builtinCommands.o: builtinCommands.h builtinCommands.c
	gcc -c builtinCommands.c

executoner.o: executoner.h executoner.c
	gcc -c executoner.c

clean: 
	rm *.o
