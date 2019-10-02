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

// This method checks for builtin commands. Can probably be put in its own header/c file
// Returns 1 if the command is one of the builtin commands
// Returns 1 if the command is the builtin commands EXIT
// Resutns -1 otherwise
int CheckBuiltinCommand(char* tokens[],Command* commands, Screen* screen);


#endif
