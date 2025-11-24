#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// parte de la tarea 3

// helper para mrdprotect y munrdprotect

// Cambia el bit PTE_R en un rango de memoria de usuario.
// set = 0 -> limpia PTE_R (prohibir lectura)
// set = 1 -> pone PTE_R (permitir lectura)
static int
do_rdprotect(uint64 addr, int len, int set)
{
  struct proc *p = myproc();

  // Validaciones básicas
  if (len <= 0)
    return -1;

  if (addr % PGSIZE != 0)  // addr debe estar alineada a página
    return -1;

  uint64 start = addr;
  uint64 end   = addr + (uint64)len * PGSIZE;

  // Espacio de usuario, todo debe estar dentro de [0, p->sz)
  if (start >= p->sz || end > p->sz)
    return -1;

  // Validar todas las páginas
  for (uint64 va = start; va < end; va += PGSIZE) {
    pte_t *pte = walk(p->pagetable, va, 0);
    if (pte == 0)
      return -1;            // no hay PTE para esta VA

    if ((*pte & PTE_V) == 0)
      return -1;            // página no válida

    if ((*pte & PTE_U) == 0)
      return -1;            // no es de usuario (probable kernel)
  }

  // Modificar PTE_R
  for (uint64 va = start; va < end; va += PGSIZE) {
    pte_t *pte = walk(p->pagetable, va, 0);

    if (set)
      *pte |= PTE_R;       // permitir lectura
    else
      *pte &= ~PTE_R;      // quitar lectura
  }

  // Limpiar TLB
  sfence_vma();

  return 0;
}



// Protege una region de memoria contra escrituras
uint64
sys_mrdprotect(void)
{
  uint64 addr;
  int len;

  // En esta versión, argaddr y argint no retornan valor
  argaddr(0, &addr);
  argint(1, &len);

  return do_rdprotect(addr, len, 0);  // 0 = quitar lectura
}

uint64
sys_munrdprotect(void)
{
  uint64 addr;
  int len;

  argaddr(0, &addr);
  argint(1, &len);

  return do_rdprotect(addr, len, 1);  // 1 = restaurar lectura
}
