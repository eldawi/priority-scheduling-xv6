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

// --- ADD YOUR NEW SYSTEM CALL HANDLER HERE ---
int
sys_setpriority(void)
{
  int new_priority;
  // Get the single argument (new_priority) from the user stack
  if(argint(0, &new_priority) < 0) {
    // If fetching the argument fails, return an error
    return -1;
  }

  // Basic validation: Ensure the priority is within a reasonable range.
  // You should define MIN_PRIORITY and MAX_PRIORITY in param.h or proc.h
  // For example: #define MIN_PRIORITY 0, #define MAX_PRIORITY 100
  if (new_priority < MIN_PRIORITY || new_priority > MAX_PRIORITY) {
    cprintf("setpriority: invalid priority %d (must be between %d and %d)\n",
            new_priority, MIN_PRIORITY, MAX_PRIORITY);
    return -1; // Indicate error for invalid priority
  }

  // Get a pointer to the current process
  struct proc *p = myproc();

  // Acquire the process's lock before modifying its fields
  // This is crucial for thread safety in a multiprocessor environment.

  // acquire(&p->lock); // This line caused the error
  p->priority = new_priority;
  // release(&p->lock); //this too

  // Important: After changing the current process's priority, it should yield
  // the CPU. This allows the scheduler to immediately pick the highest priority
  // runnable process, which might now be a different one (if the current process's
  // priority was lowered) or even the current process itself (if its priority
  // was raised and it's now the highest).
  yield();

  return 0; // Return 0 on success
}
// ---------------------------------------------