#include "signalHandler.h"
#include "executoner.h"
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

	// Set up handler for SIGCHLD signal (see executoner.c)
  	struct sigaction childDone;
		
  	childDone.sa_flags = SA_RESTART;
  	childDone.sa_flags |= SA_SIGINFO;
  	childDone.sa_sigaction = ChildHandler;
  	sigaction(SIGCHLD,&childDone,NULL);
}
