#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // Antes del fork: verificar ancestros del proceso actual
  printf("[pre-fork] pid=%d ppid=%d anc0=%d anc1=%d anc99=%d\n",
         getpid(), getppid(), getancestor(0), getancestor(1), getancestor(99));

  int c = fork();
  if(c < 0){
    printf("fork falla\n");
    exit(1);
  }

  if(c == 0){
    // Proceso hijo
    printf("[hijo]    pid=%d ppid=%d anc0=%d anc1=%d anc2=%d anc99=%d\n",
           getpid(), getppid(),
           getancestor(0),   // debe ser el pid del hijo
           getancestor(1),   // debe ser el pid del padre
           getancestor(2),   // normalmente el abuelo (ej: init/sh)
           getancestor(99)); // -1
    exit(0);
  } else {
    // Proceso padre
    int w = wait(0);
    printf("[padre]   pid=%d ppid=%d anc0=%d anc1=%d (esperé a %d)\n",
           getpid(), getppid(),
           getancestor(0),   // debe ser el pid del padre
           getancestor(1),   // debe ser el ppid del padre
           w);
    exit(0);
  }
}
