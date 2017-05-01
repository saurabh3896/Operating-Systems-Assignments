#include <ctime>
#include <cstdio>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

long fibonacci(long n){                                     // function to calculate nth fibonacci number
  long prev = 1, curr = prev, next = prev, i;               // initialize prev and curr to 1
  for(i = 3;i <= n;i++){                                    // keep taking sum of curr and prev
    next = curr + prev;
    prev = curr;                                            // update prev and curr
    curr = next;
  }
  return next;                                              // next is the nth fibonacci number
}

int main(int argc, char *argv[]){
  long n;
  if(argc > 1)                                              // if command-line argument is given, set n to int(argv[1])
    n = atoi(argv[1]);
  else{
    cout << "Enter the number of terms : ";                 // else take the input from the user
    cin >> n;
  }
  while(n < 10){                                            // loop until user not enters n greater than or equal to 10
    cout << "Please enter a number greater than 10, Enter : ";
    cin >> n;
  }
  int shared;

  clock_t start, end;                                       // start the clock
  start = clock();
  double cpu_time_used;                                     // store the time elapsed in double variable, cpu_time_used
                                                            // create shared memory object, with read/write permissions
  shared = shm_open("/shared_region", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if(shared == -1){                                         // on failure of successful object creation
    cout << "Shared memory segment open/create fail.\n";
    exit(1);                                                // exit with non-zero status
  }
  long *fib_array;
  fib_array = new long[n + 1];                              // allocate memory to fib_array for storing the complete sequence
  fib_array = (long *) mmap(NULL, sizeof(fib_array), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, shared, 0);
  if(fib_array == MAP_FAILED){                              // typecast the pointer to the mapped area (in the virtual address space of the calling process) to long*
    cout << "mmap fail.\n";                                 // case of mmap fail
    exit(1);                                                // exit with non-zero status
  }
  fib_array[0] = 0, fib_array[1] = 1;                       // initialize the top two array values
  for(int i = 2;i <= 5;i++){                                // calculate the first five fibonacci numbers
    fib_array[i] += fib_array[i - 1] + fib_array[i - 2];
  }
  for(int i = 6;i <= n;i++){                                // loop to calculate rest n - 5 fibonacci numbers
    pid_t pid;                                              // create a process id
    if((pid = fork()) == 0){                                // if its a child process, compute ith fibonacci number and place in the shared array
      fib_array[i] = fibonacci(i);
      exit(0);                                              // exit once computed
    }
    else if(pid < 0){                                       // fork error
      cout << "Fork error.\n";
    }
  }
  int status;                                               // status variable whose address is the argument in the wait() function
  for(int i = 6;i < n + 1;++i){                             // the parent prints the whole sequence only after all child processes exit
    wait(&status);                                          // wait for all the child processes to compute their respective fibonacci number
  }
    
  end = clock();                                            // end the clock
  cpu_time_used = ((double) (end - start))/CLOCKS_PER_SEC;  // store the elapsed time in double variable

  cout << "Fibonacci sequence : ";                          // output the fibonacci sequence
  for(int i = 0;i <= n;i++){
    cout << fib_array[i] << " ";
  }
  cout << endl;
  cout << "Time taken : ";
  cout << cpu_time_used << endl;                            // output the time of execution
  shared = shm_unlink("/shared_region");                    // unlink and destroy/de-allocate the shared memory object created
  return 0;
}
