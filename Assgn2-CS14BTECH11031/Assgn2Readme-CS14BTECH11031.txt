Instructions :

Once the user is cd into the root directory of this Assignment folder, the user needs to cd into the directory "src".
Once the user is inside the "src" directory, there are two files (contents of this directory) :

1. Assgn2Src-CS14BTECH11031.c
2. Makefile

The Makefile consists of obj-m := *module_name*.o, along with the directory paths, with functions such as
make all, make clean, make, etc.
This Makefile is needed to compile the source code and run it.

After a successful make, sudo 'insmod ./*.ko' command is invoked to load the module into the kernel.

The argument to 'insmod' is the kernel object file 'Assgn2Src-CS14BTECH11031.ko',
which loads the module into the linux kernel.

To unload the module, the user can invoke sudo 'rmmod *module_name*' command.
Both the 'insmod' and 'rmmod' commands require root privileges.

The user can enter 'lsmod' to view modules and 'dmesg' to view
the output of the kernel log buffer.

Commands :

            1. make   -   This compiles and runs the program.

            2. sudo insmod ./Assgn2Src-CS14BTECH11031.ko    -   This command inserts the kernel module into the linux kernel.

            3. lsmod    -   To view the newly inserted module (at the top) along other loaded modules.

            4. dmesg    -   To view the current processes' details.

            4. sudo rmmod Assgn2Src-CS14BTECH11031    -   This command removes/unloads the module from the linux kernel.

            5. make clean   -   This command is invoked to clean up any *.o and *.ko and other residual files, thus leaving
                                only the Makefile and Assgn2Src-CS14BTECH11031.c .
