#include "screen.h"

char shellPrompt[100];

void SplashScreen()
{
    system("clear");
    printf("\n\n\n\n\n");
    printf("\t----------------------------------------------\n");
    printf("\t     WELCOME TO THE BEST SHELL EVER!\n");
    printf("\t----------------------------------------------\n");
    sleep(1);
    system("clear");

}

void InitializeScreen(Screen* screenStruct)
{
  (*screenStruct).shellPrompt[0] = '%';
  (*screenStruct).shellPrompt[1] = '>';
}

void ChangeShellPrompt(Screen* screenStruct,char* newPrompt)
{  
  strcpy(screenStruct->shellPrompt,newPrompt);
  strcat(screenStruct->shellPrompt,">");
}
