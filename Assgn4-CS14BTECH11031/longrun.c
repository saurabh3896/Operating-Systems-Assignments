#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#define LOOP_COUNT_MIN 100
#define LOOP_COUNT_MAX 100000000

//Function to calculate time of running (total turnaround time including waiting time)
long long diff (struct timeval start, struct timeval end) {
  long long difference = (end.tv_sec - start.tv_sec);
  difference *= 1000000;
  //micro-second precision
  difference += (end.tv_usec - start.tv_usec);
  return difference;
}

int main (int argc, char *argv[]) {
  char *idStr;
  unsigned int v;
  int i = 0;
  int iteration = 1;
  int loopCount;
  int maxloops;
  FILE *fp;
  fp = fopen("output.txt", "a");

  //Print current Scheduler, priority, quantum
  //Set your scheduler priority.
  //Print new Scheduler, priority, quantum
  int policy;
  char *scheduler__;
  if ((policy = sched_getscheduler (getpid())) == -1)
    fprintf(stderr, "Unable to get policy.\n");
  else{
    if (policy == SCHED_FIFO)
      scheduler__ = (char *) "SCHED_FIFO";
    else if (policy == SCHED_BATCH)
      scheduler__ = (char *) "SCHED_BATCH";
    else if (policy == SCHED_IDLE)
      scheduler__ = (char *) "SCHED_IDLE";
    else if (policy == SCHED_OTHER)
      scheduler__ = (char *) "SCHED_OTHER";
    else if (policy == SCHED_RR)
      scheduler__ = (char *) "SCHED_RR";
  }

  if (argc < 3 || argc > 4) {
    printf ("Usage: %s <id> <loop count> [max loops]\n", argv[0]);
    exit (-1);
  }
  /* Start with PID so result is unpredictable */
  v = getpid ();
  /* ID string is first argument */
  idStr = argv[1];
  /* loop count is second argument */
  loopCount = atoi (argv[2]);
  if ((loopCount < LOOP_COUNT_MIN) || (loopCount > LOOP_COUNT_MAX)) {
    printf ("%s: loop count must be between %d and %d (passed %d)\n", argv[0], LOOP_COUNT_MIN, LOOP_COUNT_MAX, atoi(argv[2]));
    exit (-1);
  }
  /* max loops is third argument (if present) */
  if (argc == 4) {
    maxloops = atoi (argv[3]);
  } else {
    maxloops = 0;
  }

  struct timeval start, end;
  double waiting_time = 0;
  //clock_start
  gettimeofday (&start, NULL);

  /* Loop forever - use CTRL-C to exit the program */
  while (1) {
    /* This calculation is done to keep the value of v unpredictable. Since the compiler can't calculate it in advance (even from the original value of v and the loop count), it has to do the loop. */
    v = (v << 4) - v;
    if (++i == loopCount) {
      /* Exit if we've reached the maximum number of loops. If maxloops is 0 (or negative), this'll never happen... */
      if (iteration == maxloops) {
        break;
      }

      /* Feel free to change the output code to make the display clear.
         In case the display from different processes get mixed, you can use synchronization tools to make the display clear.
         You can comment it out if this output is not required.
      */
      printf ("Process id %d - %s:%06d\n", getpid(), idStr, iteration);
      fprintf (fp, "Process id %d - %s:%06d\n", getpid(), idStr, iteration);
      fflush (stdout);
      iteration += 1;
      i = 0;
    }
  }

  //clock_end()
  gettimeofday (&end, NULL);

  /* Print a value for v that's unpredictable so the compiler can't optimize the loop away. Note that this works because the compiler can't tell in advance that it's not an infinite loop. */
  printf ("Total-time taken = %0.6lf seconds\n", (double) (diff(start, end)/1000000.0));
  fprintf (fp, "Total-time taken = %0.6lf seconds\n", (double) (diff(start, end)/1000000.0));
  printf ("The final value of v is 0x%08x of process %d with CURRENT Scheduler-policy : %s\n", v, getpid(), scheduler__);
  fprintf(fp, "The final value of v is 0x%08x of process %d with CURRENT Scheduler-policy : %s\n", v, getpid(), scheduler__);
  fprintf(fp, "/*********************************************************************************************/\n");
}
