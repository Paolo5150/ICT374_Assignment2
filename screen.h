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

// Display spash creen
void SplashScreen();

// Initialize screen parameters
void InitializeScreen(Screen* screenStruct);

// Change shell prompt (default "%>")
void ChangeShellPrompt(Screen* screenStruct, char* newPrompt);


#endif
