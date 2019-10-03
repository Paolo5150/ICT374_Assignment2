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

char** IsPath(char* line, char** args,int argc)
{
  char cpy[100];
  strncpy(cpy,line,strlen(line)+1);
  char* tks[10];
  int t = tokenise(cpy,tks,"/");

  if(t == 2)
    return NULL;
  else
  {
    // This mess below creates a new set of arguments, copied from the command arguments, except that the first argument is the command with no /
    strncpy(cpy,line,strlen(line)+1);
    int t = tokenise(cpy,tks,"/");
    char** newArgs = NULL;
    newArgs = (char **) realloc(newArgs, sizeof(char *) * argc);
    newArgs[0] = (char *) realloc(newArgs[0], sizeof(char) * strlen(tks[t-2]));
    strcpy(newArgs[0],tks[t-2]);
    int i = 1;
    for(; i< argc; i++)
    {
      newArgs[i] = (char *) realloc(newArgs[i], sizeof(char) * strlen(args[i]));
      strcpy(newArgs[i],args[i]);
    }
    newArgs[i] = NULL;
 
    return newArgs;
  }



}
int ExecuteSingleCommand(char* tokens[],Command* cmd)
{

    char** newArgs = IsPath(tokens[cmd->first] ,cmd->argv,cmd->argc);
  
  if(newArgs != NULL)
  {    
    execv (tokens[cmd->first], newArgs);
   // printf("Failed!");

  } 
  else
  {
    execvp(tokens[cmd->first] ,cmd->argv);
    printf("Failed!");
  }
  return 0;
    
}

int ExecuteProcessedSingleCommand(char* tokens[],Command* cmd)
{

  //Spawn process
  // If command has '&', remove stdout to screen, parent won't wait
  // If command has '>' or '<', change process stdout or stdin
 if ((pid=fork()) < 0)
  {
    printf("Error while forking in pipe execution\n");
    return -1;
  }
  
  // Parent
  if (pid > 0)
  {
    //if '&', don't wait for child process
    //if ';' wait and then return for next command
  }
  // Child
  else if(pid == 0)
  {
    // Check if cmd->separator is '&'
       // If it is, detached stdout (no print to screen) - I'll get up to this point
       // However, if here is a '>', stdout shout go to file (filename should be argument index 1) so exec will output there
       // Also check for '<', which means read from that file

   // If separator is ';', i guess execute as normal (and of course check for '<' '>')

   
  }
  

}


















