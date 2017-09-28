//// GetTime.h
////
////
//
//#pragma once
//
//
//#ifdef WIN32
//
//static LONGLONG GetSecondCount()
//{
//	static LARGE_INTEGER liCounter = {0};
//	if (0 == liCounter.QuadPart)
//		QueryPerformanceFrequency(&liCounter);
//
//	return liCounter.QuadPart;
//}
//
//// unit: MilliSecond (high precision)
//__inline DWORD highGetTime()
//{
//	LARGE_INTEGER liCurrent = {0};
//	QueryPerformanceCounter(&liCurrent);
//	return (DWORD)(liCurrent.QuadPart * 1000 / GetSecondCount());
//}
//
//#else
//
//#include <sys/times.h>
//
//#define Sleep(a) usleep((a) * 1000)
//
//__inline DWORD timeGetTime()
//{
//	tms tm;
//	return (times(&tm) * 10);
//}
//
//#define highGetTime timeGetTime
//
//	
//#endif


