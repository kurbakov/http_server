#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>

int main(int argc, char const *argv[])
{
	// get the key of the queue
	key_t key = ftok("/temp/sem.temp", 1);

	// create sim
	int sem_id = semget(key, 16, IPC_CREAT | 0666);

	union semun {
		int val;
		struct semid_ds *buf;
 		ushort *array;
 	} arg;

	for(int i=0; i<16; i++)
	{
		arg.val = i;
		semctl(sem_id, i, SETVAL, arg);
	}

	return 0;
}
