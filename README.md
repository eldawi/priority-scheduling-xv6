<h1> xv6 Priority-Based Process Scheduler </h1>

This project involved modifying the xv6 operating system kernel to implement a priority-based process scheduling algorithm. The goal was to replace xv6's default round-robin scheduler with one that prioritizes processes based on a numerical priority value, allowing higher-priority tasks to receive preferential CPU access.

Project Objective
The primary objective was to deepen understanding of operating system process scheduling mechanisms by:

Introducing a concept of "priority" for each process.
Modifying the kernel's scheduler to select processes based on their assigned priority.
Providing a system call for user processes to interact with and manage their own priorities.
Background: xv6's Original Scheduler
xv6, a simplified Unix-like teaching OS, uses a basic round-robin scheduling algorithm. Its scheduler() function (kernel/proc.c) simply iterates through the process table and picks the next RUNNABLE process it finds, giving each process a fair share of CPU time in a cyclic manner. This approach does not differentiate between processes based on their importance.

Implementation Details
To achieve priority-based scheduling, several core components of the xv6 kernel and userland were modified:

Process Priority Field (kernel/proc.h):

A new int priority; field was added to the struct proc definition.
Constants like MIN_PRIORITY (highest), MAX_PRIORITY (lowest), and DEFAULT_PRIORITY were defined to establish the priority range and initial value for new processes.
Default Priority Assignment (kernel/proc.c - allocproc()):

The allocproc() function was updated to assign a DEFAULT_PRIORITY to every newly created process.
Priority-Aware Scheduler (kernel/proc.c - scheduler()):

The core scheduler() function was rewritten to iterate through all RUNNABLE processes.
It now selects the process with the highest priority (defined as the numerically smallest priority value) among all available runnable processes.
The existing preemption mechanism (via timer interrupts and yield()) ensures that the scheduler is invoked regularly to re-evaluate priorities.
setpriority System Call:

A new system call, setpriority(int new_priority), was introduced to allow user processes to change their own priority dynamically.
kernel/syscall.h: A unique SYS_setpriority number was defined.
user/user.h: The setpriority function prototype was declared for user-space programs.
user/usys.S: An assembly stub was added to bridge the user-space setpriority call to the kernel's system call handler (movl $SYS_setpriority, %eax; int $T_SYSCALL;).
kernel/syscall.c: The sys_setpriority handler was declared and mapped to its system call number in the kernel's system call table.
kernel/sysproc.c: The sys_setpriority function was implemented. It fetches the new_priority argument, validates it, updates the current process's p->priority field, and then calls yield() to immediately trigger a reschedule based on the new priority. (Note: No p->lock was used, relying on ptable.lock for overall process table protection in this xv6 version).
Demonstration
A user-space test program, prioritytest.c, was created to demonstrate the functionality:

It forks three child processes.
Each child sets its priority to a different level (highest, medium, lowest).
All children then enter a CPU-intensive loop, printing their progress.
Observed Outcome:
As demonstrated by the attached screenshot, the prioritytest program clearly showed:

The child process with Priority 0 (highest) completed its task first.
The child process with Priority 60 (medium) completed its task second.
The child process with Priority 100 (lowest) completed its task last, receiving significantly less CPU time until the higher-priority processes finished.
Key Learnings
This project provided invaluable hands-on experience in:

Understanding and modifying fundamental operating system scheduling algorithms.
Implementing new system calls and integrating them into the kernel and userland.
Working with kernel data structures (e.g., struct proc).
Debugging kernel-level code and understanding compilation/linking processes in a minimal OS environment.

