// user/prioritytest.c
// A user-space program to test the priority-based scheduler in xv6.

#include "user.h"   // Standard xv6 user library functions (fork, exit, printf, getpid, etc.)
#include "param.h"  // To get access to DEFAULT_PRIORITY, MIN_PRIORITY, MAX_PRIORITY constants
#include "types.h" //watch later

int main(int argc, char *argv[]) {
    int pid[3]; // Array to store PIDs of child processes
    int i;

    printf(1, "Starting priority test...\n");

    // Loop to fork 3 child processes
    for (i = 0; i < 3; i++) {
        pid[i] = fork(); // Create a new child process

        if (pid[i] == 0) { // This block runs only in the CHILD process
            int my_priority;

            // Assign different priorities to each child based on 'i'
            if (i == 0) {
                my_priority = MIN_PRIORITY;     // Child 1: Highest priority
            } else if (i == 1) {
                my_priority = DEFAULT_PRIORITY; // Child 2: Medium priority
            } else {
                my_priority = MAX_PRIORITY;     // Child 3: Lowest priority
            }

            printf(1, "Child %d (PID %d) setting priority to %d.\n", i + 1, getpid(), my_priority);
            
            // Call the custom system call to set the process's own priority
            setpriority(my_priority);

            // CPU-intensive task: A loop to consume CPU time
            // Adjust loop count (100,000,000) based on your QEMU speed
            // If it finishes too fast, increase it. If it never finishes, decrease it.
            for (int j = 0; j < 1000000000; j++) { // Increased loop count for better observation
                // Print progress periodically to see which child is running
                if (j % 100000000 == 0) { // Print every 100 million iterations
                    printf(1, "PID %d (Prio %d): Progress %d%%\n", getpid(), my_priority, (int)(j / 10000000));
                }
            }
            printf(1, "Child %d (PID %d) finished its task.\n", i + 1, getpid());
            exit(); // Child process exits
        } else if (pid[i] < 0) { // Error case: fork failed
            printf(2, "fork failed for child %d\n", i + 1);
            exit(); // Parent exits if fork fails
        }
        // Parent process continues here to fork the next child
    }

    // Parent waits for all children to finish
    for (i = 0; i < 3; i++) {
        wait(); // Wait for any child to exit
    }

    printf(1, "Priority test finished. All children have exited.\n");
    exit(); // Parent process exits
}