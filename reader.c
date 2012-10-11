#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

struct shared_use{
  #define TXT_SIZE 2048

	int write;
	char text[TXT_SIZE];
};

int main(void){

int running = 1;
int shmId;

struct shared_use *Data;
void *shared_memory = (void *)0;

int key = getuid();

shmId = shmget((key_t)key, sizeof(struct shared_use), 0666 | IPC_CREAT);

if(shmId == -1){
	fprintf(stderr, "shmget failed\n");
	exit(EXIT_FAILURE);
}

shared_memory = shmat(shmId, (void *)0, 0);

if(shared_memory == (void *)-1){
	fprintf(stderr, "shmat failed\n");
	exit(EXIT_FAILURE);
}

printf("Memory Attached at %X\n", (int)(intptr_t)shared_memory);

Data = (struct shared_use *)shared_memory;
Data->write = 0;
FILE *p = NULL;
char *file = "output.txt";
p = fopen(file, "w");
while(running){
	if(Data->write){
		fwrite(Data->text,strlen(Data->text),1, p);
		printf("Data Read From Memory: %s", Data->text);
	//	fwrite(Data->text,strlen(Data->text),1, p);
		sleep( rand() % 4 );
            	Data->write = 0;
	}
	if(strncmp(Data->text, "Exit", 3) == 0){
		fclose(p);
		running = 0;
	}

}


if(shmdt(shared_memory) == -1){
	fprintf(stderr, "shmdt failed\n");
	exit(EXIT_FAILURE);
}

if(shmctl(shmId, IPC_RMID, 0) == -1){
	fprintf(stderr, "shmctl(IPC_RMID) failed\n");
	exit(EXIT_FAILURE);
}
exit(EXIT_SUCCESS);
}
