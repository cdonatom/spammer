#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h> 
#include <stdbool.h>
#include <time.h>
#include "log.h"

pthread_mutex_t MUTEX_LOG;

struct thread_args{
    int keep_going;
    int index;
    useconds_t sleep;
};

void log_lock(bool lock, void* udata) {
  pthread_mutex_t *LOCK = (pthread_mutex_t*)(udata);
  if (lock)
    pthread_mutex_lock(LOCK);
  else
    pthread_mutex_unlock(LOCK);
}

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (unsigned long i = 5; i * i <= n; i += 6) {
	if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

void* find_primes(void* arg) {
    struct thread_args* data = (struct thread_args*) arg;
    log_info("Thread %i - Starting benchmark",data->index);
    log_debug("Thread %i: keep going = %i; sleep = %i", data->index, data->keep_going, data->sleep); 
    while(data->keep_going){
      unsigned int seed = time(NULL);
      log_info("seed = %i", seed);
      int start = rand_r(&seed);
      log_debug("Thread %i: start = %i", data->index, start);
      unsigned long end = ULONG_MAX;
      for (unsigned long i = start; i < end; i++) {
	log_trace("Thread %i: checking %ld", data->index, i);
        if ( i % data->sleep )
            usleep(1);
        if (is_prime(i)) {
            log_trace("Thread %i: %ld is prime", data->index, i);
        }
      }
      log_info("Thread %i: start over again!", data->index);
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

    log_info("Total RAM: %lld MB", total_ram);
    log_info("Free RAM: %lld MB", free_ram);

    return (size_t) info.freeram;
}


int main(int argc, char *argv[]) {

    size_t mem_size;
    int num_threads;
    float per_mem;
    int log_lvl = 2;
    useconds_t sleep = 0;
    if(argc > 1 && argc < 4){
      printf("Usage is: %s <max_mem> <percentage_memory> <num_threads> <log_level> <percentage_cpu>", argv[0]);
      return -1;
    }

    if (argc == 1){
       printf("Usage is: %s <max_mem_in_mb> <percentage_memory> <num_threads> <log_level> <percentage_cpu>", argv[0]);
       mem_size = free_ram();
       num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    }
    else{
      mem_size = atoi(argv[1]);
      mem_size *= (1024*1024);
      per_mem = atoi(argv[2]);
      mem_size = (size_t) mem_size*(per_mem/100);
      num_threads = atoi(argv[3]);
      log_lvl = atoi(argv[4]);
      sleep = atoi(argv[5]);
    }
    log_set_level(log_lvl); 
    log_info("MEMSIZE: %lu", mem_size);
    //This values might change over different archs
    log_trace("SIZE OF: void*: %lu bytes", sizeof(void*));
    log_trace("SIZE OF: char*: %lu bytes", sizeof(char*));
    void *based = malloc(mem_size);
    int stage = 1024;
    int initialized = 0;
    if (based) {
        log_info("Allocated %zu Bytes from %p to %p", mem_size, based, based + mem_size);
    } else {
        log_error("Error in allocation.");
        return 1;
    }
    int n = 0;
    while (initialized < mem_size) {  //initialize it in batches
        n++;
        memset((char *)based + initialized, '$', stage);
        initialized += stage;
    }
    log_info("Memory allocated and initialized");

    struct thread_args args[num_threads];
    pthread_t threads[num_threads];
    pthread_mutex_init(&MUTEX_LOG, NULL);
    log_set_lock(log_lock, &MUTEX_LOG);
    for (int i = 0; i < num_threads; i++) {
	args[i].keep_going = 1;
	args[i].index = i;
	args[i].sleep = sleep;
        unsigned int seed = time(NULL);
        int start = rand_r(&seed);
	usleep(sleep*1000000);
        pthread_create(&threads[i], NULL, find_primes, (void*)&args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    free_ram();
    free(based);

    return 0;
}
