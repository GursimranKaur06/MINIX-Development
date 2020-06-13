#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define PSIZE 4096

int main(int argc, char *argv[])
{
    //Allocate Memory
    int pages_to_alloc = 10;

    if (argc > 2) {
        pages_to_alloc = atoi(argv[2]);
    }

    char *region = minix_mmap(NULL, PSIZE * pages_to_alloc, PROT_READ | PROT_WRITE,
            MAP_PREALLOC | MAP_CONTIG, -1, 0);
    if (region == MAP_FAILED) {
        perror("Could not mmap");
        return 1;
    }

    //Fill Memory
    if(argc > 1)
        strcpy(region, strcat("Hello Region ", argv[1]));
    else
        strcpy(region, "Hello Region");
    printf("Contents of region: %s\n", region);

    //Release Memory
    int unmap_result = minix_munmap(region, PSIZE * pages_to_alloc);
    if (unmap_result != 0) {
        perror("Could not munmap");
        return 1;
    }

    return 0;
}
