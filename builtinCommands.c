#include "builtinCommands.h"


// This method checks for builtin commands. Can probably be put in its own header/c file
// Returns 1 if the command is "exit"
int CheckBuiltinCommand(char* tokens[],Command* commands, Screen* screen)
{ 
    int ret = 0;
    if(strcmp(tokens[commands[0].first],EXIT_COMMAND) == 0)
    {
        printf("\nBye!\n");
        ret = 1;
    }
    else if(strcmp(tokens[commands[0].first],PROMPT_COMMAND) == 0)
    {
       if(commands[0].argv[1] == NULL)      
         printf("No argument provided!\n");
       else
       ChangeShellPrompt(screen,commands[0].argv[1]); //Get the first argument after the prompt command
    }
    else if(strcmp(tokens[commands[0].first],PWD_COMMAND) == 0)
    {
      char buffer[512];
      if (getcwd(buffer, sizeof(buffer)) == NULL)
        printf("Error while runnung %s\n",PWD_COMMAND);
      else
        printf("%s\n", buffer);       
    }
    else if(strcmp(tokens[commands[0].first],CHANGEDIR_COMMAND) == 0)
    {
       if(commands[0].argv[1] == NULL)      
         printf("No argument provided!\n");
       else
       {
         if(chdir(commands[0].argv[1]) != 0)
         {
           printf("Error while changing directory (argument may be invalid)\n");
         }
       }
         
    }


    return ret;

}
