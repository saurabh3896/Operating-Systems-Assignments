#include <lib.h>
#include <unistd.h>
#include <minix/endpoint.h>

int mysyscall(int deadline, int pid){
	message m;
	m.m1_i1 = deadline;
	m.m1_i2 = pid;
	return _syscall(PM_PROC_NR, SETDEADLINE, &m);
}
