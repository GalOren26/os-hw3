#ifndef __Lock__
#define __Lock__ 
#include <Windows.h>
#include "HardCodedData.h"
#include "Functions.h"
typedef struct {
	int readers; 
	HANDLE readers_mutex; 
	HANDLE asset_in_use;
	HANDLE turnstile;
	int ErrorValue 
}Lock ;

//init lock object ant it's synch elements if failed put the error_value in the field ErrorValue.
Lock* InitializeLock(int num_of_threads);
//Receives a pointer to the structure of a lock and performs a lock for reading.
//Capture the lock for re-reading if the lock is not occupied for writing
//If the lock is busy writing then wait until Timeout and return Failure.
//enable parrlel reading 
void read_lock(Lock* lock);


#endif