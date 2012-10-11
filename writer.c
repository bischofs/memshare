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

int main ()
{
       int shmId;
   	char *shmPtr;
	key = getuid();
	int check = 1;
   	
	if ((shmId = shmget ((key_t) key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
   		perror ("i can't get no..\n");
      		exit (1);
   	}
   	
	if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      		perror ("can't attach\n");
      		exit (1);
   	}

	printf("Memory attached at %X\n", (int)(intptr_t)shmPtr);
	
	shared_stuff = (struct shared_use_st *)shmPtr;
    	while(check) {
        	printf("Enter some text: ");
        	fgets(buffer, BUFSIZ, stdin);
		printf("buffer: %s",buffer);        
        	strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
        	shared_stuff->written_by_you = 1;

        	if (strncmp(buffer, "end", 3) == 0) {
                	check = 0;
        	}
    	}

	if (shmdt(shmPtr) == -1) {
        	fprintf(stderr, "shmdt failed\n");
        	exit(EXIT_FAILURE);
    	}
 
	return 1;
}


