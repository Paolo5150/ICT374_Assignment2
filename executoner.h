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
int ExecuteSingleCommand(char* tokens[],Command* cmd);
int ExecuteProcessedSingleCommand(char* tokens[],Command* cmd);
int ExecutePipedCommand(char* tokens[],Command* leftCmd, Command* rightCmd);

void ChildHandler(int n, siginfo_t* info, void* idk);

#endif
