#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h> 


struct thread_args{
    int keep_going;
    int start;
    int index;
};
	
int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

void* find_primes(void* arg) {
    struct thread_args* data = (struct thread_args*) arg;
    printf("keep going = %i\nstart = %i\n", data->keep_going, data->start); 
    while(data->keep_going){
      int start = data->start;
      int end = UINT_MAX;
      for (int i = start; i < end; i++) {
        if (is_prime(i)) {
            printf("Thread %i: %d is prime\n", data->index, i);
        }
      }
    }
    pthread_exit(NULL);
}

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


int main() {

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

    struct thread_args args[sysconf(_SC_NPROCESSORS_ONLN)];
    pthread_t threads[sysconf(_SC_NPROCESSORS_ONLN)];

    for (int i = 0; i < sysconf(_SC_NPROCESSORS_ONLN); i++) {
	args[i].keep_going = 1;
	args[i].start = 3;
	args[i].index = i;
        pthread_create(&threads[i], NULL, find_primes, (void*)&args[i]);
    }

    for (int i = 0; i < sysconf(_SC_NPROCESSORS_ONLN); i++) {
        pthread_join(threads[i], NULL);
    }
    free_ram();
    free(based);

    return 0;
}
