/*
	JT
	TC
	Liao, Rory
*/

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char* argv[])
{
	while(1)
	{
		printf("prompt> ");
		char input[80];

   		//printf( "Enter a command :\n");
		fgets(input, 80, stdin);
		//printf("input is %s\n", input);
		
		//remove \n at end of input stream
		char *pos;
		if ((pos=strchr(input, '\n')) != NULL)
    			*pos = '\0';

		
		//printf( "Reading tokens :\n");
		char *arg = strtok(input, " \t");
		char cmd[80];
		strcpy(cmd, arg);
		
		char* args[10];
		int counter = 0;
		int bg = 0;
		while(arg != NULL) {
			//printf("arg: %s\n", arg);
			arg =  strtok(NULL, " \t");
			args[counter] = arg;
			counter++;
		}
		int i;
		for(i = 0; i < counter-1; i++)
			//printf("%s\n", args[i]);
		if(strcmp(cmd, "exit") == 0)
		{
			printf("exit\n");
			break;
		}
		 
	
		pid_t pid;

		//printf("last part: %s\n", args[counter-2]);

		if (counter > 1)
		{
			printf("last: %s", args[counter-2]);
			if (strcmp(args[counter-2], "&") == 0)
			{
				bg = 1;
			}

		}

		int child_status;
		if((pid = fork()) == 0) 
		{
			printf("HC: hello from child\n");
			    if (execve(cmd, args, environ) < 0) { 
        				printf("%s: Command not found.\n", cmd);
    					waitpid(pid, &child_status, WNOHANG);	
    					printf("About to exit\n");
        				exit(0); 
    				} 
    		
    		
		}
		if (!bg)
		{
			printf("HP: hello from parent\n");
			if (waitpid(pid, &child_status, 0)  < 0)
				printf("waitfg: waitpid error\n");
			printf("CT: child has terminated\n");

		}
		else
			printf("%d %s", pid, cmd);

		if (WIFEXITED(child_status))
			printf("we gucci\n");
		printf("Bye\n");
	}
	return 1;
}
