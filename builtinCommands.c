#include "builtinCommands.h"
#include "executoner.h"


// This method checks for builtin commands. Can probably be put in its own header/c file
// Returns 1 if the command is one of the builtin commands
// Returns 1 if the command is the builtin commands EXIT
// Resutns -1 otherwise
int CheckBuiltinCommand(char* tokens[],Command* commands, Screen* screen)
{ 
	int ret = -1;
	int savedStdout = -1;
	int savedStdin = -1;
	Redirect(tokens, commands, &savedStdout, &savedStdin);
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
	ret = 0;
    }
    else if(strcmp(tokens[commands[0].first],PWD_COMMAND) == 0)
    {
	char buffer[512];
	if (getcwd(buffer, sizeof(buffer)) == NULL)
		printf("Error while runnung %s\n",PWD_COMMAND);
	else
		printf("%s\n", buffer);
      ret = 0;      
    }
    else if(strcmp(tokens[commands[0].first],CHANGEDIR_COMMAND) == 0)
    {
	char* folder = NULL;
	if(commands[0].argv[1] == NULL || strcmp(commands[0].argv[1],"~")==0)      
 	{
		folder = getenv("HOME");        
	}
	else
	{
	  folder = commands[0].argv[1];
	}
	if(chdir(folder) != 0)
 	{
   	  printf("Error while changing directory (argument may be invalid)\n");
 	}
	ret = 0; 
    }

	if (savedStdout >= 0)
		RedirectOutputFD(savedStdout);
	if (savedStdin >= 0)
		RedirectInputFD(savedStdin);

    return ret;

}
