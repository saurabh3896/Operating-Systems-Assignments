Instructions :

The contents of the assignment directory are :

1. Assgn3-Task1Src-CS14BTECH11031.cpp
2. Assgn3-Task2Src-CS14BTECH11031.cpp
3. Assgn3Readme-CS14BTECH11031.txt
4. Assgn3Report-CS14BTECH11031.pdf
5. comparison.png, which is the plot of running-times of both the programs v/s number of terms (n)

Commands to be run on terminal :

  Task-1 : Multi-threaded program to calculate fibonacci numbers

  1. g++ Assgn3-Task1Src-CS14BTECH11031.cpp -lpthread                 (with the -lpthread option)
  2. [./a.out n]    OR    [./a.out]                                   (n being the command-line argument)

  , where 'n' is the number of terms (fibonacci numbers).

  Task-2 : Multi-process program to calculate fibonacci numbers

  1. g++ Assgn3-Task2Src-CS14BTECH11031.cpp -lrt                      (with the -lrt option)
  2. [./a.out n]    OR    [./a.out]                                   (n being the command-line argument)

  , where 'n' is the number of terms (fibonacci numbers) supplied/input by the user.

  The program automatically determines if the user has supplied 'n' as a commandline argument, if not, the program asks the
  user to input the value of 'n'.

Output :

  Both the programs output the terms of the fibonacci sequence, as well as, the time of execution of the program.
