#include "syslib.h"

int sys_calc_mul(int multiplicand, int multiplier)
{
    message m;
    m.m_type = CALC_MULTIPLY;
    m.CALC_MULTIPLY_MULTIPLICAND = multiplicand;
    m.CALC_MULTIPLY_MULTIPLIER = multiplier;

    return(_kernel_call(SYS_CALC_MUL, &m));
}
