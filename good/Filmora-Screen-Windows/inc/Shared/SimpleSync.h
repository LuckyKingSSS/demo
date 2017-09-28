#pragma once

#include "stdint.h"
#include "pthread.h"

#ifdef WIN32

#include <windows.h>
#include "winsock.h"

int gettimeofday(struct timeval * tp, void* pNoUsed = NULL);

#else

#include <sys/time.h>
int pthread_mutex_timedlock(pthread_mutex_t * mutex, const struct timespec *abstime);

#endif


namespace SimpleSync
{

class CSimpleMutex
{
public:
    CSimpleMutex()
    : _mutex(PTHREAD_RECURSIVE_MUTEX_INITIALIZER) {
    }
	virtual ~CSimpleMutex() {
	}

	operator pthread_mutex_t *() {
		return &_mutex;
	}

private:
	pthread_mutex_t _mutex;
};

class CSimpleCond
{
public:
	CSimpleCond()
		: _cond(PTHREAD_COND_INITIALIZER) {
	}
	virtual ~CSimpleCond() {
	}

	operator pthread_cond_t *() {
		return &_cond;
	}

private:
	pthread_cond_t _cond;
};

class CSimpleMutexLock
{
public:
	CSimpleMutexLock(CSimpleMutex &m)
		: _mutex(m) {
		pthread_mutex_lock(_mutex);
	}
	virtual ~CSimpleMutexLock() {
		pthread_mutex_unlock(_mutex);
	}

private:
	CSimpleMutex &_mutex;
};

class CSimpleMutexTimedLock
{
public:
	CSimpleMutexTimedLock(CSimpleMutex &m, int64_t usec)
		: _mutex(m) {
		struct timeval now;
		timespec abs_time;
        gettimeofday(&now, NULL);

        int64_t nsec = (usec + int64_t(now.tv_usec)) * 1000;
        
        abs_time.tv_sec = now.tv_sec + nsec / 1000000000;
        abs_time.tv_nsec = nsec % 1000000000;
        _wait_ret = pthread_mutex_timedlock(_mutex, &abs_time);
	}
	virtual ~CSimpleMutexTimedLock() {
		if (_wait_ret == 0)
			pthread_mutex_unlock(_mutex);
	}

private:
	CSimpleMutex &_mutex;
	int _wait_ret;
};

class CSimpleCondWait
{
public:
	CSimpleCondWait(CSimpleCond &c, CSimpleMutex &m) {
		_wait_ret = pthread_cond_wait(c, m);
	}
	virtual ~CSimpleCondWait() {
	}

	int WaitResult() const {
		return _wait_ret;
	}

private:
	int _wait_ret;
};

class CSimpleCondTimedWait
{
public:
	CSimpleCondTimedWait(CSimpleCond &c, CSimpleMutex &m, int64_t usec) {

		struct timeval now;
        struct timespec abs_time;
		gettimeofday(&now, NULL);
        
        int64_t nsec = (usec + int64_t(now.tv_usec)) * 1000;
        
        abs_time.tv_sec = now.tv_sec + nsec / 1000000000;
        abs_time.tv_nsec = nsec % 1000000000;
		_wait_ret = pthread_cond_timedwait(c, m, &abs_time);//ETIMEDOUT 60, EAGAIN 35, EINVAL 22
	}
	virtual ~CSimpleCondTimedWait() {
	}

	int WaitResult() const {
		return _wait_ret;
	}

private:
	int _wait_ret;
};

class CSimpleCondSignal
{
public:
	CSimpleCondSignal(CSimpleCond &c) {
		pthread_cond_signal(c);
	}
	virtual ~CSimpleCondSignal() {
	}
};

class CSimpleCondBroadcast
{
public:
	CSimpleCondBroadcast(CSimpleCond &c) {
		pthread_cond_broadcast(c);
	}
	virtual ~CSimpleCondBroadcast() {
	}
};

}
