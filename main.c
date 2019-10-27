#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "command.h"
#include "screen.h"
#include "builtinCommands.h"
#include "signalHandler.h"
#include "executoner.h"

#define MAX_COMMANDS 100
#define MAX_TOKENS 1000
#define BUF_SIZE 1000

// GLOBALS
Command commands[MAX_COMMANDS];
char* tokens[MAX_TOKENS];
int validCommand = 0;
Screen screen;

// This just prints the list of commands once they are tokenized, use for debugging

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
    int again = 1;
    char* linePtr;
     validCommand = 1; //Will be modified by the signal handler in case we catch a signal
     char line[BUF_SIZE] = ""; //Input buffer

     printf("%s",screen.shellPrompt);

     while (again) {
           again = 0;
           linePtr = fgets(line, BUF_SIZE, stdin);
           if (linePtr == NULL) 
               if (errno == EINTR)
                    again = 1;        // signal interruption, read again
      }
    
     //This will remove the '\n' at the end, replacing it with a '\0' 
     line[strcspn(line,"\n")] = '\0';
     
     //Check for \t
     for( int i=0; i< strlen(line); i++)
     {
       if(line[i] == '\t')
       {
         line[i] = ' ';
       }
       
     }
 
     
     // If a signal was caught, validCommand will be set to 0
     // Also, check that user did not enter an empty line
    if(!validCommand || line[0] == '\n' || line[0] == '\0' || strcmp(line,"") == 0 )
     {
        //printf("\n");
     	continue;
     }

     // To lower case
     for(int i=0; i < strlen(line); i++)
     {
       line[i] = tolower(line[i]);
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
        for(int i=0; i< cms; i++)
        {


          // Check for pipe
         if(strcmp(commands[i].sep ,PIPESEP) == 0)
          {
            // If the command has pipe, batch the cmd and all subsequent commands if they have a pipe (including last one with no pipe)
            Command* pipeCommands[MAX_COMMANDS];
            int index = 0;
            int isPipe = 1;            
            for(int j=i; j< cms && isPipe; j++)
            {
              if(strcmp(commands[j].sep ,PIPESEP) == 0)
              {
                pipeCommands[index] = &commands[j];
                index++;
                i++;
              }
              else
                isPipe = 0;              
            }     
          pipeCommands[index] = &commands[i]; 
          index++;   
       
         ExecutePipedCommand(tokens,*pipeCommands,index);
         }
         else
         {
            ExecuteProcessedSingleCommand(tokens,&commands[i]);
         }
         
        }    
     }

  }

  return 0;

}
