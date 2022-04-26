#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  int status;
  if(argint(0, &status) < 0)
    return -1;
  exit(status);
  return 0;  // not reached
}

int
sys_wait(void)
{
  int *status;
  if(argptr(0, (void*)&status, sizeof(status)) < 0)
    return -1;
  return wait(status);
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

/*
int waitpid(int pid, int *status, int options)
The system call must wait for a process (not necessary a child process) 
with a pid that equals to one provided by the pid argument. The return value 
must be the process id of the process that was terminated or -1 if this process 
does not exist or if an unexpected error occurred. We are required only to
implement a blocking waitpid where the kernel prevents the current process
from execution until a process with the given pid terminates.
*/
int 
sys_waitpid(void) 
{
  int pid;
  int option = 0;
  int *status;
  if(argint(0, &pid) < 0)
    return -1;
    
  if(argptr(1, (void*)&status, sizeof(*status)) < 0)
    return -1;

  return waitpid(pid, status, option);
}
