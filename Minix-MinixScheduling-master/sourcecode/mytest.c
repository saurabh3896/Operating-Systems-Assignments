#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
//Here , we are starting the IO and CPU bound processes in different ratios to check the time variations
//longrun1 is a cpu bound and longrun2 is an io bound process
//Based on i%10 , we start either a CPU or IO bound process.
int main()
{
	int pid[15];
	int i;
	char processid[50];
  
    for(i=1;i<=10;i++)
    {
    	pid[i]=fork(); //fork 10 processes
    	if(pid[i]==0)
    	{
    		sprintf(processid,"%2d",i);
    		if(i%10<3)
                execlp("./longrun0","./longrun0",processid,"100000","1000",NULL); //create a CPU bound process 
    	    else
                execlp("./longrun1","./longrun1",processid,"100000","1000",NULL); //create an IO bound process
        }
    }
    for(i=1;i<=10;i++) //wait for everything to terminate 
    {
    	wait(NULL);
    }	
    return 0;
}

/* 
 * This program does not set priorites of the different processes. This can be set through the minix system call SYS_NICE.
 * Its signature: int sys_nice(endpoint_t proc_nr, int priority); 
 * More details can be found here: http://wiki.minix3.org/doku.php?id=developersguide:kernelapi#sys_nice
 */


