#include "signalHandler.h"
#include <stdio.h>
#include <signal.h>

void CatchSignal(int signalNumber)
{
   *setValidCommand = 0;
   
   fseek(stdin,0,SEEK_END);
}

void SetUpSignal(int* validCommand)
{
        setValidCommand = validCommand;
        struct sigaction act;
	act.sa_flags = 0;
	act.sa_handler = CatchSignal;

	//sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGKILL, &act, NULL);
}
