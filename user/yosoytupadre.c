#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("[pre-fork] pid=%d ppid=%d\n", getpid(), getppid());

  int c = fork();
  if(c < 0){
    printf("fork falla\n");
    exit(1);
  }

  if(c == 0){
    // hijo
    printf("[hijo]    pid=%d ppid=%d\n", getpid(), getppid());
    exit(0);
  }else{
    // padre
    int w = wait(0);
    printf("[padre]   pid=%d ppid=%d (esperé a %d)\n", getpid(), getppid(), w);
    exit(0);
  }
}
