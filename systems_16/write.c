#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int key = ftok("story.txt", 7);
	int semid = semget(key, 1, 0);
	if (semid == -1) {
		printf("Error getting semaphore: %s\n", strerror(errno));
		return 1;
	}
	struct sembuf smop;
	smop.sem_num = 0;
	smop.sem_flg = SEM_UNDO;
	smop.sem_op = -1;
	printf("Waiting for semaphore\n");
	semop(semid, &smop, 1);

	int shmid = shmget(key, sizeof(int), 0);
	int *shmdata = shmat(shmid, 0, 0);
	int fd = open("story.txt", O_RDWR | O_APPEND, 0);
	lseek(fd, *shmdata * -1, SEEK_END);
	char* file = malloc(*shmdata);
	read(fd, file, *shmdata);
	printf("Previous line: %s\n", file);

	char input[10000]; // hope nobody writes extra-long stories
	printf("Enter next line: ");
	fgets(input, 10000, stdin);
	*shmdata = strlen(input);

	write(fd, input, strlen(input));

	smop.sem_op = 1;
	semop(semid, &smop, 1);
}
