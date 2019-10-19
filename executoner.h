#ifndef EXECUTONER_H
#define EXECUTONER_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <stdio.h>
#include "screen.h"
#include "command.h"

#define MAX_PIPES 1000

// Check if the command is a "path type" (eg. "bin/ls")
int IsPath(char* line, char** tks);

//"Public" method for determining if redirect is applicable and applies it, oldout and oldin can be used to determine whether a redirection occurred
void Redirect(char* tokens[], Command* cmd, int* oldOut, int* oldIn);

//Redirects the stdin, called by Redirect
int RedirectInput(char* inputFilename);

void RedirectInputFD(int fd);

//Redirects the stdout, called by Redirect
int RedirectOutput(char* outputFilename);

void RedirectOutputFD(int fd);

// Execute command using the appropriate exec function
void ExecuteSingleCommand(char* tokens[],Command* cmd);

// Spawn process to execute command
void ExecuteProcessedSingleCommand(char* tokens[],Command* cmd);

// Special function to execute commands joint with the pipe separator
int ExecutePipedCommand(char* tokens[],Command* pipedCmd, int size);

// Callback for child process that are terminated
void ChildHandler(int n, siginfo_t* info, void* idk);

// Determines whether to wait for the process or not
int CheckForWait(Command* cmd, int pid);

#endif
