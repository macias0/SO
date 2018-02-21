#include <stdio.h>
#include <utmpx.h>
#include <unistd.h>
#include <stdbool.h>
#include <dlfcn.h>


void (*getUserId)(char *);
void (*getUserGroups)(char *);

int main(int argc, char **argv)
{
    int opt;
    bool showId = false, showGroups = false;
    while( (opt = getopt(argc, argv, "ig") ) != -1 )
    {
         switch(opt)
		 {
		 case 'i':
			 showId = true;
			 break;
		     case 'g':
			 showGroups = true;
		         break;
		 default:
		         break;
		 }
    }

	void *libHandle = NULL;
	if(showId || showGroups)
	{
		libHandle = dlopen("./lab2lib.so", RTLD_NOW | RTLD_GLOBAL);
		if(!libHandle)
		{	
			fputs(dlerror(), stderr);
			showId = showGroups = false;
			//return 1;
		}
		getUserId = dlsym(libHandle, "getUserId");
		if(!getUserId)
		{
			fputs(dlerror(), stderr);
			showId = false;
			//return 1;
		}
		getUserGroups = dlsym(libHandle, "getUserGroups");
		if(!getUserGroups)
		{
			fputs(dlerror(), stderr);
			showGroups = false;
			//return 1;
		}
	}

    struct utmpx *user;
    while( (user = getutxent()) )
    {
		if(user->ut_type == USER_PROCESS)
		{

			
			//user->ut_user
			
		    if(showId)
		        getUserId(user->ut_user);
		    printf("%s", user->ut_user);
		    if(showGroups)
				getUserGroups(user->ut_user);
		    printf("\n");
		        
		}
    }

	if(libHandle != NULL)
		dlclose(libHandle);
    endutxent();
    return 0;
    
}
