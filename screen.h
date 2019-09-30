#ifndef SCREEN_H
#define SCREEN_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct 
{

  char shellPrompt[100];

} Screen;

void SplashScreen();
void InitializeScreen(Screen* screenStruct);
void ChangeShellPrompt(Screen* screenStruct, char* newPrompt);


#endif
