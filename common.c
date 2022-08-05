#include "common.h" //importing common.h
#include<stdio.h>

int ee(char * msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int seccomp(unsigned int operation, unsigned int flags, void *args) {
    return syscall(__NR_seccomp, operation, flags, args);
}
