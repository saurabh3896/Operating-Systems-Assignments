#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
int main(int argc,char *argv[])
{
	int fd;
	struct stat st;
	int size,stsize;
	long count=0L;
        if(argc!=2)
        {
                printf("Incorrect usage:\nUsage:./prog.out <filename>");
		return 0;
        }
	fd=open(argv[1],O_RDONLY);
	if((size=lseek(fd,count,SEEK_END))==0)
	{
		printf("0000\n ");
	}
	printf("Size of file measured using lseek: %d\n",size);
	if (stat(argv[1],&st)!=0)
		stsize=0;
	else
		stsize=st.st_size;
	printf("Size of file measured using stat: %zd",stsize);
}
