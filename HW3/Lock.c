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
	lock->ErrorValue = WaitForSingleObjectWrap(lock->readers, TIMEOUT_IN_MILLISECONDS);
	if (lock->ErrorValue != SUCCESS)
		return;



	//lock->turnstile
	/*turnstile.wait()
		turnstile.signal()
		mutex.wait()
		readers += 1
		if readers == 1:
	roomEmpty.wait()
		mutex.signal()*/
}
