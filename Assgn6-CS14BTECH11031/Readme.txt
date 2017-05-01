The contents of the assignment folder are	:
	1.	reader_writer-CS14BTECH11031.cpp
	2.	in-params.txt	:	Contains nw, nr, kw, kr, l1, l2
	3.	graph_script.py - Script used to plot avergage-time taken (ms) by a reader/writer thread to execute its respective task
	4.	Report.pdf - Graph of avergage-times (in ms) taken by reader/writer threads to perform their respective tasks,
      other anomalies explanation, etc.
	5.	Plot.png	-	Plot as given in the report above.

Running on Terminal :
	1.	g++ -std=c++11 reader_writer-CS14BTECH11031.cpp -pthread
  3.  [a.out] for both programs to execute the executable found, can have one command-line argument, n - number of threads.

Note :
The above program use C++11's library functions, and hence the appropriate compiler switch. The output of the above program is
 a log-file named "output.txt".
