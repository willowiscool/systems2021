#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage:\n\t%s create\t\tcreates resources\n\t%s remove\t\tremoves resources\n", argv[0], argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "create") == 0) {
		printf("Creating file\n");
		int fd = open("story.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
		int key = ftok("story.txt", 7);
		close(fd);

		int semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
		if (semid == -1) {
			printf("Error creating semaphore: %s\n", strerror(errno));
			return 1;
		}

		union semun data;
		data.val = 1;
		int err = semctl(semid, 0, SETVAL, data);
		if (err == -1) {
			printf("Error setting semaphore value: %s\n", strerror(errno));
			return 1;
		}
		printf("Semaphore created\n");

		int shmid = shmget(key, sizeof(int), IPC_CREAT | 0644);
		int* shmdata = shmat(shmid, 0, 0);
		*shmdata = 0;
		shmdt(shmdata);
		printf("Shared memory created\n");
	} else if (strcmp(argv[1], "remove") == 0) {
		int key = ftok("story.txt", 7);
		int semid = semget(key, 1, 0);
		if (semid == -1) {
			printf("Error removing semaphore: %s\n", strerror(errno));
			return 1;
		}
		int err = semctl(semid, IPC_RMID, 0);
		if (err == -1) {
			printf("Error removing semaphore: %s\n", strerror(errno));
			return 1;
		}
		printf("Semaphore removed\n");

		int shmid = shmget(key, sizeof(int), 0);
		shmctl(shmid, IPC_RMID, 0);
		printf("Shared memory removed\n");

		printf("\nStory:\n\n");
		int fd = open("story.txt", O_RDONLY, 0);
		struct stat status;
		stat("story.txt", &status);
		char* file = malloc(status.st_size);
		read(fd, file, status.st_size);
		printf("%s", file);
	} else {
		printf("Usage:\n\t%s create\t\tcreates resources\n\t%s remove\t\tremoves resources\n", argv[0], argv[0]);
		return 1;
	}
}
