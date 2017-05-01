#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
char readst[10];
int fd_in,fd_out,lines;
//Function to count the total number of lines
int totallines(int fd_in)
{
	printf("in fun lines");
	int n, flines=0;
	while((n=read(fd_in,readst,1))>0)
	{ 
	//std::cout<<"\n"<<n<<"\t"<<int(n)<<"\n";
	  // printf("read %d\n",n);
		n=readst[0];
	    //  printf("new n %d\n",n);
	
	if(n==10)
	{
		flines++;
		printf("\nnewline");
	}
	}
	printf("\nlines- %d",flines);
	return flines;
}
//Function to find the position of first charecter of a line
int movetoline(int fd_in,int lno)
{
	printf("\nin fun move");
	int l=0,n;
	int pos=0;
	while((n=read(fd_in,readst,1))>0)
{
//	std::cout<<n<<"\t"<<readst<<"\n";
		pos++;
		if (l==lno-1)
		{
			printf("\nAt position : %d",pos);
			return pos;
	
	}
		if(readst[0]==10)
			l++;
	}
	return pos;
}

int main(int argc,char *argv[])
{
	char buff[2];
	ssize_t size_in,size_out;
	off_t seekret,pos;
	if(argc!=3)
	{
		printf("Incorrect usage:\nUsage:./progobj <file1> <file2>");
		return 0;
	}
	//Getting the file descriptors
		//input file in read only mode
	fd_in=open(argv[1],O_RDONLY);
	if(fd_in==-1)
	{
		printf("Failed to load input file!");
		return 0;
	}
		//output file is opened in writeonly mode and if doesnt exists, the file will be created.Care is taken that the owner of the file has all permissions.
	fd_out=open(argv[2],O_WRONLY|O_CREAT,0700);
	if(fd_in==-1)
	{
		printf("Failed to load input file!");
		return 0;
	}
	seekret=lseek(fd_in,0,SEEK_SET);
	lines=totallines(fd_in);
//	printf("came");
	seekret=lseek(fd_in,0,SEEK_SET);
	while (lines)
	{
		int x=movetoline(fd_in,lines);
	//	std::cout<<"\nx:"<<x;
	//	std::cout<<"\nline:"<<lines;
		
		seekret=lseek(fd_in,0,SEEK_SET);
		seekret=lseek(fd_in,x-1,SEEK_SET);
		while(1)
		{
			size_in=read(fd_in,&buff,1);
			size_out=write(fd_out,&buff,1);
			//do check
			//printf("buff %c",buff[0]);
			if(buff[0]==10)
			{
				//printf("buff=10");
				break;
			}
		}
		seekret=lseek(fd_in,0,SEEK_SET);
		lines--;	
	}	

	
	printf("Completed reverse copying from %s to %s .",argv[1],argv[2]);
	close(fd_in);
	close(fd_out);

}
