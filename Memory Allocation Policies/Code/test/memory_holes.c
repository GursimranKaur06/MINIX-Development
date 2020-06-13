#include <lib.h>
#include <sys/resource.h>
#include "memory_holes.h"

int print_memory_holes(int min_size)
{
    message m;
    m.m_type = VM_PRINT_HOLES;
    m.VM_PRINT_HOLES_MIN_LEN = min_size;

    return _syscall(VM_PROC_NR, VM_PRINT_HOLES, &m);
}
