Delta Timer
-----------
This sets up a delta timer which can be 
daemonized to run on the background. The 
project includes a driver program to test
the functionality of the timer.

Timer
-----
This uses sys/time.h's itimerval structure
for setting up the timer functionality. This
allows the process to interrupt itself in the
future according to the specified time.

Compilation
-----------
Please use the Makefile provided for building
the executables

Running the program
-------------------
Run the "timerprocess" executable. This starts
the delta timer server on port 1030

The driver program can be edited and used 
according to the application's needs.

It accepts the IP address and the port of the 
delta timer server as command line parameters.
