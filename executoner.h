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

int IsPath(char* line, char** args, int argc, char** newArgs);
int ExecuteSingleCommand(char* tokens[],Command* cmd);
int ExecuteProcessedSingleCommand(char* tokens[],Command* cmd);
int ExecutePipedCommand(char* tokens[],Command* leftCmd, Command* rightCmd);


#endif
