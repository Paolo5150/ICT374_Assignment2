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

char** IsPath(char* line, char** args, int argc);
//"Public" method for determining if redirect is applicable and applies it, oldout and oldin can be used to determine whether a redirection occurred
void Redirect(char* tokens[], Command* cmd, int* oldOut, int* oldIn);
//Redirects the stdin, called by Redirect
int RedirectInput(char* inputFilename);
void RedirectInputFD(int fd);
//Redirects the stdout, called by Redirect
int RedirectOutput(char* outputFilename);
void RedirectOutputFD(int fd);
int ExecuteSingleCommand(char* tokens[],Command* cmd);
int ExecuteProcessedSingleCommand(char* tokens[],Command* cmd);
int ExecutePipedCommand(char* tokens[],Command* leftCmd, Command* rightCmd);

void ChildHandler(int n, siginfo_t* info, void* idk);

#endif
