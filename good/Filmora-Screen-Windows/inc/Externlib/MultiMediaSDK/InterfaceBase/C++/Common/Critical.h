// Critical.h : header file

#pragma once


class CCriticalSection 
{
public:
	CCriticalSection() {
		::InitializeCriticalSection(&m_sect);
	}
	virtual ~CCriticalSection()	{
		::DeleteCriticalSection(&m_sect); 
	}

public:
	operator CRITICAL_SECTION*() { return &m_sect; };
	CRITICAL_SECTION m_sect;

// Operations
public:
	void Enter() {
		::EnterCriticalSection(&m_sect);
	}
	void Leave() {
		::LeaveCriticalSection(&m_sect);
	}

};

class CSingleLock
{
public:
	CSingleLock(CCriticalSection& cs)
		: m_cs(cs)
	{
		m_cs.Enter();
	}
	virtual ~CSingleLock()
	{
		m_cs.Leave();
	}

protected:
	CCriticalSection& m_cs;
};