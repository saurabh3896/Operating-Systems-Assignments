#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>
void deletechilds(char dirname[])
{
	int temp;
	DIR* dir=opendir(dirname);
	if(dir==NULL)
		return;
	printf("-Removing the directory %s",dirname);
	char filepath[1000];
	struct dirent *direntry;
	while((direntry=readdir(dir))!=0)
	{
		if(strcmp(direntry->d_name,".")==0||strcmp(direntry->d_name,"..")==0)
			continue;
		strcpy(filepath,dirname);
		strcat(filepath,"/");
		strcat(filepath,direntry->d_name);
		
		temp=remove(filepath);
		deletechilds(filepath);	
	}
	remove(dirname);
	return;
}


int main(int argc, char *argv[])
{
	DIR* dir=opendir(argv[1]);
	///dir not 0
	if(dir==NULL)
	{
		printf("Not a directory");
		return 0;	
	}
	deletechilds(argv[1]);
	return 0;
}
