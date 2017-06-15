#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <cstring>

const int SIZE = 2 * 1024*1024; /*2 MB*/

int main(int argc, char const *argv[])
{
	// get the key of the queue
	key_t key = ftok("/tmp/mem.temp",1);

	// create shm
	int shm_id = shmget(key, SIZE , IPC_CREAT | IPC_EXCL | 0600);
	if(shm_id < 0)
		std::cout << "shmget failed" << "\n";

	// write data
	void* it = shmat(shm_id, NULL, 0);
	if(it == (void*)-1)
		std::cout << "shmat failed" << "\n";

	memcpy(it, "42", 1024*1024+10);

	return 0;
}
