#ifdef WIN32
#include <windows.h>
__inline LONGLONG NLEGetSecondCount()
{
	static LARGE_INTEGER liCounter = { 0 };
	if (0 == liCounter.QuadPart)
		QueryPerformanceFrequency(&liCounter);

	return liCounter.QuadPart;
}

// unit: Microsecond
__inline DWORD NLEGetMicrosecond()
{
	LARGE_INTEGER liCurrent = { 0 };
	QueryPerformanceCounter(&liCurrent);
	return (DWORD)(liCurrent.QuadPart * 1000000 / NLEGetSecondCount());
}

#else
#include <sys/times.h>
#include "unistd.h"
#include "wstypedef.h"

#define Sleep(a) usleep((a) * 1000)

__inline DWORD NLEGetMicrosecond()
{
	tms tm;
	return (times(&tm) * 10000);
}
#endif
/**@brief 微秒(us)为单位*/
__inline DWORD GetCurrentTimeMicrosecond()
{
	return NLEGetMicrosecond();
}

#ifdef _DEBUG
#define TEST_COST_TIME_BEGIN \
{ \
	static DWORD cost_time_total = 0; \
	static int nCostTimeCount = 0; \
	DWORD cost_time_begin = GetCurrentTimeMicrosecond();

#define TEST_COST_TIME_END \
	DWORD cost_time = GetCurrentTimeMicrosecond() - cost_time_begin; \
	printf("%s, %s, %d: cost time %d us\n", __FILE__, __FUNCTION__, __LINE__, cost_time); }

#else
#define TEST_COST_TIME_BEGIN 
#define TEST_COST_TIME_END 
#endif
