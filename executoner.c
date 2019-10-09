#include "executoner.h"
#include "wildcard.h"

static int deadChild = 0;

void ChildHandler(int n, siginfo_t* info, void* idk)
{
  deadChild = info->si_pid;
  // Avoid zombie apocalypse!
  while(waitpid(-1,NULL,WNOHANG)>0)
  {};

}

int CheckForWait(Command* cmd, int pid)
{
  if(strcmp(cmd->sep,CONSEP) == 0)
    {
      // If '&', returns immmediately. Pricess will be claimed in ChildHandler      
      return 0;
    
    }      
    else if(strcmp(cmd->sep,SEQSEP) == 0)
    {     

      while(1)
      {
	//printf("Parent waiting..\n");
        int i = waitpid(-1,NULL,0);
 
        if(i == deadChild)
          {
            deadChild = 0;
            break;
          } 
        
        sleep(0.1);
      }     
      //printf("Returned here\n");
      return 0;
    }   
}

int ExecutePipedCommand(char* tokens[],Command* pipedCmds, int size)
{

  // Create pipes
  int p[MAX_PIPES][2];
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
      
      if(i == 0)
      {
         
      }
      // If last command, check for '&' or ';'
      else if(i == size - 1)
      {
        close(p[pipeIndex][0]);
        close(p[pipeIndex][1]);
        CheckForWait(&pipedCmds[i],pid);        
      }     
      else 
      {
          close(p[pipeIndex][0]);
          close(p[pipeIndex][1]);
      }      
      
    }  
    if(i > 0 && pipeIndex < pipeNum)
    {
      pipeIndex++;
    }   	 
  }
}

int IsPath(char* line, char** tks)
{
  
  int t = tokenise(line,tks,"/");

  if(t == 2)
    return 0;
  else
  {
    return t;
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
	int outFileDesc = open(outputFilename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
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
  // Save original command to when tokenizing, it won't be compromised
  char orig[100];
  strcpy(orig,tokens[cmd->first]);

  // Check if there are tokens (if input is a path eg. /bin/ls)
  char* tks[10];
  int p = IsPath(tokens[cmd->first] ,tks);

  //Redirects output/input if necessary
  int out = -1;
  int in = -1;
  Redirect(tokens, cmd, &out, &in);
	
  if(p != 0)
  {    
	tokens[cmd->first] = orig; 		// Command set to original value (eg. "/bin/ls")
	strcpy(cmd->argv[0],tks[p-2]);	// First Argument set to tokenized value (eg. "ls")
	int arrSize;
	char** afterWildcard = GetWildcardCommands(cmd->argv, cmd->argc, &arrSize);
	if (afterWildcard != NULL)
	{
		execv (tokens[cmd->first], afterWildcard);
	}
	else
	{
		execv (tokens[cmd->first], cmd->argv);
	}
	printf("Command not recognized :(\n");
	exit(0);
  } 
  else
  {
	int arrSize;
	char** afterWildcard = GetWildcardCommands(cmd->argv, cmd->argc, &arrSize);
	if (afterWildcard != NULL)
	{
		execvp(tokens[cmd->first] ,afterWildcard);
	}
	else
	{
		execvp(tokens[cmd->first] ,cmd->argv);
	}
    printf("Command not recognized :(\n");
    exit(0);
  }
  return 0;
    
}


int ExecuteProcessedSingleCommand(char* tokens[],Command* cmd)
{
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
    ExecuteSingleCommand(tokens,cmd);      
  } 

}

