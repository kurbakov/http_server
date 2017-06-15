#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <cstring>

const int SIZE = 4 * 1024*1024; /*4 MB*/

int main(int argc, char const *argv[])
{
	// get the key of the queue
	key_t key = ftok("/tmp/mem.temp",1);

	// create shm
	int shm_id = shmget(key, SIZE , IPC_CREAT | IPC_EXCL | 0666);

	// write data
	char data = '42';
	void* it = shmat(key, NULL, 0);
	memcpy((char*)it, data, 10);

	return 0;
}
