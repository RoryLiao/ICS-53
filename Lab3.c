/*
	JT
	TC
	Liao, Rory
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdbool.h>
void look_heap(int start, char* heap);
int main(int argc, char* argv[])
{

	char *HEAP = malloc(sizeof(char) * 400);
	char *ptrs[256];

	int i;
	// for(i = 0; HEAP[i] != '\0'; i++) {
		
 //    		printf("%c", word[i]);

	// }
	int blockCounter = 1;
	while(1)
	{
		printf("> ");
		char input[81];

   		//printf( "Enter a command :\n");
		fgets(input, 81, stdin);
		//printf("input is %s\n", input);
		
		//remove \n at end of input stream
		char *pos;
		if ((pos=strchr(input, '\n')) != NULL)
    			*pos = '\0';

		//printf( "Reading tokens :\n");
		char *arg = strtok(input, " \t");
		//char cmd[81];
		//strcpy(cmd, arg);
		
		char* args[4] = {NULL};
		args[0] = arg;

		int counter = 1;
		int bg = 0;
		while(arg != NULL) {
			//printf("arg: %s\n", arg);
			arg =  strtok(NULL, " \t");
			args[counter] = arg;
			counter++;
		}

		if(args[0] == NULL)
			continue;

		int arraycounter = 0;
		if(strcmp(args[0], "allocate") == 0) 
		{
			int i;
			for(i = 0; i < 400; i=i + atoi(HEAP[i+1]) + 3) 
			{
				if(HEAP[i] == '\0')
				{
					ptrs[blockCounter-1] = HEAP[i]; //Allocates asked for space
					HEAP[i]=blockCounter;
					HEAP[i+1] = args[1];
					HEAP[i+2] = 1;
					break;

				}
				if(atoi(HEAP[i+2]) == 0 && (atoi(HEAP[i+1]) <= atoi(args[1])))     //if space is already allocated, go to next header
				{
					ptrs[blockCounter-1] = HEAP[i]; //Allocates asked for space
					HEAP[i]=blockCounter;
					HEAP[i+1] = args[1];
					HEAP[i+2] = 1;
					break;
				}
			}
			printf("%i\n", blockCounter++);
		}
	
		else if(strcmp(args[0], "free") == 0)
		{
			int i;
			// for(i = 0; HEAP[i] != '\0'; i++) {
   //  				printf("%c", word[i]);
			// }
		}
		else if(strcmp(args[0], "blocklist"))
		{
			char* allocStr;
			int size;
			printf("Size\t Allocated\t Start\t End\n");
			int i;
			for(i = 0; i < blockCounter-1; i++)
			{
				allocStr = "no";
				size = atoi(*(ptrs[i]+1));
				if(atoi(*(ptrs[i]+2)))
					allocStr = "yes";
				if(HEAP[i] != '\0')
				{	
					printf("%s\t %s\t %s\t %s\n", size, allocStr, ptrs[i],*(ptrs[i]+size));
				}
			}
		}
		else if(strcmp(args[0], "quit"))
		{
			free(HEAP);
			break;
		}

	}
	return 1;
}


// void look_heap(int start, char* heap)
// {
// 	int i;
// 	for (i = start; temp < 400; < )
// }
