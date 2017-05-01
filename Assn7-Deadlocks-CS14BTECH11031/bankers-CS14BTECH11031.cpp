#include <ctime>
#include <mutex>
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

double myRand(double low, double high){
    std::default_random_engine generator;          //random number generator
    std::uniform_real_distribution<double> distribution(low, high);
    double number = distribution(generator);       //get the number using uniform_real_distribution
    return number;                                 //return the number
}

void printArray(int *array, int n){
    for(int i = 0;i < n;i++){
        if(i == n - 1){
            printf("%d", array[i]);
        }
        else{
            printf("%d, ", array[i]);
        }
    }
    printf("\n");
}

string currentTime(time_t d){                       //function to return current time in standard format
  time_t now = time(NULL);                          //get the current time
  struct tm tstruct;                                //time struct
  char buf[80];                                     //char buffer
  tstruct = *localtime(&now);                       //store current time into struct
  strftime(buf, sizeof(buf), "%X", &tstruct);       //print in standard format
  return string(buf);                               //return the buf
}

int delay_sleep(int l){                             //calculate t on given exponential average lambda
	int sd = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine random_gen(sd);           //seed the random engine
	exponential_distribution<double> dist_1(1.0/l); //initialize the exponential_distribution value
	return (int) dist_1(random_gen);                //typecast to int and return the value of t
}

int restIters, sleep_mu, n, acqRelRatio, m, emptyResources;
int *available;                                     //initialize arrays required for banker's algorithm
int **max_alloc;
int **allocation;
int **need;
int **request;
int **release;
int **tempAllocationState;
int *tempAvailResources;
std::mutex mtx;                                     //mutex for critical section

bool is_safe_state(int iteration){
    bool *finish = new bool[n];                     //initialize boolean array
    memset(finish, false, sizeof(finish));          //initialize all finish values to be 0
    int finished = 0;                               //initialize the finished value to be 0
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            int resource_c = 0;                     //initialize resources_count to be 0
            if(finish[j] == true){
                ;                                   //do nothing
            }
            else{
				for(int k = 0;k < m;k++){           //loop over to check if allocation is true
					if((max_alloc[j][k] - tempAllocationState[j][k]) <= tempAvailResources[k]){
						resource_c++;               //if true, then increase resource count
					}
				}
				if(resource_c == m){                //if all the resources allocated, mark it to be finished
					finish[i] = true;               //mark the finish to be true, if conditions are satisfied
					finished++;                     //Increment the finished variable
					for(int j1 = 0;j1 < m;j1++){    //update available resources
						tempAvailResources[j1] += tempAllocationState[j][j1];
					}
				}
            }
            if(finished == n){                      //if all finished,
                //safe state found
				;
			}
        }
    }
    bool get = true;                                //all finish values must be true
    for(int i = 0;i < n;i++){
        if(finish[i] == false){                     //if any one of the finish values are false,
            get = false;                            //set the get variable to be false
            break;                                  //break
        }
    }
    //safe state not found
    return get;                                     //safe state not found, return false
}

void request_function(int process_no, int *request_array, int iteration){
    mtx.lock();                                     //apply the mutex lock
    for(int i = 0;i < n;i++){
    	for(int j = 0;j < m;j++){                   //loop over all the processes
    		tempAllocationState[i][j] = allocation[i][j];
    		if(i == process_no){                    //get the current process no.
                tempAvailResources[j] = available[j] - request_array[j];
    			tempAllocationState[i][j] += request_array[j];
    		}
    	}
    }
    if(!is_safe_state(iteration)){                  //if not safe state, return
        //printf("Thread %d\'s not allocated resources for %dth iteration for deadlock avoidance.\n", process_no, iteration);
		mtx.unlock();                               //unlock the mutex lock
        return;
    }
    else{                                           //if safe state
        int counts = 0;
        for(int i = 0;i < m;i++){                   //restore components used
            allocation[process_no][i] += request_array[i];
            need[process_no][i] = max_alloc[process_no][i] - allocation[process_no][i];
			available[i] -= request_array[i];       //restore available resources
			counts = counts + (need[process_no][i] == 0);
		}
        mtx.unlock();                               //unlock the mutex lock
        return;
    }
}

