# makefile


main:   main.o command.o screen.o signalHandler.o builtinCommands.o executoner.o wildcard.o
	gcc main.o command.o screen.o signalHandler.o builtinCommands.o executoner.o wildcard.o -Wall -o main

main.o: main.c
	gcc -c -Wall main.c

command.o: command.h command.c
	gcc -c -Wall command.c

screen.o: screen.h screen.c
	gcc -c -Wall screen.c

signalHandler.o: signalHandler.h signalHandler.c
	gcc -c -Wall signalHandler.c

builtinCommands.o: builtinCommands.h builtinCommands.c
	gcc -c -Wall builtinCommands.c

executoner.o: executoner.h executoner.c
	gcc -c -Wall executoner.c

wildcard.o: wildcard.h wildcard.c
	gcc -c -Wall wildcard.c

clean: 
	rm *.o
