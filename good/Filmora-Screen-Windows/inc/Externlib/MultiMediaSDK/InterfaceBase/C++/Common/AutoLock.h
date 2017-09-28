// AutoLock.h : header file

#pragma once


// wrapper for whatever critical section we have
class CritSec 
{
public:
    CritSec() {
        InitializeCriticalSection(&m_CritSec);
    };

    ~CritSec() {
        DeleteCriticalSection(&m_CritSec);
    };

    void Lock() {
        EnterCriticalSection(&m_CritSec);
    };

    void Unlock() {
        LeaveCriticalSection(&m_CritSec);
    };

private:
    CRITICAL_SECTION m_CritSec;

};


// locks a critical section, and unlocks it automatically
// when the lock goes out of scope
class AutoLock 
{
public:
    AutoLock(CritSec * plock)
    {
        m_pLock = plock;
        m_pLock->Lock();
    };

    ~AutoLock() {
        m_pLock->Unlock();
		m_pLock = 0;
    };

private:
    CritSec * m_pLock;

};
