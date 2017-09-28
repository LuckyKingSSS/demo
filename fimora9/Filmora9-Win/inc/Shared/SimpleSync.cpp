#include "stdafx.h"
#include "SimpleSync.h"

#ifdef WIN32
int gettimeofday(struct timeval * tp, void* pNoUsed)
{
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	static const ULONGLONG EPOCH = ((ULONGLONG)116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	ULONGLONG    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((ULONGLONG)file_time.dwLowDateTime);
	time += ((ULONGLONG)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}

#else

int pthread_mutex_timedlock(pthread_mutex_t * mutex, const struct timespec *abstime)
{
	int result;
	int clip = abstime->tv_nsec;
	do
	{
		result = pthread_mutex_trylock(mutex);
		if (result == EBUSY)
		{
			timespec ts;
			ts.tv_sec = 0;
			ts.tv_nsec = 2 * 1000 * 1000;

			/* Sleep for 10,000,000 nanoseconds before trying again. */
			int status = -1;
			while (status == -1)
				status = nanosleep(&ts, &ts);

			clip -= 2 * 1000 * 1000;
		}
		else
			break;
	} while (result != 0 && (clip > 0));

	return result;
}

#endif
