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
#include <sys/syscall.h>

using namespace std;

fstream outfile;                                                //file to store output
vector<int> values;                                             //vector to store n, k, l1, l2
double elapsed = 0.0;                                           //measure average time taken in ms

class Bounded{                                                  //bounded lock class
  private:
    atomic<bool> *waiting;                                      //array of atomic bools
    std::atomic_flag lock_stream = ATOMIC_FLAG_INIT;            //initialize lock variable
  public:
    Bounded(int number_of_threads){                             //constructor
      waiting = new atomic<bool>[number_of_threads];            //allocate memory
      memset(waiting, false, sizeof(waiting));
      for(int i = 0;i < number_of_threads;i++){                 //initialize all waiting values to be false
        waiting[i].store(false);
      }
      lock_stream.clear();                                      //set the lock variable to be false
    }
    ~Bounded(){};                                               //destructor
    void lock(int);                                             //lock method
    void unlock(int);                                           //unlock method
};
Bounded *test;                                                  //pointer to object of lock class

void Bounded::lock(int i){                                      //lock method
  bool key = true;
  waiting[i].store(true);                                       //set current thread waiting status to be true
  while(waiting[i].load() && key){                              //standard while loop to lock other threads
    key = lock_stream.test_and_set();                           //set key to existing lock value and set lock to opposite value
  }
  waiting[i].store(false);                                      //set waiting status of current thread to be false
}

void Bounded::unlock(int i){                                    //unlock method
  int n = values[0];                                            //set n to be no. of threads
  int j = (i + 1) % n;                                          //circular traversal of array
  while((j != i) && !(waiting[j].load())){                      //standard while loop
    j = (j + 1) % n;                                            //traverse to next array position
  }
  if(j == i){                                                   //if same thread reached, set lock variable to false
    lock_stream.clear();
  }
  else{
    waiting[j].store(false);                                    //else set waiting status of current thread to false
  }
}

string currentTime(time_t d){                                   //function to return current time in standard format
  time_t now = time(NULL);                                      //get the current time
  struct tm tstruct;                                            //time struct
  char buf[80];                                                 //char buffer
  tstruct = *localtime(&now);                                   //store current time into struct
  strftime(buf, sizeof(buf), "%X", &tstruct);                   //print in standard format
  return string(buf);                                           //return the buf
}

int delay(int l){                                               //calculate t on given exponential average lambda
	int sd = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine random_gen(sd);                         //seed the random engine
	exponential_distribution<double> dist_1(1.0/l);               //initialize the exponential_distribution value
	return (int)dist_1(random_gen);                               //typecast to int and return the value of t
}

void *testCS(void *arg){                                        //testCS method
  time_t curr;                                                  //variable to store current time
  string push = "";                                             //string variable to push into file
  pid_t x = syscall(__NR_gettid);                               //get current threadid
  int thread_no = (long) arg;                                   //get thread number
  for(int i = 1;i <= values[1];i++){                            //enter into cs k times (second vector element)
    double start, finish;
    start = time(NULL);                                         //get current time
    printf("%dthCS\tRequest at\t%s by thread %d (tid - %d)\n", i, currentTime(start).c_str(), thread_no, x);
    push = to_string(i) + "th\tCS Request at\t" + currentTime(curr) + " by thread " + to_string(thread_no) + " (tid - " + to_string(x) + ")\n";
    fflush(stdout);                                             //fflush from stdout
    start = time(NULL);                                         //get start time

    test->lock(thread_no - 1);                                  //call the lock method

    finish = time(NULL);                                        //get end time
    elapsed += (finish - start);                                //get time elapsed
    printf("%dthCS\tEntry at\t%s by thread %d (tid - %d)\n", i, currentTime(finish).c_str(), thread_no, x);
    push += to_string(i) + "th\tCS Entry at\t" + currentTime(curr) + " by thread " + to_string(thread_no) + " (tid - " + to_string(x) + ")\n";
    fflush(stdout);                                             //fflush from stdout
    sleep(delay(values[2]));                                    //sleep since thread entered the cs

    test->unlock(thread_no - 1);                                //call the unlock method once exited the cs section

    curr = time(NULL);                                          //get the current time
    printf("%dthCS\tExit at\t\t%s by thread %d (tid - %d)\n", i, currentTime(curr).c_str(), thread_no, x);
    push += to_string(i) + "th\tCS Exit at\t" + currentTime(curr) + " by thread " + to_string(thread_no) + " (tid - " + to_string(x) + ")\n\n";
    fflush(stdout);                                             //fflush from stdout
    sleep(delay(values[3]));                                    //sleep since thread exited the cs
    outfile << push;                                            //push the output to the file
  }
  pthread_exit(NULL);                                           //pthread EXIT
}

int main(int argc, char *argv[]){
  int found;                                                    //found int variable to capture file contents
  std::ofstream ofs("bounded-output.txt", std::ios::out | std::ios::trunc);//open file in write mode to clear it
  ofs.close();                                                  //fclose
  outfile.open("bounded-output.txt", std::fstream::in | std::fstream::out | std::fstream::app);//open file r, w and a mode
  outfile << "Bounded Waiting Time lock output : \n\n";         //write first line to output file
  ifstream infile;                                              //infile to take input
  string input, temp;
  stringstream stream;                                          //variable declarations
  infile.open("inp-params.txt");                                //open the input file
  getline(infile, input);                                       //get the line containing n, k, l1, l2
  stream << input;                                              //push the stream into sstream variable
  while(getline(stream, temp, ' ')){                            //get the space separated values and store them into vector
    stringstream(temp) >> found;                                //extract string from strngstream
    values.push_back(found);                                    //v[0]-n, v[1]-k, v[2]-l1, v[3]-l2
  }
  if(argc > 1){
    values[0] = atoi(argv[1]);                                  //if n is supplied as command-line argument
  }
  test = new Bounded(values[0]);                                //new Bounded object
  pthread_t threads[values[0]];                                 //create pthread_t array
  for(long i = 1;i <= values[0];i++){                           //create n threads
    pthread_create(&threads[i], NULL, testCS, (void *) i);      //function pointer to testCS method
  }
  for(int i = 1;i <= values[0];i++){                            //join all the threads
    pthread_join(threads[i], NULL);
  }
  return 0;
}
