The contents of the assignment folder are	:
	1.	longrun.c
	2.	longrun	-	The executable of the above longrun.c file, executed by the MAIN program
	3.	CS14BTECH11031.cpp	-	The MAIN program
	4.	output.txt : Contains the output of the MAIN program
	5.	input.txt	:	Conatains k = 150 scheduler_policies
	6.	graph_script.py - Script used to calculate the average time for each category and graph plotting as well
	7.	Report.pdf - Graph of waiting times v/s category and explanation.
	8.	Plot.png and Plot_.png	-	Image versions of the Graph.

Running on Terminal :
	1.	g++ CS14BTECH11031.cpp
	2.	sudo ./a.out	-	The execution requires ROOT privileges.

Note :
	The commandline arguements for the longrun.c program, namely :
		LOOP_COUNT and MAX_LOOPS	- the third and fourth commandline argument,
	are set to be 100000 and 20 respectively. The user can modify these values in the MAIN program itself.

	Also, if the user wishes to re-compile longrun.c, this should be compiler using the C++ compiler.
	(g++ -o longrun longrun.c)
