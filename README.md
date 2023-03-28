# spammer
Small program that fills the available RAM with garbage and creates an many threads as CPU are available in the system to compute all the prime numbers if no params are passed.

## Usage
If you are using this program from CLI, the usage is as follows if you do not wish to employ all the resources of the target machine:
`spammer <max_mem> <percentage_memory> <num_threads> <log_level>`

In case you are executing this program from `podman` or `docker`, then you shall use some environment variables

`podman run -e MAX_MEM=<max_mem> -e PER_MEM=<percentage_memory> -e NUM_THREADS=<num_threads> -e LOG_LEVEL=<log_level> localhost/spammer:latest`

* `max_mem`: Upper limite for maximum memory to be allocated with garbage expressed on MB
* `percentage_memory`: How much of the memory is really allocated expressed on %
* `num_threads`: number of CPU threads created. Integer value
* `log_level`: By default it is in info level. This might impact on performance due to lock/unlock 

Example of use:
`./spammer 500 70 8 2`

This example will allocate 500 MB at 70%, 8 CPU threads and INFO_LEVEL for logs.

