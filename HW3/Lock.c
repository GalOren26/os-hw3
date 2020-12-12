#include "Lock.h"

Lock* InitializeLock(int num_of_threads)
{
	Lock* my_lock = calloc(1, sizeof(Lock));
	my_lock->ErrorValue= CheckAlocation(my_lock);
	if (my_lock->ErrorValue != SUCCESS)
		return my_lock;
	my_lock->readers = 0;
	my_lock->ErrorValue = CreateMutexWrap(FALSE,& my_lock->readers_mutex);
	if (my_lock->ErrorValue != SUCCESS)
		return my_lock;
	my_lock->ErrorValue = CreateSemphoreWrap(num_of_threads, &my_lock->asset_in_use,1);
	if (my_lock->ErrorValue != SUCCESS)
		return my_lock;
	my_lock->ErrorValue = CreateMutexWrap(FALSE, &my_lock->turnstile);
	if (my_lock->ErrorValue != SUCCESS)
		return my_lock;
}

void read_lock(Lock* lock)
{
	lock->ErrorValue = WaitForSingleObjectWrap(lock->turnstile, TIMEOUT_IN_MILLISECONDS);
	if (lock->ErrorValue != SUCCESS)
		return;

	lock->ErrorValue = ReleaseMutexeWrap(lock->turnstile);
	if (lock->ErrorValue != SUCCESS)
		return;
	lock->ErrorValue = WaitForSingleObjectWrap(lock->readers_mutex, TIMEOUT_IN_MILLISECONDS);
	if (lock->ErrorValue != SUCCESS)
		return;
	lock->readers += 1; 
	if (lock->readers == 1)
	{
		lock->ErrorValue = WaitForSingleObjectWrap(lock->asset_in_use, TIMEOUT_IN_MILLISECONDS);
		if (lock->ErrorValue != SUCCESS)
			return;
	}
	lock->ErrorValue = ReleaseMutexeWrap(lock->readers_mutex);
	if (lock->ErrorValue != SUCCESS)
		return;
}

void release_read(Lock* lock)
{
	lock->ErrorValue = WaitForSingleObjectWrap(lock->readers_mutex, TIMEOUT_IN_MILLISECONDS);
	if (lock->ErrorValue != SUCCESS)
		return;
	lock->readers -= 1; 
	if (lock->readers == 0)
	{
		lock->ErrorValue = ReleaseSemphoreWrap(lock->asset_in_use, 1);
		if (lock->ErrorValue != SUCCESS)
			return;
	}
	lock->ErrorValue = ReleaseMutexeWrap(lock->readers_mutex);
	if (lock->ErrorValue != SUCCESS)
		return;

}

void lock_write(Lock* lock)
{
	// only the thread that lock this mutex for write can realse it .
	lock->ErrorValue = WaitForSingleObjectWrap(lock->turnstile, TIMEOUT_IN_MILLISECONDS);
	if (lock->ErrorValue != SUCCESS)
		return;
	lock->ErrorValue = WaitForSingleObjectWrap(lock->asset_in_use, TIMEOUT_IN_MILLISECONDS);
	if (lock->ErrorValue != SUCCESS)
		return;

}
void release_write(Lock* lock)
{
	lock->ErrorValue = ReleaseMutexeWrap(lock->turnstile);
	if (lock->ErrorValue != SUCCESS)
		return;
	lock->ErrorValue = ReleaseSemphoreWrap(lock->asset_in_use, 1);
	if (lock->ErrorValue != SUCCESS)
		return;
}
int DestroyLock(Lock** lock)
{
	int ret_val = 0; 
	ret_val= CloseHandleWrap((*lock)->asset_in_use);
	if (ret_val != SUCCESS)
	{
		return ret_val;
	}
	ret_val = CloseHandleWrap((*lock)->readers_mutex);
	if (ret_val != SUCCESS)
	{
		return ret_val;
	}
	ret_val = CloseHandleWrap((*lock)->turnstile);
	if (ret_val != SUCCESS)
	{
		return ret_val;
	}
	free(*lock); 
	*lock = NULL;
}

