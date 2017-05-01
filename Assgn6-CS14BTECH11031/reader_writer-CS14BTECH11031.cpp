#include <ctime>
#include <atomic>
#include <vector>
#include <cstdio>
#include <random>
#include <chrono>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/syscall.h>

using namespace std;

double elapsed = 0.0, elapsed_ = 0.0;                           //variables to measure time
vector<int> values;                                             //vector to store input parameters
sem_t wrt, mutex;                                               //semaphore variables declaration
atomic<int> readcount;                                          //atomic readcount variable
FILE *ptr;                                                      //ptr to FILE

struct sharedbuffer{                                            //shared buffer declaration
  int id;
  int iteration;                                                //(id, i)
} buffer;

string currentTime(time_t d){                                   //function to return current time in standard format
  time_t now = time(NULL);                                      //get the current time
  struct tm tstruct;                                            //time struct
  char buf[80];                                                 //char buffer
  tstruct = *localtime(&now);                                   //store current time into struct
  strftime(buf, sizeof(buf), "%X", &tstruct);                   //print in standard format
  return string(buf);                                           //return the buf
}

int delay_sleep(int l){                                         //calculate t on given exponential average lambda
	int sd = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine random_gen(sd);                         //seed the random engine
	exponential_distribution<double> dist_1(1.0/l);               //initialize the exponential_distribution value
	return (int)dist_1(random_gen);                               //typecast to int and return the value of t
}

void *writerTh(void *arg){
  double s, f;
  chrono::milliseconds start, finish;                           //declare variables to record start and finish times
  int thread_no = (long) arg;
  pid_t x = syscall(__NR_gettid);                               //store thread_no and thread id
  for(int i = 1;i <= values[2];i++){                            //repeat for kw times
    s = time(NULL);                                             //get current time
    printf("Writer Thread %d requesting to write to the shared buffer for %dth-time at %s (tid : %d)\n",
      thread_no, i, currentTime(s).c_str(), x);                 //write to stdout as well as file
    fprintf(ptr, "Writer Thread %d requesting to write to the shared buffer for %dth-time at %s (tid : %d)\n",
      thread_no, i, currentTime(s).c_str(), x);

    start = chrono::duration_cast<chrono::milliseconds>(
      chrono::system_clock::now().time_since_epoch()
    );                                                          //get current time (start)

    sem_wait(&wrt);                                             //lock the semaphore for exclusive write access

    finish = chrono::duration_cast<chrono::milliseconds>(
      chrono::system_clock::now().time_since_epoch()
    );                                                          //record the finish time

    f = time(NULL);
    printf("Writer Thread %d obtained the permission to write to the shared buffer for %dth-time at %s (tid : %d)\n",
      thread_no, i, currentTime(f).c_str(), x);
    buffer.id = thread_no;                                      //write to the shared buffer here
    buffer.iteration = i;
    fprintf(ptr, "Writer Thread %d obtained the permission to write to the shared buffer for %dth-time at %s (tid : %d)\n",
      thread_no, i, currentTime(f).c_str(), x);                 //write output again to stdout as well as file
    elapsed += (double) chrono::duration_cast<chrono::milliseconds>(finish - start).count();

    sem_post(&wrt);                                             //unlock the semaphore

    sleep(delay_sleep(values[4]));                              //sleep to simulate some complex operation
  }
}

