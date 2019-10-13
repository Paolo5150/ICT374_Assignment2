#include <glob.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "wildcard.h"

char** GetWildcardCommands(char** args, int argc, int* returnSize)
{
	int foundWildcard = -1;
	int lastWildcardIndex = -1;
	int newArgsIndex = 0;
	int newArgsSize = 0;
	char** newArgs = NULL;
	
	//Loop through all arguments
	for (int i = 0; i < argc; i++)
	{
		//Loop through all characters in arguments
		for (int j = 0; j < strlen(args[i]); j++)
		{
			//Check if they match the wildcard characters
			if (args[i][j] == '*' || args[i][j] == '?')
			{
				//Glob accepts '*'
				if (args[i][j] == '?')
					args[i][j] = '*';
				foundWildcard = 1;
				newArgsSize += (i - (lastWildcardIndex + 1));
				newArgs = NULL;
				newArgs = (char**) realloc(newArgs, sizeof(char *) * newArgsSize);
				//Add any arguments since the last wildcard
				for (int k = newArgsIndex, l = lastWildcardIndex + 1; k < newArgsSize; k++, l++)
				{
					newArgs[k] = args[l];
				}
				
				newArgsIndex = newArgsSize;

				glob_t variants;
				int globReturn = glob(args[i], 0, NULL, &variants);
				if (globReturn == GLOB_ABORTED)
					printf("GLOB ABORTED\n");
				if (globReturn == GLOB_NOMATCH)
					printf("GLOB NOMATCH\n");
				if (globReturn == GLOB_NOSPACE)
					printf("GLOB NOSPACE\n");
				if (globReturn != GLOB_ABORTED | GLOB_NOMATCH | GLOB_NOSPACE)
				{
					newArgsSize += variants.gl_pathc; 
					newArgs = (char**) realloc(newArgs, sizeof(char *) * newArgsSize);
					//Add the path arguments found by glob to the return arguments
					for (int k = newArgsIndex; k < newArgsIndex + variants.gl_pathc; k++)
					{
						newArgs[k] = NULL;
						newArgs[k] = (char*) realloc(newArgs[k], sizeof(char) * strlen(variants.gl_pathv[k - newArgsIndex])+1);
						strcpy(newArgs[k], variants.gl_pathv[k - newArgsIndex]);
					}
					
					globfree(&variants);
					
					newArgsIndex = newArgsSize;
				}
				lastWildcardIndex = i;
				break;
			}
		}
	}

	if (foundWildcard)
	{
		//Add remaining command parameters to new arguments
		if (lastWildcardIndex != argc - 1)
		{
			newArgsSize += (argc - 1) - lastWildcardIndex;
			newArgs = (char**) realloc(newArgs, sizeof(char *) * newArgsSize);

			for (int i = lastWildcardIndex + 1, j = newArgsIndex; i < argc; i++, j++)
			{
				newArgs[j] = args[i];
			}
			newArgsIndex = newArgsSize;
		}

		//Add an additional NULL argument so that the end of the array can be identified
		newArgsSize += 1;
		newArgs = (char**) realloc(newArgs, sizeof(char *) * newArgsSize);
		newArgs[newArgsIndex] = NULL;
	}

	*returnSize = newArgsSize;
	 
	return newArgs;
}
