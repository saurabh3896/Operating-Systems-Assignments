#include <iostream>             /* I/O  */
#include <cstdlib>              /* Basic C-library functions  */
#include <unistd.h>             /* Symbolic Constants */
#include <string>               /* C++ string class */
#include <sys/types.h>          /* Primitive System Data Types */
#include <errno.h>              /* Errors */
#include <fstream>              /* C++ file-stream library  */
#include <vector>               /* C++ vector library */
#include <pthread.h>            /* POSIX Threads */
#include <sched.h>              /* For scheduling policies  */
#include <sys/wait.h>           /* Definition for wait() function */
#include <sstream>              /* C++ stringstream library */

using namespace std;

string removeSpaces(string str){                            //function to remove spaces from a string
  stringstream ss;                                          //stringstream object
  string temp;
  ss << str;                                                //store the complete string in the sstream
  str = "";
  while(!ss.eof()){                                         //while ss is not empty
    ss >> temp;
    str = str + temp;                                       //keep appending the strings in a string variable
  }
  return str;                                               //return the final space removed string
}

int main(){
  char *args[5];                                            //char array to store commandline arguments
  ifstream input_file;                                      //file-stream object
  std::ostringstream out;
  bool *is_real;
  int no_of_processes = -1, count = 0;                      //variable declarations
  string input, max_loops, loop_count, process_id, skip;
  vector<int> priorities;                                   //vector to store priorities and policies
  vector<string> scheduling_policies;
  input_file.open("input.txt");                             //open the file
  while(getline(input_file, input)){                        //read the file line by line
    if(input.find("/*") != string::npos || input.find("*/") != string::npos){
      continue;                                             //ignore if a comment is encountered
    }
    input = removeSpaces(input);                            //remove spaces from the line
    if(no_of_processes == -1){
      no_of_processes = atoi(input.c_str());                //get the number of processes from the first line
      is_real = new bool[no_of_processes];
      count++;                                              //increment the count variable
    }
    else{
      string test = input.substr(input.length() - 2);       //get the last two chars, if 10 or 90, then priority is specified, meaning a real time process
      if(test == "10" || test == "90" || input.find("10") != string::npos || input.find("90") != string::npos){
        is_real[count - 1] = true;                          //this is a real-time policy
        count++;
        priorities.push_back(atoi(test.c_str()));           //store the priorities in case of real-time process
        input = input.substr(0, input.length() - 2);
        scheduling_policies.push_back(input);               //store the policy name
      }
      else{
        is_real[count - 1] = false;                         //this is not a real-time policy
        count++;
        priorities.push_back(0);                            //store the priorities in case of real-time process (0 in this case)
        scheduling_policies.push_back(input);               //store the policy name
      }
    }
  }
  for(int i = 1;i <= no_of_processes;i++){                  //fork k processes
    pid_t pid;                                              //create a process id
    if((pid = fork()) == 0){
      out.str(std::string());
      out << getpid();                                      //get the process_id
      process_id = out.str();
      out.str(std::string());
      out << (100000);                                      //THIS is the LOOP_COUNT, the second argument passed as 100000
      loop_count = out.str();
      if(is_real[i - 1] == true){                           //if process number is real-time process number
        //this section is for Real-time schedulers
        struct sched_param param;
        param.sched_priority = priorities[i - 1];           //set the priority
        if(scheduling_policies[i - 1] == "SCHED_FIFO"){     //if the scheduler policy is FIFO
          if(sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1){//error handling
        		cout << "--Error1--" << i << endl;              //returns -1 in case of error
        		exit(1);
        	}
        }
        else{                                               //the scheduler policy is ROUND-ROBIN
          if(sched_setscheduler(getpid(), SCHED_RR, &param) == -1){//error handling
        		cout << "--Error2--" << endl;                   //returns -1 in case of error
        		exit(1);
        	}
        }
      }
      else{
        //for Non-real-time schedulers
        struct sched_param param;
        param.sched_priority = 0;
        string expr = scheduling_policies[i - 1];           //set the priority to be 0, which is 0 set as above
        if(expr == "SCHED_BATCH"){                          //if the scheduler policy is SCHED_BATCH
          if(sched_setscheduler(getpid(), SCHED_BATCH, &param) == -1){//error handling
            cout << "--Error3--" << endl;                   //returns -1 in case of error
            exit(1);
          }
        }
        else if(expr == "SCHED_IDLE"){                      //if the scheduler policy is SCHED_IDLE
          if(sched_setscheduler(getpid(), SCHED_IDLE, &param) == -1){//error handling
            cout << "--Error4--" << endl;                   //returns -1 in case of error
            exit(1);
          }
        }
        else{                                               //if the scheduler policy is SCHED_OTHER
          if(sched_setscheduler(getpid(), SCHED_OTHER, &param) == -1){//error handling
            cout << "--Error5--" << endl;                   //returns -1 in case of error
            exit(1);
          }
        }
      }
      args[0] = (char *) "./longrun"; args[1] = (char *) process_id.c_str();//store the longrun commandline arguments
      args[2] = (char *) loop_count.c_str();                //setting the LOOP_COUNT, which was set as 1000000 above 

      /************************************************************************/
      args[3] = (char *) "20";                              //THE MAX_LOOPS, fourth commandline argument of longrun is set here, to be 20, the user can modify here
      /************************************************************************/

      args[4] = NULL;                                       //terminated by NULL char
      char *envp[] = {(char *) "PATH=/bin", (char *) "USER=me", NULL};//execute longrun using the EXECVE system-call
      execve(args[0], args, envp);
      exit(0);
    }
    else if(pid < 0){                                       //fork error
      cout << "Fork error.\n";
    }
  }
  int status;                                               //declare a status variable
  for(int i = 1;i <= no_of_processes;i++){                  //wait for all the child processes to finish their execution
    wait(&status);
  }
  return 0;
}
