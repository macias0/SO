#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	//dodac dopelnienie do potegi 2
	int stringLength = strlen(argv[argc-1]);
	while( stringLength & (stringLength -1 ) && stringLength != 0 ) // if not power of 2
	{
		argv[argc-1][stringLength-1]='\0'; //remove 1 char
		stringLength = strlen(argv[argc-1]);
	}
	if( stringLength > 1 )
	{

		for(int i=0; i<2; i++)
		{
			if( fork() == 0 ) //child process
			{
				//for(int i=0; i<argc; i++)
				//	printf("argv[%d]=%s\n", i, argv[i]);
				char **newArgv = malloc( (argc+2) * sizeof(char*) ); //one for new param, another for NULL
				for(int i=0; i<argc; i++)
					newArgv[i] = argv[i];
				newArgv[argc+1] = NULL;
				newArgv[argc] = malloc( stringLength / 2 + 1 );
				strncpy(newArgv[argc], argv[argc-1] + i * stringLength / 2, stringLength / 2);
				newArgv[argc][stringLength/2] = '\0'; //terminated by null
				//for(int i=0; i<argc+2; i++)
				//	printf("newArgv[%d]=%s\n", i, newArgv[i]);
				execv(argv[0], newArgv);
				printf("Po exec");
				free(newArgv); //is it neccesary?
			}
		}
		//still parent
		int status = 0;
		while( wait(&status) > 0);
		//parent after all child process ended
	}
	printf("%d", getpid());
	for(int i=1; i<argc; i++)
		printf(" %s", argv[i]);
	printf("\n");
	return 0;
}
