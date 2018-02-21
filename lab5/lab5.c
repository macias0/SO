#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/prctl.h> //for PR_SET_NAME
#include <string.h>
#include <stdlib.h> //for atoi
#include <signal.h> //for kill




int main(int argc, char **argv)
{
	int killall = 0;
	if( strcmp(argv[0], "./killall") == 0)
		killall = 1; 
	if( argc < 2 )
	{
		printf("Nie podano argumentu z nazwa procesu\n");
		return 1;
	}
	DIR *dirp = opendir("/proc");
	if(dirp)
	{
		char commFilePath[PATH_MAX];
		struct dirent * dirStruct ;
		while( ( dirStruct = readdir(dirp) ) != NULL)
		{
			strcpy(commFilePath, "/proc/");
			strcat(commFilePath, dirStruct->d_name);
			strcat(commFilePath, "/comm");
			//printf("Otiweram plik: %s\n", commFilePath);
			int fileDescriptor = open(commFilePath, O_RDONLY);
			if(fileDescriptor)
			{
				char name[PR_SET_NAME+1];
				int bytesRead;
				if( ( bytesRead = read(fileDescriptor, name, PR_SET_NAME) ) != -1 )
				{
					//add null byte at the end of buffer (replace newline)
					name[bytesRead-1] = '\0';
					//printf("pid: %s %s bytesRead: %d\n", dirStruct->d_name, name, bytesRead);
					//for(int i=0; i<bytesRead; ++i)
						//printf("Bajt nr %d=%d\n", i, name[i]);
					//printf("strlen of name: %lu\n", strlen(name));
					//printf("strlen of argv: %lu\n", strlen(argv[1]));
					if(killall)
						kill(atoi(dirStruct->d_name), SIGINT);
					else
						if( !strcmp(name, argv[1]) )
							printf("%s\n",  dirStruct->d_name);
				}
				close(fileDescriptor);
			}
			//printf("%s\n", dirStruct->d_name); 
		}

		closedir(dirp);
	}
	return 0;
}
