#include <glob.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "wildcard.h"

char** ExecuteWildcard(char* tokens[], Command* cmd)
{
	int count = 0;
	int wildcardIndex = -1;
	for (int i = cmd->first; i <= cmd->last; i++)
	{
		for (int j = 0; j < strlen(tokens[i]); j++)
		{
 			if (count == 0)
			{
				if (tokens[i][j] == '*' || tokens[i][j] == '?')
				{
					count++;
					wildcardIndex = i;
					break;
				}
			}
			else
			{
				printf("Only one wildcard parameter allowed!\n");
			}
		}
	}

	if (wildcardIndex >= 0)
	{
		glob_t out;
		out.gl_offs = wildcardIndex - cmd->first;
		
		for (int k = cmd->first; k < wildcardIndex; k++)
		{
			out.gl_pathv[k - cmd->first] = tokens[k];
		} 
		glob(tokens[wildcardIndex], GLOB_DOOFFS, NULL, &out);
		execvp(out.gl_pathv[0], &out.gl_pathv[0]);
		globfree(&out);
	}
}