void *readerTh(void *arg){
  double s, f;
  chrono::milliseconds start, finish;                           //declare variables to record start and finish times
  int thread_no = (long) arg;
  pid_t x = syscall(__NR_gettid);                               //store thread_no and thread id
  for(int i = 1;i <= values[3];i++){                            //repeat for kr times
    s = time(NULL);                                             //record current time for printing
    printf("Reader Thread %d requested to read the shared buffer for %dth-time at %s (tid : %d)\n",
      thread_no, i, currentTime(s).c_str(), x);                 //write to stdout as well as file
    fprintf(ptr, "Reader Thread %d requested to read the shared buffer for %dth-time at %s (tid : %d)\n",
      thread_no, i, currentTime(s).c_str(), x);

    start = chrono::duration_cast<chrono::milliseconds>(
      chrono::system_clock::now().time_since_epoch()
    );                                                          //get current time (start)

    sem_wait(&mutex);                                           //lock reader semaphore here for mutual access to readcount variable
    readcount++;                                                //increment readcount as one more thread is reading now
    if(readcount.load() == 1)                                   //if there exist a reader, make the semaphore of writer wait
      sem_wait(&wrt);
    sem_post(&mutex);                                           //unlock the reader semaphore

    finish = chrono::duration_cast<chrono::milliseconds>(
      chrono::system_clock::now().time_since_epoch()
    );                                                          //record the finish time

    f = time(NULL);
    printf("Reader Thread %d obtained the permission to read the shared buffer for %dth-time at %s (tid : %d)\n",
      thread_no, i, currentTime(f).c_str(), x);
    printf("Read : [%d, %d]\n", buffer.id, buffer.iteration);   //write the output to stdout as well as file
    fprintf(ptr, "Reader Thread %d obtained the permission to read the shared buffer for %dth-time at %s (tid : %d)\n",
      thread_no, i, currentTime(f).c_str(), x);                 //read from the shared buffer here
    fprintf(ptr, "Read : [%d, %d]\n", buffer.id, buffer.iteration);
    elapsed_ += (double) chrono::duration_cast<chrono::milliseconds>(finish - start).count();

    sem_wait(&mutex);                                           //again lock reader semaphore
    readcount--;                                                //as reading is done, decrement readcount
    if(readcount.load() == 0)                                   //if no readers, again unlock the writer semaphore
      sem_post(&wrt);
    sem_post(&mutex);                                           //unlock the reader semaphore

    sleep(delay_sleep(values[5]));                              //sleep to simulate some complex operation
  }
}

int main(int argc, char *argv[]){
  readcount = 0;                                                //initialize readcount variable
  int found;                                                    //found int variable to capture file contents
  std::ofstream ofs("output.txt", std::ios::out | std::ios::trunc); //open file in write mode to clear it
  ofs.close();                                                  //fclose
  ptr = fopen("output.txt", "a+");                              //open the file in append mode
  ifstream infile;                                              //infile to take input
  string input, temp;
  stringstream stream;                                          //variable declarations
  infile.open("inp-params.txt");                                //open the input file
  getline(infile, input);                                       //get the line containing nw, nr, kw, kr, l1, l2
  stream << input;                                              //push the stream into sstream variable
  while(getline(stream, temp, ' ')){                            //get the space separated values and store them into vector
    stringstream(temp) >> found;                                //extract string from strngstream
    values.push_back(found);                                    //store the values in a vector
  }
  if(argc > 1){
    values[0] = atoi(argv[1]);                                  //if nw is supplied as command-line argument, change nw
  }
  /****************************************************************************/
  sem_init(&wrt, 0, 1);
  sem_init(&mutex, 0, 1);                                       //initialize both the semaphores to 1
  pthread_t writer_threads[values[0]], reader_threads[values[1]];
  for(long i = 1;i <= values[0];i++){                           //create writer threads
    pthread_create(&writer_threads[i], NULL, writerTh, (void *) i);
  }
  for(long i = 1;i <= values[1];i++){                           //create reader threads
    pthread_create(&reader_threads[i], NULL, readerTh, (void *) i);
  }
  for(int i = 1;i <= values[0];i++){                            //join writer threads once they finish execution
    pthread_join(writer_threads[i], NULL);
  }
  for(int i = 1;i <= values[1];i++){                            //join reader threads once they finish execution
    pthread_join(reader_threads[i], NULL);
  }
  /****************************************************************************/
  fclose(ptr);                                                  //close the file
  sem_destroy(&wrt);                                            //destroy the writer semaphore
  sem_destroy(&mutex);                                          //destroy the reader semaphore
  /****************************************************************************/
  /*Uncomment below lines if want to store the times taken by reader and writer threads for graph plotting
  FILE *writer_time = fopen("writer.txt", "a+"), *reader_time = fopen("reader.txt", "a+");
  fprintf(writer_time, "%d %f\n", values[0], elapsed/(values[0]*values[2]));
  fprintf(reader_time, "%d %f\n", values[0], elapsed_/(values[1]*values[3]));
  fclose(writer_time); fclose(reader_time);*/
  return 0;
}
