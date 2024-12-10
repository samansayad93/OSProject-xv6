#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

//this is claude
int
sys_set_limit(void)
{
  int limit;
  if(argint(0, &limit) < 0)
    return -1;
  
  struct proc *p = myproc();
  
  // Validate limit
  if(limit < 0 || limit > 100)
    return -1;
  
  // Strict enforcement
  p->cpu_limit = limit;
  p->cpu_usage_ms = 0;
  p->last_check_tick = ticks;
  
  return 0;
}

//this is chat gpt

// int
// set_limit(int limit) {
//     if (limit < 0 || limit > 100)
//         return -1; // Invalid input
//     struct proc *curproc = myproc();
//     curproc->cpu_quota = limit; // Set the CPU usage limit
//     return 0; // Success
// }

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
