The contents of the assignment folder are	:
	1.	Bounded-CS14BTECH11031.cpp
	2.	Unbounded-CS14BTECH11031.cpp
	3.	in-params.txt	:	Contains n, k, l1, l2
	4.	graph_script.py - Script used to calculate the average time (in ms) to enter critical section for varying number of threads,
	 		graph plotting
	5.	Report.pdf - Graph of avergage-times (in ms) to enter critical section v/s number of threads and explanation
	6.	Plot.png	-	Plot of average time (in ms) taken by each thread v/s number of threads.

Running on Terminal :
	1.	First program - g++ -std=c++11 Bounded-CS14BTECH11031.cpp -pthread
  2.  Second program  - g++ -std=c++11 Unbounded-CS14BTECH11031.cpp -pthread
  3.  [a.out] for both programs to execute the executable found, can have one command-line argument, n - number of threads.

Note :
The above programs use C++11's library functions, and hence the appropriate compiler switch. The output of the two programs namely
Bounded and Unbounded locking-algorithms are bounded-output.txt and unbounded-output.txt respectively.
