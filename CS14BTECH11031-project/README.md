Directory structure :

1. source-code - contains test program (CS14BTECH11031-sched.c), longrun.c and the input files.
2. minix-files - contains files to be modified present in MINIX3 OS. (main scheduler files - schedule.c and schedule_non_preemptive.c -> to be renamed to schedule.c if the user wishes to run non-preemptive version of the scheduler)
2. README.md - readme.
3. Report_Design_Documentation.pdf - design documentation.

Files involved in this assignment (make sure they are put in their corresponding correct directories) :

/usr/src/servers/sched/main.c
/usr/src/servers/sched/schedule.c
/usr/src/servers/sched/schedproc.h
/usr/src/servers/pm/proto.h
/usr/src/servers/pm/misc.c
/usr/src/servers/pm/table.c
/usr/src/include/minix/callnr.h
/usr/include/mysyscallscheduling.h (this file needs to be added as compared to above files which are present already in MINIX3)

Files added for the system call in this assignment :

/usr/include/mysyscallscheduling.h -> a new file in /usr/include named mysyscallscheduling.h, remember to #include in longrun.c

MAIN PROCEDURE :

1. Copy the above mentioned files to their respective directories. The directories of the files are mentioned along them.
2. Run this command : cd /usr/src/releasetools && make hdboot
3. Reboot once the files are uploaded to MINIX3 and **make hdboot** is run in /usr/src/releasetools directory.
3. Go to the main assignment directory "source-code" in the parent directory.
4. Run the following commands :
		- clang -o longrun longrun.c
		- clang CS14BTECH11031-sched.c
		- ./a.out

	Note that two input files : inp-params.txt and inp-params0.txt are provided alongside the sourcecode, and the name needs to be
	modified accordingly in CS14BTECH11031-sched.c in order to run the desired input file.

Procedure followed for implementing system call :

1. Added 69th minix syscall as do_setdeadline()
2. Added SETDEADLINE macro at line 68 in include/minix/callnr.h
3. Added do_setdeadline function in misc.c
4. Run this command to invoke the changes : cd /usr/src/releasetools && make services && make install
5. Reboot

FOR NON-PREEMPTIVE PROGRAM :
	As discussed in class, sir told us to implement a static version of this policy, where the same deadline programs run according to FCFS policy. If the user wishes to run non-preemptive program, he/she must rename the file **schedule_non_preemptive.c** to schedule.c and paste in the /usr/src/servers/sched directory, and do rest of the steps as mentioned above.

Points to be noted :

	(i) There is primary function named mysyscall() in /usr/include/mysyscalllib.h, which has setting the message parameters, m1_i1 -> deadline and m1_i2 -> pid.
	(ii) Then, this calls in /usr/src/servers/pm/misc.c 's last function, do_setdeadline, which now passes the contents of m_in to schedule.c's functions.
	(iii) Then, I also made a new type SYS_SETDL = (SCHEDULING_BASE+6), this type is also used in misc.c's last function, which passes type, pid and deadline finally to schedule.c.
	(iv) Also, **_taskcall** is used to send info to schedule.c from misc.c, which by now has already received from defined systemcall.
	(v) I added an extra "switch" case to "main.c", which is specifically for setting deadlines, then again call do_start_scheduling().
	(vi) Now, system-call is complete, now changing code in schedule.c and schedproc.h for further code change.
