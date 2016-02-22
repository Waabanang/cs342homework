#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
/*
Stole this example from: http://www.tutorialspoint.com/c_standard_library/c_function_signal.htm
*/


void sighandler(int);

int main(int argc, char * argv[])
{
   signal(SIGINT, sighandler);

   while(1) 
   {
      printf("Going to sleep for a second...\n");
      sleep(1); 
   }
   return(0);
}

void sighandler(int signum)
{
   printf("Caught signal %d, coming out...\n", signum);
   exit(1);
}