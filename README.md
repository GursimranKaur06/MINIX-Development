# MINIX-Development

This project involves working with the Unix like operating system - MINIX and tweaking it's source code. It has a microkernel architecture, which means the kernel contains the least amount of components that are actually required to be present in the kernel, and the rest of the components like device drivers are outside the kernel.

This project has the following modules :
1. System and Kernel Calls - Implements custom system and kernel calls for MINIX.
2. User Mode Process Scheduling - Implements different scheduling policies for processes.
3. Memory Allocation Policies - Implements various memory allocation policies for processes.
4. Character Device Driver - Implements a custom character device driver that works with a polymorphic list.