#include <linux/module.h>             // Needed to write kernel module
#include <linux/kernel.h>             // Needed for KERN_INFO
#include <linux/sched.h>              // Needed for task_struct
#include <linux/init.h>               // Needed for module_init() and module_exit() macros
#include <linux/dcache.h>
                                      // .ko is the kernel object file, with automatically kernel generated data-structures, needed by the kernel
int start_module(void){               // invoked at sudo insmod ./*file*.ko
  struct task_struct *task;           // define the structure task_struct to obtain a particular process's details

  // message printed at the time of insmod (module loading), and followed by each of the tasks' details
  printk(KERN_INFO "Kernel module successfully loaded !\n");

  // loop over each process using the 'for_each_process' macro defined in the "linux/sched.h" file
  for_each_process(task){
    // print task-name or executable name using task->comm[16] (char array with maximum capacity of 16 chars)
	  printk("Task-Name : %s -|- ", task->comm);
    // print task-state using task->state
    printk("Task-State : %ld -|- ", task->state);
    // print PID using task->pid
    printk("PID : %d\n", task->pid);
	}
  return 0;                           // successful return from the function
}

void end_module(void){                // invoked at sudo rmmod ./*file*.ko (unloading of the module)
  printk(KERN_INFO "Kernel module successfully unloaded !\n");
}                                     // exit (unload confirmation) message printed once the kernel module is successfully unloaded

module_init(start_module);            // put start_module function in module_init to invoke the function at 'insmod' (insert)
module_exit(end_module);              // put end_module functin in module_exit to invoke the function at 'rmmod' (exit)

MODULE_LICENSE("GPL");                // module infomation including license (license for loadable kernel module), author and description
MODULE_AUTHOR("saurabh");
MODULE_DESCRIPTION("\\'ps -el\\' command implementation using task_struct");
