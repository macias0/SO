#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>


int breakLoop = 0;
int ctrZOccurs = 0;

void signalHandler(int signum)
{
	if(signum == SIGINT)
	{
		//printf("SIGINT process %d\n", getpid());
		breakLoop = 1;
	}
	if(signum == SIGKILL)
		printf("SIGKLL\n");
	if(signum == SIGTSTP)
		ctrZOccurs = 1;
		//printf("SIGTSTP\n");
}



int main(int argc, char **argv)
{
	//printf("uruchamiam process %d\n", getpid());
	//intercept signals
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = signalHandler;
	sigaction(SIGINT, &act, NULL);
	//sigaction(SIGKILL, &act, NULL);
	//sigaction(SIGTSTP, &act, NULL);

	//block signal
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGTSTP);
	sigprocmask(SIG_BLOCK, &mask, NULL);


	int stringLength = strlen(argv[argc-1]);
	while( stringLength & (stringLength -1 ) && stringLength != 0 ) // if not power of 2
	{
		argv[argc-1][stringLength-1]='\0'; //remove 1 char
		stringLength = strlen(argv[argc-1]);
	}
	
	int childPids[2] = {-1, -1};
	if( stringLength > 1 )
	{

		for(int i=0; i<2; i++)
		{
			//printf("for loop\n");
			if( (childPids[i] = fork() ) == 0 ) //child process
			{
				setpgid(getpid(), getpid()); //set process group to pid (unique)

				char **newArgv = malloc( (argc+2) * sizeof(char*) ); //one for new param, another for NULL
				for(int i=0; i<argc; i++)
					newArgv[i] = argv[i];
				newArgv[argc+1] = NULL;
				newArgv[argc] = malloc( stringLength / 2 + 1 );
				strncpy(newArgv[argc], argv[argc-1] + i * stringLength / 2, stringLength / 2);
				newArgv[argc][stringLength/2] = '\0'; //terminated by null
				execv(argv[0], newArgv);
				printf("Po exec");
				free(newArgv); //is it neccesary?
			}
		}

	}
	while( !breakLoop ); //infinite loop

	//pass signal to children processes
	for(int i=0; i<2; ++i)
		if(childPids[i] != -1)
			kill(childPids[i], SIGINT);

	int status = 0;
	while( wait(&status) > 0); //wait for childrens to exit

	printf("%d", getpid());
	for(int i=1; i<argc; i++)
		printf(" %s", argv[i]);
	if(ctrZOccurs)
		printf(" Pojawil sie sygnal CTRL-Z");
	printf("\n");
	sigprocmask(SIG_UNBLOCK, &mask, NULL);
	return 0;
}