void release_function(int process_no, int *release_array){
	mtx.lock();                                     //lock the release function
	for(int i = 0;i < m;i++){
        need[process_no][i] = max_alloc[process_no][i];
		allocation[process_no][i] = 0;              //set its allocation to be zero, once released
        available[i] += max_alloc[process_no][i];   //restore available resources
	}
	mtx.unlock();                                   //unlock the release function
}

void *workerThread(void *arg){
    int thread = (long) arg;                        //typecast the thread
    pid_t x = syscall(__NR_gettid);
    double start, finish;                           //declare variables to record start and finish times
    for(int i = 0;i < restIters;i++){
        sleep(delay_sleep(sleep_mu));
        double randVal = myRand(0.0, 1.0);          //generate the random value
        if(randVal <= acqRelRatio){
            for(int i = 0;i < m;i++){               //if ration satisfied
                need[thread][i] = max_alloc[thread][i] - allocation[thread][i];
                request[thread][i] = myRand(0.0, (double) need[thread][i]);
                allocation[thread][i] += request[thread][i];
            }
            start = time(NULL);                     //record the time
            printf("%dth resource request by thread %d made at time %s consisting of the following items : \n", i + 1, thread, currentTime(start).c_str());
            printArray(request[thread], m);         //print the array
                                                    //call the request function
            request_function(thread, request[thread], i + 1);

            finish = time(NULL);                    //record the finish time
            printf("%dth resource request by thread %d granted at time %s\n", i + 1, thread, currentTime(finish).c_str());
        }
        else{
            for(int i = 0;i < m;i++){               //else release the resources
                release[thread][i] = myRand(0.0, (double) allocation[thread][i]);
                allocation[thread][i] -= release[thread][i];
            }
            start = time(NULL);                     //record the current time
            printf("%dth release request by thread %d made at time %s consisting of the following items : \n", i + 1, thread, currentTime(start).c_str());
            printArray(release[thread], m);         //print the array
                                                    //call the release function
            release_function(thread, release[thread]);

            finish = time(NULL);                    //record the current time
            printf("%dth release request by thread %d granted at time %s\n", i + 1, thread, currentTime(finish).c_str());
        }
    }
    sleep(delay_sleep(sleep_mu));                   //exponential sleep

    start = time(NULL);
    printf("Final release request by thread %d made at time %s consisting of the following items : \n", thread, currentTime(start).c_str());
    printArray(release[thread], m);

    release_function(thread, allocation[thread]);   //call the release function

    finish = time(NULL);                            //capture finish time
    printf("Final release request by thread %d granted at time %s\n", thread, currentTime(finish).c_str());
}

int main(int argc, char const *argv[]){
    string input, temp;                             //declare string variables for file parsing
    stringstream stream;
    cin >> m;                                       //take input into the variable m
    available = new int[m];                         //allocate memory to array
    for(int i = 0;i < m;i++){
        cin >> available[i];                        //take input into the available resources array
    }
    cin >> n;                                       //take input into the variable n
    tempAvailResources = new int[m];                //allocate memory to all the two-d arrays
    allocation = new int*[n];
    need = new int*[n];
    max_alloc = new int*[n];
    request = new int*[n];
    release = new int*[n];
    tempAllocationState = new int*[n];
    for(int i = 0;i < n;i++){                       //memory allocation
        allocation[i] = new int[m];
        need[i] = new int[m];
        max_alloc[i] = new int[m];
        request[i] = new int[m];
        release[i] = new int[m];
        tempAllocationState[i] = new int[m];
    }
    for(int i = 0;i < n;i++){                       //input into the max allocation array
        for(int j = 0;j < m;j++){
            cin >> max_alloc[i][j];                 //scan into the matrix
        }
    }
    cin >> restIters;
    cin >> sleep_mu;
    cin >> acqRelRatio;                             //take input mu, iteration_count and ration
    for(int i = 0;i < n;i++){
        for(int j = 0;j < m;j++){
            allocation[i][j] = 0;                   //initialize allocation matrix
        }
    }
    pthread_t threads[n];                           //create n threads
    for(long i = 0;i < n;i++){
        pthread_create(&threads[i], NULL, workerThread, (void *) i);
    }
    for(int i = 0;i < n;i++){                       //join the threads v/s they finish their execution
        pthread_join(threads[i], NULL);
    }
    return 0;
}
