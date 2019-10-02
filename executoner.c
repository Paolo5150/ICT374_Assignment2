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
      execlp(tokens[rightCmd->first],tokens[rightCmd->first] ,rightCmd->argv[1],(char *)0);
      printf("\nCould not execute command 2..\n"); 
        close(p[0]);
      close(p[1]);
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
 
    execlp(tokens[leftCmd->first],tokens[leftCmd->first] ,leftCmd->argv[1],(char *)0);
    printf("\nCould not execute command 1..\n"); 
     exit(0);
  }
  return 0;
}
