#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // sleep()
#include <fstream>


const char* file_name = "/home/box/main.pid";
pthread_mutex_t mutex;
pthread_spinlock_t spin;
pthread_rwlock_t read_lock;
pthread_rwlock_t write_lock;


void* func_mutex(void*)
{	
	int rc_mutex;
	std::cout << "waiting for lock..." <<"\n";
	rc_mutex = pthread_mutex_lock(&mutex);

	std::cout << "lock is done.." <<"\n";
	rc_mutex = pthread_mutex_unlock(&mutex);

	return NULL;
}

void* func_spin(void*)
{	
	int rc_spin;
	std::cout << "waiting for spin..." <<"\n";
	rc_spin = pthread_spin_lock(&spin);

	std::cout << "spin is done.." <<"\n";
	rc_spin = pthread_spin_unlock(&spin);

	return NULL;
}

void* func_read(void*)
{
	int rc_read;
	std::cout << "waiting for read lock..." <<"\n";
	rc_read = pthread_rwlock_rdlock(&read_lock);

	std::cout << "read lock is done.." <<"\n";
	rc_read = pthread_rwlock_unlock(&read_lock);

	return NULL;
}

void* func_write(void*)
{
	int rc_write;
	std::cout << "waiting for write lock..." <<"\n";
	rc_write = pthread_rwlock_wrlock(&write_lock);

	std::cout << "write lock is done.." <<"\n";
	rc_write = pthread_rwlock_unlock(&write_lock);

	return NULL;
}

int main(int argc, char const *argv[])
{
// -------------------------------------------------------------------------------

	// write PID	
	// std::fstream f(file_name);
	// f << getpid();
	// f.close();

// -------------------------------------------------------------------------------

	// create processes
	pthread_t thread1, thread2, thread3, thread4;

// -------------------------------------------------------------------------------

	// mutex
	int rc_mutex;
	rc_mutex = pthread_mutex_init(&mutex, NULL);
	rc_mutex = pthread_mutex_lock(&mutex);


	// spin
	int rc_spin;
	rc_spin = pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE); 
	rc_spin = pthread_spin_lock(&spin);

	// read
	int rc_read;
	rc_read = pthread_rwlock_init(&read_lock, NULL);
	rc_read = pthread_rwlock_rdlock(&read_lock);

	int rc_write;
	rc_write = pthread_rwlock_init(&write_lock, NULL);
	rc_write = pthread_rwlock_wrlock(&write_lock);


	int p_status_mutex = pthread_create(&thread1, NULL, func_mutex, NULL);
	int p_status_spin = pthread_create(&thread2, NULL, func_spin, NULL);
	int p_status_read = pthread_create(&thread3, NULL, func_read, NULL);
	int p_status_write = pthread_create(&thread4, NULL, func_write, NULL);

	sleep(5);

	rc_mutex = pthread_mutex_unlock(&mutex);
	rc_mutex = pthread_mutex_destroy(&mutex);
	rc_spin = pthread_spin_unlock(&spin);
	rc_spin = pthread_spin_destroy(&spin);
	rc_read = pthread_rwlock_unlock(&read_lock);
	rc_read = pthread_rwlock_destroy(&read_lock);
	rc_write = pthread_rwlock_unlock(&write_lock);
	rc_write = pthread_rwlock_destroy(&write_lock);

	// join all threads:
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);

	return 0;
}
