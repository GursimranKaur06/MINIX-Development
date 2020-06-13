# Implementing User Mode scheduling algorithms in MINIX


## Algorithms


### Random Scheduling

The random scheduling algorithm works by counting the number of processes available and choosing it randomly. The algorithm is nonpreemptive. If the process is in use and its quantum "expires," it instructs the kernel to schedule a time slice for the same process again.
Test Program - busy.c

### Lottery Scheduling

The lottery scheduling algorithm first assigns tickets to processes. It works by finding the number of tickets issued to processes, and then choosing a ticket. The algorithm is preemptive. The user can assign tickets himself to a process through a system call. Setting the number of tickets assigned to a process is done through the `ASSIGN_TICKETS` system call to `pm`. The `pm` "relays" this message to the scheduler.
Test Program - busy-lottery.c

### FCFS Scheduling
This algorithm schedules the processes based on arrival times. The process that arrives first, gets the first time slot, one arriving second gets the next and so on.
