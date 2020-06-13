#include <stdio.h>
#include <lib.h>
#include "calc.h"

int multiply(int multiplicand, int multiplier)
{
    message m;
    m.m1_i1 = multiplicand;
    m.m1_i2 = multiplier;

    int result = _syscall(CALC_PROC_NR, MULTIPLY, &m);
    return result;
}

int add(int augend, int addend) {
    message m;  // Minix uses a message to pass parameters to a system call

    m.m1_i1 = augend;
    m.m1_i2 = addend;

    int result = _syscall(CALC_PROC_NR, ADD, &m);
    return result;
    /* _syscall asks the kernel to ask the server process identified by
    * MYSERVER_PROC_NR (the MYSERVER server process) to execute the system call
    * identified by call number WELCOME with any parameters in the
    * message m (accessed through its address &m).
    */
}
