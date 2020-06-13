#include "kernel/system.h"
#include <minix/endpoint.h>
#include <stdio.h>

/*===========================================================================*
 *				do_multiply					     *
 *===========================================================================*/

int do_multiply(struct proc * caller, message *m_ptr)
{
    int multiplier = m_ptr->CALC_MULTIPLY_MULTIPLICAND;
    int multiplicand = m_ptr->CALC_MULTIPLY_MULTIPLIER;
    int product = multiplier * multiplicand;

    m_ptr->CALC_MULTIPLY_PRODUCT = product;

    return(OK);
}
