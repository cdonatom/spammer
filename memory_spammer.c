#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

size_t free_ram() {
    struct sysinfo info;
    sysinfo(&info);

    long long total_ram = info.totalram;  // in bytes
    long long free_ram = info.freeram;  // in bytes

    // Convert bytes to megabytes
    total_ram /= (1024 * 1024);
    free_ram /= (1024 * 1024);

    printf("Total RAM: %lld MB\n", total_ram);
    printf("Free RAM: %lld MB\n", free_ram);

    return (size_t) info.freeram;
}

int main(void)
{
    size_t mem_size = free_ram();
    printf("MEMSIZE: %lu\n", mem_size);
    printf("SIZE OF: void*: %lu bytes\n", sizeof(void*));
    printf("SIZE OF: char*: %lu bytes\n", sizeof(char*));
    void *based = malloc(mem_size);  //mem_size = 1024^3
    int stage = 1024;
    int initialized = 0;
    if (based) {
        printf("Allocated %zu Bytes from %p to %p\n", mem_size, based, based + mem_size);
    } else {
        printf("Error in allocation.\n");
        return 1;
    }
    int n = 0;
    while (initialized < mem_size) {  //initialize it in batches
        //printf("%6d %p-%p\n", n, based+initialized, based+initialized+stage);
        n++;
        memset((char *)based + initialized, '$', stage);
        initialized += stage;
    }
	printf("\n\nMemory allocated and initialized\n");
	free_ram();
	printf("Sleeping...\n");
	sleep(20);
    free(based);

    return 0;
}