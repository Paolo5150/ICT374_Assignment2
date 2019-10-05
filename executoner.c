#include "executoner.h"

int deadChild = 0;

void ChildHandler(int n, siginfo_t* info, void* idk)
{
 // printf("Got signal %d by %d, parent: %d\n",info->si_signo, info->si_pid,getpid()); 

  deadChild = info->si_pid;

}

int CheckForWait(Command* cmd, int pid)
{
  if(strcmp(cmd->sep,CONSEP) == 0)
    {
      //printf("Waiting for child to die, background, ppid %d\n",getpid());
      waitpid(0,NULL,WNOHANG);

      //printf("Parent returned");
      return 0;
    
    }      
    else if(strcmp(cmd->sep,SEQSEP) == 0)
    {     

      while(1)
      {
        int i = waitpid(0,NULL,0);
        //printf("Waiting for child to die, got %d, deadChild is %d\n",i,deadChild);
        if(pid == deadChild)
          {
            deadChild = 0;
            break;
          } 
        
        sleep(1.0);
      }     
      //printf("Returned here\n");
      return 0;
    }   
}

int ExecutePipedCommand(char* tokens[],Command* pipedCmds, int size)
{

  // Create pipes
  int p[20][2];
  int pipeNum = size - 1;
  int pipeIndex = 0;
  for(int i=0; i< pipeNum ; i++)
  {
    if(pipe(p[i]) < 0)
    {
      printf("Failed to create pipe :(\n");
      exit(0);
    }
  }

  for(int i=0; i< size; i++)
  {
    int pid = fork();
    if(pid < 0)
    {
      printf("failed to fork :(\n");
      exit(0);
    }
    
    if(pid == 0)
    {
      //printf("Spawn child %d, I: %d, size: %d\n", getpid(),i,size);
      // If not last command, set up to write
      if(i == 0)
        {
          //printf("First cmd using pipe %d\n",pipeIndex);
          close(p[pipeIndex][0]);
          dup2(p[pipeIndex][1],STDOUT_FILENO);
          close(p[pipeIndex][1]); 
          
        }       
      // If it's last command, close writing end
      else if(i == size - 1)
      {
        //printf("Lst cmd using pipe %d\n",pipeIndex);
        close(p[pipeIndex][1]);
        dup2(p[pipeIndex][0],STDIN_FILENO);
        close(p[pipeIndex][0]);
        
        // If background process, close stdout to terminal
        if(strcmp(pipedCmds[i].sep,CONSEP) == 0)
        {
        // printf("BG Child %d\n", getpid());
          fclose(stdout);
          fclose(stderr);    
        }  
     
      }
      else
      {
          close(p[pipeIndex][1]);
          dup2(p[pipeIndex][0],STDIN_FILENO);
          close(p[pipeIndex][0]); 

          close(p[pipeIndex+1][0]);
          dup2(p[pipeIndex+1][1],STDOUT_FILENO);
          close(p[pipeIndex+1][1]); 
      }

      ExecuteSingleCommand(tokens,&pipedCmds[i]);
      exit(0);     
    }
    else if(pid > 0)
    { 
      // If first command, wait before proceeding
      if(i == 0)
      {
         waitpid(0,NULL,0);
      }
      // If last command, check for '&' or ';'
      else if(i == size - 1)
      {
        close(p[pipeIndex][0]);
        close(p[pipeIndex][1]);
        CheckForWait(&pipedCmds[i],pid);
      }
      // If "middle" command(s), wait before proceeding
      else 
      {
          close(p[pipeIndex][0]);
          close(p[pipeIndex][1]);
          waitpid(0,NULL,0);
          //printf("Parent, middle CMD\n");
      }      
      
    }  
    if(i > 0 && pipeIndex < pipeNum)
    {
      pipeIndex++;
    }   	 
  }
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

void Redirect(char* tokens[], Command* cmd, int* oldOut, int* oldIn)
{
	for (int i = cmd->first; i <= cmd->last; i++)
	{
		if (strcmp(tokens[i], ">") == 0)
		{
			if (i + 1 == cmd->last)
			{
				*oldOut = RedirectOutput(tokens[i+1]);
				break;
			}
			else
			{
				printf("Output Redirection requires single argument!\n");
				break;
			}
		}
		else
		{
			if (strcmp(tokens[i], "<") == 0)
			{
				if (i + 1 == cmd->last)
				{
					*oldIn = RedirectInput(tokens[i+1]);
					break;
				}
				else
				{
					printf("Input Redirection requires single argument!\n");
					break;
				}
			}
		}
		
	}
}

int RedirectInput(char* inputFilename)
{
	int inFileDesc = open(inputFilename, O_RDONLY | O_CREAT, 0666);
	int origFD = dup(STDIN_FILENO);
	dup2(inFileDesc, STDIN_FILENO);
	return origFD;
}

void RedirectInputFD(int fd)
{
	dup2(fd, STDIN_FILENO);
}


int RedirectOutput(char* outputFilename)
{
	int outFileDesc = open(outputFilename, O_WRONLY | O_CREAT, 0666);
	int origFD = dup(STDOUT_FILENO);
	dup2(outFileDesc, STDOUT_FILENO);
	return origFD;
}

void RedirectOutputFD(int fd)
{
	dup2(fd, STDOUT_FILENO);
}

int ExecuteSingleCommand(char* tokens[],Command* cmd)
{

    char** newArgs = IsPath(tokens[cmd->first] ,cmd->argv,cmd->argc);

	//Redirects output/input if necessary
	int out = -1;
	int in = -1;
	Redirect(tokens, cmd, &out, &in);
	
  if(newArgs != NULL)
  {    
	
    execv (tokens[cmd->first], newArgs);
    printf("Command not recognized :(\n");
    exit(0);

  } 
  else
  {
    execvp(tokens[cmd->first] ,cmd->argv);
    printf("Command not recognized :(\n");
    exit(0);
  }
  return 0;
    
}




int ExecuteProcessedSingleCommand(char* tokens[],Command* cmd)
{
  //printf("executing %s, pid %d\n", tokens[cmd->first],getpid());
  int pid;


 if ((pid=fork()) < 0)
  {
    printf("Error while forking in pipe execution\n");
    return -1;
  }
  
  // Parent
  if (pid > 0)
  {
    CheckForWait(cmd,pid);   
    
  }
  // Child
  else if(pid == 0)
  {
    if(strcmp(cmd->sep,CONSEP) == 0)
    {
     // printf("BG Child %d\n", getpid());
      fclose(stdout);
      fclose(stderr);
      fclose(stdin);
          
    }
    ExecuteSingleCommand(tokens,cmd);      
  } 

}


















