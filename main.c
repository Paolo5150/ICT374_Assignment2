#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "command.h"
#include "screen.h"
#include "commandLibrary.h"
#include "signalHandler.h"


Command commands[100];
char shellPrompt[100] = ">";


// Returns 1 if the command is "exit"
int CheckCommand(char* command)
{
    printf("Checking command %s",command);
    if(strcmp(command,EXIT_COMMAND) == 0)
    {
        return 1;
    }

    return 0;

}

int main()
{

  SplashScreen();
  SetUpSignal();

  int timeToQuit = 0;

  while(timeToQuit == 0)
  {
     char line[1000];
     char* tokens[1000];

     printf("%s",shellPrompt);
     fgets(line,1000,stdin);
     line[strcspn(line,"\n")] = '\0'; //This will remove the '\n' at the end, replacing it with a '\0'

     tokenise(line,tokens," ");

      Command commands[10];
      int cms =  separateCommands(tokens, commands);


      for(int i=0; i< cms; i++)
      {
         printf("Command:\n");
         printf("\tFirst: %s\n",tokens[commands[i].first]);
         printf("\tLast: %s\n",tokens[commands[i].last]);
         printf("\tArgs: %d\n",commands[i].argc);
         for(int a=1; a< commands[i].argc; a++)
         {
           printf("\t\t%s\n",commands[i].argv[a]);
         }
         printf("\tSeparator: %s\n",commands[i].sep);
         if(commands[i].stdin_file != NULL)
                printf("\tStdin: %s\n",commands[i].stdin_file);
         if(commands[i].stdout_file != NULL)
                printf("\tStdout: %s\n",commands[i].stdout_file);
      }


      timeToQuit = CheckCommand(tokens[commands[0].first]);

  }

  return 0;

}
