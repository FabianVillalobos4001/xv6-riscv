#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define N 10  // Number of processes to create

int
main(int argc, char *argv[])
{
  int i;
  int n = N;
  int pid;

  // Create N processes
  for(i = 0; i < n; i++) {
    pid = fork();
    if(pid < 0) {
      printf("fork failed\n");
      exit(1);
    }
    if(pid == 0) {  // Child
      // Set different number of tickets for each process
      settickets(50 * (i + 1));
      
      // Do some work to consume CPU
      for(volatile int j = 0; j < 1000000000; j++) {
        if(j % 1000000 == 0) {
          // Print progress periodically
          printf("Process %d (tickets: %d) at %d\n", 
                getpid(), 50 * (i + 1), j/1000000);
        }
      }
      exit(0);
    }
  }

  // Parent waits for all children
  for(i = 0; i < n; i++) {
    wait(0);
  }
  
  exit(0);
}