#include "signalHandler.h"
#include <stdio.h>
#include <signal.h>

void CatchSignal(int signalNumber)
{
}

void SetUpSignal()
{
    struct sigaction act;
	act.sa_flags = 0;
	act.sa_handler = CatchSignal;

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}
