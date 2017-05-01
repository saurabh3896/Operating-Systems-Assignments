The project folder contains 
	1.sourcecode -Contains the scheduler files for diff
	2.Designdocumentation.pdf -Describes the goals,design,implementation.
	3.Screenshot of an implementation
	4.Values.txt :Contains the waitiing and turnaround time
	5.script.py -Script used by me to processa all the values and generate the average.
How to run/implement

1.  Copy the respective scheduler file into /usr/src/minix/servers/sched as schedule.c

2. 	cd /usr/src/releasetools
	make hdboot
	This makes all dependencies of the scheduler by rebuilding the kernel.

3.  cd <folder_path_containing_testfiles>
	clang longrun1.c -o longrun1 
	clang longrun2.c -o longrun2
	clang mytest.c 
	./a.out > scheduler.txt
	This writes waiting & turnaround times of 10 longrun0 and longrun1 instances scheduled according to our scheduler to the logfile.