#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "command.h"
#include "screen.h"
#include "builtinCommands.h"
#include "signalHandler.h"
#include "executoner.h"

#define MAX_COMMANDS 100
#define BUF_SIZE 1000

// GLOBALS
Command commands[MAX_COMMANDS];
char* tokens[1000];
int validCommand = 0;
Screen screen;



// This just prints the list of commands once they are tokenized, use for debugging
// Currently crashes :)
void DEBUG_printCommands(int cms,Command* commands)
{
     for(int i=0; i< cms; i++)
       {
         printf("Command:\n");
	 printf("\tFirst: %s\n",tokens[commands[i].first]);
	 printf("\tLast: %s\n",tokens[commands[i].last]);
	 printf("\tArgs: %d\n",commands[i].argc);
	 for(int a=0; a< commands[i].argc; a++)
	 {
	   printf("\t\t%s\n",commands[i].argv[a]); 

         }
         printf("\tSeparator: %s\n",commands[i].sep);

           if(commands[i].stdin_file != NULL)
	     printf("\tStdin: %s\n",commands[i].stdin_file);

           if(commands[i].stdout_file != NULL)
             printf("\tStdout: %s\n",commands[i].stdout_file);
       }    


}

int main()
{
  InitializeScreen(&screen);
  SplashScreen();
  SetUpSignal(&validCommand);

  int timeToQuit = 0;

  while(timeToQuit != 1)
  {
     validCommand = 1; //Will be modified by the signal handler in case we catch a signal
     char line[BUF_SIZE] = ""; //Input buffer

     printf("%s",screen.shellPrompt);

    if (fgets(line, BUF_SIZE, stdin)) {
      while (!strchr(line, '\n') && fgets(line, BUF_SIZE, stdin)) { }
     }

     //This will remove the '\n' at the end, replacing it with a '\0' 
     line[strcspn(line,"\n")] = '\0';

     // If a signal was caught, validCommand will be set to 0
     // Also, check that user did not enter an empty line
     if(!validCommand || line[0] == '\n' || line[0] == '\0' || strcmp(line,"") == 0)
     {
        printf("\n");
     	continue;
     }

     // Split into tokens, spearated by empty space
     tokenise(line,tokens," ");

     Command commands[MAX_COMMANDS];
     int cms =  separateCommands(tokens, commands,MAX_COMMANDS);

     // First thing, check if the command was a built in command ("prompt", "exit" and so on)
     // See "builtinCommands.h" to see what the method returns
     int wasBuiltIn = CheckBuiltinCommand(tokens, commands,&screen);
     timeToQuit = wasBuiltIn;

     if(wasBuiltIn == -1)
     {
	//DEBUG_printCommands(cms,commands);
        
        for(int i=0; i< cms; i++)
        {
          // Check for pipe
         if(strcmp(commands[i].sep ,PIPESEP) == 0)
          {
            ExecutePipedCommand(tokens,&commands[i], &commands[i+1]);

          }

        }
    
     }

  }

  return 0;

}
