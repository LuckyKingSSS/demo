// strconv.h
//

#pragma once

#ifndef _H_STR_CONV_H_
#define _H_STR_CONV_H_

#ifndef WIN32
	#include <stdlib.h>
	#include "Linux32_Typedef.h"
	#include "utf.h"
#else
	#pragma warning(disable: 4996)
	#pragma warning(disable: 4995)
	#pragma warning(disable: 4005)

	#include <tchar.h>
	#include <malloc.h>
#endif



#ifdef WIN32
#pragma warning(disable: 4100)
#endif
// count 
__inline char* __w2a(char* buf, const wchar_t* s, int count)
{
#ifdef WIN32
    int nLen = 0;
#endif

	if (!s)
		return "";
		
#ifdef WIN32
	nLen = (int)wcslen(s);
	WideCharToMultiByte( CP_ACP, 0, s, -1,
		buf, (nLen+1) * 2, NULL, NULL );
#else
	//wcstombs(buf, s, count);
	WtoUTF8(s, -1, buf, count, false);
#endif

	return buf;
}

__inline wchar_t* __a2w(wchar_t* buf, const char* s, int count)
{
#ifdef WIN32
    int nLen = 0;
#endif

	if (!s)
		return L"";
		
#ifdef WIN32
	nLen = (int)strlen(s);
	MultiByteToWideChar( CP_ACP, 0, s, -1,
		buf, nLen + 1);
#else
	//mbstowcs(buf, s, count);
	UTF8toW(s, -1, buf, count, false);
#endif

	return buf;
}

__inline wchar_t* __w32tow16(wchar_t* buf, const wchar_t* s, int count)
{
	if (!s)
		return L"";
		
#ifdef WIN32
	return (wchar_t*)s;
#else
	W32toW16(s, -1, buf, count, false); 
#endif

	return buf;
}

__inline TCHAR* __w2t(TCHAR* buf, const wchar_t* s, int count)
{
	if (!s)
#ifndef UNICODE
		return "";
#else
		return L"";
#endif
	
#ifdef UNICODE 
	return (TCHAR*)s;
#else
	__w2a(buf, s, count);
#endif

	return buf;
}

__inline TCHAR* __a2t(TCHAR* buf, const char* s, int count)
{
	if (!s)
#ifndef UNICODE
		return "";
#else
		return L"";
#endif

#ifndef UNICODE 
	return (TCHAR*)s;
#else
	__a2w(buf, s, count);
#endif

	return buf;
}

__inline char* __t2a(char* buf, const TCHAR* s, int count)
{
	if (!s)
		return "";

#ifdef UNICODE 
	__w2a(buf, s, count);
#else
	return (char*)s;
#endif

	return buf;
}

__inline wchar_t* __t2w(wchar_t* buf, const TCHAR* s, int count)
{
	if (!s)
		return L"";
		
#ifndef UNICODE 
	__a2w(buf, s, count);
#else
	return (wchar_t*)s;
#endif

	return buf;
}
#ifdef WIN32
#pragma warning(default: 4100)
#endif
#if 1 //old version

#define USES_CONVERSION \
	wchar_t sz_conv_temp[2048] = L"";

#define W2A(s) 		__w2a((char*)sz_conv_temp, s, 2047)
#define A2W(s) 		__a2w((wchar_t*)sz_conv_temp, s, 2047)

#define A2T(s)		__a2t((TCHAR*)sz_conv_temp, s, 2047)
#define W2T(s) 		__w2t((TCHAR*)sz_conv_temp, s, 2047)

#define T2A(s) 		__t2a((char*)sz_conv_temp, s, 2047)
#define T2W(s) 		__t2w((wchar_t*)sz_conv_temp, s, 2047)

#define W32TOW16(s)	__w32tow16((wchar_t*)sz_conv_temp, s, 2047)

#else // new version

#define USES_CONVERSION \
	int _lp_count = 0; (_lp_count); wchar_t* _lpw = NULL; (_lpw); char* _lpa = NULL; (_lpa); TCHAR* _lpt = NULL; (_lpt);


#define W2A(s) 		s ? (_lp_count = (int)(wcslen(s)+2)*sizeof(wchar_t), _lpa = (char*)alloca(_lp_count), \
	__w2a(_lpa, s, (int)(_lp_count - sizeof(wchar_t))))  : (__w2a(NULL, s, 0))

#define A2W(s) 		s ? (_lp_count = (int)(strlen(s)+2)*sizeof(wchar_t), _lpw = (wchar_t*)alloca(_lp_count),\
	__a2w(_lpw, s, (int)(_lp_count - sizeof(wchar_t)))) : (__a2w(NULL, s, 0))

#define A2T(s)		s ? (_lp_count = (int)(strlen(s)+2)*sizeof(wchar_t), _lpt = (TCHAR*)alloca(_lp_count), \
	__a2t(_lpt, s, (int)(_lp_count - sizeof(wchar_t)))) : (__a2t(NULL, s, 0))

#define W2T(s) 		s ? (_lp_count = (int)(wcslen(s)+2)*sizeof(wchar_t), _lpt = (TCHAR*)alloca(_lp_count),\
	__w2t(_lpt, s, (int)(_lp_count - sizeof(wchar_t)))) : (__w2t(NULL, s, 0))

#define T2A(s) 		s ? (_lp_count = (int)(_tcslen(s)+2)*sizeof(wchar_t), _lpa = (char*)alloca(_lp_count),\
	__t2a(_lpa, s, (int)(_lp_count - sizeof(wchar_t)))) : (__t2a(NULL, s, 0))

#define T2W(s) 		s ? (_lp_count = (int)(_tcslen(s)+2)*sizeof(wchar_t), _lpw = (wchar_t*)alloca(_lp_count),\
	__t2w(_lpw, s, (int)(_lp_count - sizeof(wchar_t)))) : (__t2w(NULL, s, 0))

#endif

#endif