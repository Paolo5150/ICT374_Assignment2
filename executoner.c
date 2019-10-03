#include "executoner.h"

int ExecutePipedCommand(char* tokens[],Command* leftCmd, Command* rightCmd)
{

  int p[2], pid, pid2;

  if (pipe(p) < 0)
  {
    printf("Error while creating pipe\n");
    return -1;
  }

  if ((pid=fork()) < 0)
  {
    printf("Error while forking in pipe execution\n");
    return -1;
  }
  
   if (pid > 0)
  {
    if ((pid2=fork()) < 0)
    {
    printf("Error while forking in pipe execution\n");
    return -1;
    }

    if (pid2 == 0)  
    {
      // Read only
      close(p[1]);
      dup2(p[0],STDIN_FILENO);
      close(p[0]);  
      //printf("\nPID %d, executing exec2, first %s, arg %s\n",getpid(),tokens[rightCmd->first],rightCmd->argv[0]);
      ExecuteSingleCommand(tokens,rightCmd);
      printf("\nCould not execute command 2..\n"); 
      exit(0); 

    }
    else
    {
      close(p[0]);
      close(p[1]);

      waitpid(0,NULL,0);
      waitpid(0,NULL,0);
      return 0;
    }

    
  }

  if (pid == 0)  
  {
    // printf("\nPID %d, executing exec1, first %s, arg %s\n",getpid(),tokens[leftCmd->first],leftCmd->argv[1]);
    // Write only
    close(p[0]);
    dup2(p[1],STDOUT_FILENO);
    close(p[1]);   
 
    ExecuteSingleCommand(tokens,leftCmd);
    printf("\nCould not execute command 1..\n"); 
     exit(0);
  }
  return 0;
}

int IsPath(char* line, char** args,int argc, char** newArgs)
{
  char cpy[100];
  strncpy(cpy,line,strlen(line)+1);
  char* tks[10];
  int t = tokenise(cpy,tks,"/");

  if(t == 2)
    return 0;
  else
  {
 
    return 1;
  }



}
int ExecuteSingleCommand(char* tokens[],Command* cmd)
{
  char* newArgs[100];
  if(IsPath(tokens[cmd->first] ,cmd->argv,cmd->argc,newArgs))
  {
    printf("Execv %s args: %s ",tokens[cmd->first],cmd->argv[0]);
  //  execv (tokens[cmd->first], cmd->argv);
  }
  else
  {
   // execvp(tokens[cmd->first] ,cmd->argv);
  }
    
}

int ExecuteProcessedSingleCommand(char* tokens[],Command* cmd)
{
   char** newArgs = NULL;
if(IsPath(tokens[cmd->first] ,cmd->argv,cmd->argc,newArgs))
  {
    char cpy[100];
    strncpy(cpy,tokens[cmd->first],strlen(tokens[cmd->first])+1);
    char* tks[10];
    int t = tokenise(cpy,tks,"/");
    newArgs = (char **) realloc(newArgs, sizeof(char *) * cmd->argc);
    newArgs[0] = (char *) realloc(newArgs[0], sizeof(char) * strlen(tks[t-2]));
    strcpy(newArgs[0],tks[t-2]);
    for(int i=1; i< cmd->argc; i++)
    {
      newArgs[i] = (char *) realloc(newArgs[i], sizeof(char) * strlen(cmd->argv[i]));
      strcpy(newArgs[i],cmd->argv[i]);
    }
    execv (tokens[cmd->first], newArgs);
    printf("Failed!");

  }
  //Spawn process
  // If command has '&', remove stdout to screen, parent won't wait
  // If command has '>' or '<', change process stdout or stdin

}


















