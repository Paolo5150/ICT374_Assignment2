#ifndef BUILTINCOMMANDS_H
#define BUILTINCOMMANDS_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "screen.h"
#include "command.h"

#define EXIT_COMMAND "exit"
#define PROMPT_COMMAND "prompt"
#define CHANGEDIR_COMMAND "cd"
#define PWD_COMMAND "pwd"


int CheckBuiltinCommand(char* tokens[],Command* commands, Screen* screen);


#endif
