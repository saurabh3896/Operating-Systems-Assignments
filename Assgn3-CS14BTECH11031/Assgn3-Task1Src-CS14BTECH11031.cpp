#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

long *fib_array;                                            // create flobal array shareable by threads

long fibonacci(long n){                                     // function to calculate nth fibonacci number
  long prev = 1, curr = prev, next = prev, i;               // initialize prev and curr to 1
  for(i = 3;i <= n;i++){                                    // keep taking sum of curr and prev
    next = curr + prev;
    prev = curr;                                            // update prev and curr
    curr = next;
  }
  return next;                                              // next is the nth fibonacci number
}

void *fib_calculate(void *counter){                         // routine executed by the thread
  long index = (long) counter;                              // typecast to long
  fib_array[index] = fibonacci(index);                      // call the fibonacci() function in the thread
  pthread_exit(0);                                          // exit once the job is done
}

int main(int argc, char *argv[]){
  long n, i;
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
  fib_array = new long[n + 1];                              // allocate memory to the fib_array
  fib_array[0] = 0; fib_array[1] = 1;                       // initialize top two values

  clock_t start, end;                                       // start the clock
  start = clock();
  double cpu_time_used;                                     // store the time elapsed in double variable, cpu_time_used

  for(i = 2;i <= 5;i++){                                    // calculate first five fibonacci numbers in the parent thread
    fib_array[i] += fib_array[i - 1] + fib_array[i - 2];
  }

  pthread_t fib_th[n - 5];                                  // create an array of thread identifiers
  for(i = 6;i <= n;i++){
    pthread_attr_t attr;                                    // the set of thread attributes
    pthread_attr_init(&attr);                               // get the default attributes by passing (initialize attr object)
                                                            // the address of thread attribute object in pthread_attr_init function
    pthread_create(&fib_th[i - 6], &attr, fib_calculate, (void *) i);
  }
  for(i = 6;i <= n;i++){                                    // join the threads once all have computed their fibonacci number
    pthread_join(fib_th[i - 6], NULL);
  }

  end = clock();                                            // end the clock and record the time
  cpu_time_used = ((double) (end - start))/CLOCKS_PER_SEC;

  cout << "Fibonacci sequence : ";
  for(i = 0;i < n + 1;i++){                                 // output the fibonacci sequence
    cout << fib_array[i] << " ";
  }
  cout << endl;
  cout << "Time taken : ";                                  // output the time of execution
  cout << cpu_time_used << endl;
  return 0;
}
