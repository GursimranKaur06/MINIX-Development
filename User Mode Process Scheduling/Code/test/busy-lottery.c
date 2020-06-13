#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "lottery.h"

int main(int argc, char *argv[]) {
	int tickets = atoi(argv[1]);
	set_lottery_tickets(tickets);

	unsigned long i;

	for (i = 1; i <= 1000000000ULL; ++i);
	for (i = 1; i <= 1000000000ULL; ++i);

	return 0;
}
