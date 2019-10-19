#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

// Set "validCommand" to 0 so the input is not sent to the tokenizer
void SignalHandler(int signalNumber);

// Set up signalhandlers
void SetUpSignal(int* validCommand);

// Set to false when user enters CTRL+C and other signals commands
int* setValidCommand;

#endif
