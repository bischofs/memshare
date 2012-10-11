#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdint.h>

#define FOO 4096

#define TEXT_SZ 2048

struct shared_use_st {
    int written_by_you;
    char some_text[TEXT_SZ];
};

int key;
struct shared_use_st *shared_stuff;
char buffer[TEXT_SZ ];

"writer.c" 64L, 1282C

