#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

struct info{					//struct for keeping file parameters
  int a, b;
  float c, d;
};

int main(){
  int n, loopCount, id, deadline;		//variable declaration
  double arrival_time, deadline__;		//variables to measure time and deadline
  FILE *fp;					//file ptr
  char *processid = (char *)malloc(256*sizeof(char)), *loopcount = (char *)malloc(256*sizeof(char)), *deadline_ = (char *)malloc(256*sizeof(char));
  fp = fopen("inp-params0.txt", "r");		//open the input file in read mode
  int count = 0;
  fscanf(fp, "%d\n", &n);			//scan the value of n, the number of processes
  struct info *array = (struct info *)malloc(n*sizeof(struct info));
  //scan the file line by line, and store arrival time, loopcount, deadline and processid of each process
  while(fscanf(fp, "%d %lf %lf %d\n", &id, &arrival_time, &deadline__, &loopCount) == 4){
    //store the parameters in an array of structs
    array[count].a = id; array[count].b = loopCount;
    array[count].c = arrival_time; array[count].d = deadline__;
    count++;
  }
  for(int i = 0;i < n;i++){			//fork n processes of type longrun.c
    pid_t pid;
    if((pid = fork()) == 0){			//child process
      sprintf(processid, "%d", i + 1);		//conversion of required command-line-arg to char *
      sprintf(loopcount, "%d", array[i].b);
      deadline = (int) array[i].d;
      sprintf(deadline_, "%d", deadline);	//do the exec system call to create a new process of type longrun.c
      execlp("./longrun", "./longrun", processid, deadline_, loopcount, "10", NULL);
    }
    else if(pid < 0){				//fork error
      printf("Fork error !\n");
    }
    else{					//parent process
      //PARENT-PROCESS
    }
  }
  int status;					//status variable
  for(int i = 0;i < n;i++)			//wait for all the processes to finish their execution
    wait(&status);
  fclose(fp);					//close the input file
  return 0;
}
