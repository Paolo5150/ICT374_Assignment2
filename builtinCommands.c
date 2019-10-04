#include "builtinCommands.h"
#include "executoner.h"


// This method checks for builtin commands. Can probably be put in its own header/c file
// Returns 1 if the command is one of the builtin commands
// Returns 1 if the command is the builtin commands EXIT
// Resutns -1 otherwise
int CheckBuiltinCommand(char* tokens[],Command* commands, Screen* screen)
{ 
	int ret = -1;
	pid_t pid = fork();
	if (pid == 0)
		Redirect(tokens, commands);
    if(strcmp(tokens[commands[0].first],EXIT_COMMAND) == 0)
    {
	if (pid == 0)
        	printf("\nBye!\n");
	else
	        ret = 1;
    }
    else if(strcmp(tokens[commands[0].first],PROMPT_COMMAND) == 0)
    {
	if (pid ==0)
	{
       		if(commands[0].argv[1] == NULL)      
         		printf("No argument provided!\n");
       		else
       			ChangeShellPrompt(screen,commands[0].argv[1]); //Get the first argument after the prompt command
	}
       	else
       		ret = 0;
    }
    else if(strcmp(tokens[commands[0].first],PWD_COMMAND) == 0)
    {
	if (pid ==0)
	{
       		char buffer[512];
      		if (getcwd(buffer, sizeof(buffer)) == NULL)
        		printf("Error while runnung %s\n",PWD_COMMAND);
      		else
        		printf("%s\n", buffer);
	}
	else
	      ret = 0;      
    }
    else if(strcmp(tokens[commands[0].first],CHANGEDIR_COMMAND) == 0)
    {
	if (pid == 0)
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
	else
       		ret = 0; 
    }
    int status;
    if (pid == 0)
	exit(0);
    else
{
	waitpid(pid, &status, WEXITED);
	sleep(1); // To allow for printf output to finish
}



    return ret;

}
