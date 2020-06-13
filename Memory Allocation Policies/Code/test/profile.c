#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define EXECUTIONS_PER_ALLOC 100
#define NBR_ALLOC_SIZES 4

static int ALLOC_SIZES[] = {5000, 1000, 100, 10};

int main() {
	pid_t child_pid, wpid;
	int status = 0;

	for (int j = 0; j < NBR_ALLOC_SIZES; j++) {
		struct timeval ts_start;
		gettimeofday(&ts_start, 0);

		for (int i = 1; i <= EXECUTIONS_PER_ALLOC; i++) {
			char execution[6];
			char size[6];
			snprintf(execution, 10, "%d", i);
			snprintf(size, 10, "%d", ALLOC_SIZES[j]);
			char *argv[] = { "./test", execution, size, NULL };
			char *environ[] = { NULL };

			if ((child_pid = fork()) == 0) {
				execve("./test", argv, environ);
			}
		}

		while ((wpid = wait(&status)) > 0);

		struct timeval ts_end;
		gettimeofday(&ts_end, 0);

		double elapsed = (double)(((ts_end.tv_sec - ts_start.tv_sec) * 1000000 + ts_end.tv_usec - ts_start.tv_usec) / EXECUTIONS_PER_ALLOC) / 1000;
		printf("Allocating Size %d, Ended with average time %lf\n", ALLOC_SIZES[j], elapsed);
	}

	return 0;
}
