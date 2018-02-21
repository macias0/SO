#include <stdio.h>
#include <utmpx.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

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

    struct utmpx *user;
    while( (user = getutxent()) )
    {
	if(user->ut_type == USER_PROCESS)
	{

    	    struct passwd *userPasswd= getpwnam(user->ut_user);
            if(userPasswd != NULL)
            {
                if(showId)
                    printf("%d ", userPasswd->pw_uid);
                printf("%s", userPasswd->pw_name);
                if(showGroups)
                {
                    int groupSize = 0;
                    if( getgrouplist(userPasswd->pw_name, userPasswd->pw_gid, NULL, &groupSize) == -1)
                    {
                        gid_t *groups = malloc(groupSize);
                        if(groupSize != 0 && getgrouplist(userPasswd->pw_name, userPasswd->pw_gid, groups, &groupSize) == groupSize )
                        {
                            printf(" [");
                            for(int i=0; i<groupSize; i++)
                            {
                                    struct group *grp = getgrgid(groups[i]);
                                    if(grp)
                                    {
                                        if(i!=0)
                                            printf(", ");
                                        printf("%s", grp->gr_name);
                                    }
                            }
                            printf("]");
                        }
                        free(groups);
                    }
                }
                printf("\n");
            }
	}
    }
    endutxent();
    return 0;
    
}
