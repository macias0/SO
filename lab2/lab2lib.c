#include "lab2lib.h"
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>

void getUserId(char *a_userName)
{
	struct passwd *userPasswd= getpwnam(a_userName);
	if(userPasswd)
		printf("%d ", userPasswd->pw_uid);
}
void getUserGroups(char *a_userName)
{
	struct passwd *userPasswd= getpwnam(a_userName);
	if(userPasswd)
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
}
