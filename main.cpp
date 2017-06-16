#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // sleep()


void* func_mutex(void* mutex)
{
	pthread_mutex_t* m;
	m = (pthread_mutex_t*) mutex;
	
	int rc_mutex;
	std::cout << "waiting for lock..." <<"\n";
	rc_mutex = pthread_mutex_lock(m);

	std::cout << "lock is done.." <<"\n";
	rc_mutex = pthread_mutex_unlock(m);

	return NULL;
}


void* func_spin(void* spin)
{
	// pthread_spin_t* s;
	// s = (pthread_spin_t*) spin;
	
	// int rc_spin;
	// std::cout << "waiting for spin..." <<"\n";
	// rc_spin = pthread_spin_lock(s);

	// std::cout << "spin is done.." <<"\n";
	// rc_spin = pthread_spin_unlock(s);

	return NULL;
}



void* func_read();
void* func_write();

int main(int argc, char const *argv[])
{
	pthread_t thread1, thread2, thread3, thread4;

// -------------------------------------------------------------------------------

	// mutex
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	int rc_mutex;
	rc_mutex = pthread_mutex_init(&mutex, NULL);
	rc_mutex = pthread_mutex_lock(&mutex);

	int p_status_mutex = pthread_create(&thread1, NULL, func_mutex, &mutex);
	sleep(5);

	rc_mutex = pthread_mutex_unlock(&mutex);
	rc_mutex = pthread_mutex_destroy(&mutex);
// -------------------------------------------------------------------------------

	// spin
	pthread_spinlock_t spin;

	int rc_spin;
	// rc_spin = pthread_spin_init(&spin, PTHREAD_PROCESS_SHARED); 
	// rc_spin = pthread_spin_lock(&spin);

	int p_status_spin = pthread_create(&thread2, NULL, func_spin, &spin);
	// sleep(5);

	// rc_spin = pthread_spin_unlock(&spin);
	// rc_spin = pthread_spin_destroy(&spin);

// -------------------------------------------------------------------------------

	// join all threads:
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}
