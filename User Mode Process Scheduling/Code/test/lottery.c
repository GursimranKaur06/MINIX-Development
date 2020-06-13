#include <stdio.h>
#include <lib.h>
#include <sys/resource.h>
#include "lottery.h"

int set_lottery_tickets(int tickets)
{
	message m;
	m.m1_i1 = PRIO_PROCESS;
	m.m1_i2 = 0;
	m.m1_i3 = tickets;

	return _syscall(PM_PROC_NR, ASSIGN_TICKETS, &m);
}
