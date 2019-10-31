#include "signalHandler.h"
#include "executoner.h"
#include <stdio.h>
#include <signal.h>

void SignalHandler(int signalNumber)
{
   // Set validCommand to false so the line written won't make it into the toknizer in main.c
   *setValidCommand = 0;   
}

void SetUpSignal(int* validCommand)
{
        setValidCommand = validCommand;

        struct sigaction act;
	act.sa_flags = 0;
	act.sa_handler = SignalHandler;

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGKILL, &act, NULL);
	sigaction(SIGTSTP, &act, NULL);

	// Set up handler for SIGCHLD signal (see executoner.c)
  	struct sigaction childDone;		

  	childDone.sa_flags = SA_SIGINFO;
  	childDone.sa_sigaction = ChildHandler;
  	sigaction(SIGCHLD,&childDone,NULL);
}
