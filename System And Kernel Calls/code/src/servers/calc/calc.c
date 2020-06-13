#include "inc.h"

int do_multiply(message *m)
{
    return(_kernel_call(SYS_CALC_MUL, m));
}

int do_add(message *m)
{
    return (m->CALC_ADD_AUGEND + m->CALC_ADD_ADDEND);
}
