/* ### WS@H Project:NLE ### */
/***********************************************************************/
/**
* Copyright(c) Wondershare software Corporation
* All rights reserved.
*
* @file		NLEMacroDefine.h
* @brief	常见宏定义
* @class	
*  															
* 功能:	实现常见工具函数 \n
* IDE:	Visual Studio 2008 \n
* 															
* @version	1.0.0.0
* @author	程伟
* @date		2014/05/14
*/
/***********************************************************************/
#ifndef _COMMON_MACRODEFINE_H
#define _COMMON_MACRODEFINE_H

#ifndef NLE_ASSERT

#ifdef _DEBUG
#include <assert.h>
#ifdef WIN32
#include "crtdbg.h"
#define NLE_ASSERT(x)  _ASSERT(x)
#else
#define NLE_ASSERT(x) assert(x)
#endif

#else //release

#define NLE_ASSERT(x) ((void)0) 

#endif //end #ifdef _DEBUG

#endif //end #ifndef NLE_ASSERT

#define PARENT_ASSERT
//常用宏定义

#ifdef _MSC_VER
#define NLEX64_PLATFORM _WIN64
#else
#define NLEX64_PLATFORM 
#endif

#ifndef CHECK_POINTER
#define CHECK_POINTER(p,ret)		if((p)==NULL) { return (ret); }
#endif

#ifndef RETURN_IF_FAILED
#define	RETURN_IF_FAILED(p, ret)	if(FAILED(p)){return (ret);}
#endif

#ifndef RETURN_IF_FALSE
#define RETURN_IF_FALSE(b,ret)		if((b)==FALSE){return (ret);}
#endif

#ifndef RETURN_IF_NULL
#define RETURN_IF_NULL(b,ret)		if((b)==NULL){return (ret);}
#endif

#ifndef FAILED_RETURN
#define FAILED_RETURN(p)			if(FAILED(p)){return;}
#endif

#ifndef NULL_RETURN
#define NULL_RETURN(p)				if((p)==NULL){return;}
#endif

#ifndef FALSE_RETURN
#define FALSE_RETURN(b)				if((b) == FALSE){return;}
#endif

#ifndef FAILED_BREAK
#define FAILED_BREAK(p)				if (FAILED(p)){break;}
#endif

#ifndef NULL_BREAK
#define NULL_BREAK(p)				if(!(p)){break;}
#endif

#ifndef BREAK_IF_FAILED
#define	BREAK_IF_FAILED(p)			if(FAILED(p)){break;}
#endif

#ifndef BREAK_IF_NULL
#define BREAK_IF_NULL(p)			if((p)==NULL){break;}
#endif

#ifndef BREAK_IF_FALSE
#define BREAK_IF_FALSE(b)			if((b)==FALSE){break;}
#endif

#ifndef BREAK_IF_ZERO
#define BREAK_IF_ZERO(b)			if(0==(b)) { break;}
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)				if((p) != NULL){delete (p); (p)=NULL;}
#endif

#ifndef SAFE_ADDREF
#define SAFE_ADDREF(p)				if((p) != NULL){(p)->AddRef();}
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)             if((p) != NULL) {(p)->Release(); p = NULL;}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif


#ifndef SAFE_DESTROYIMG
#define SAFE_DESTROYIMG(p)			if((p) != NULL){DIDestroy(p);(p) = NULL;}
#endif

#ifndef GOTO_IF_FALSE
#define GOTO_IF_FALSE(br, label)	if(!(br)){goto label;}
#endif

#ifndef GOTO_IF_NULL
#define GOTO_IF_NULL(p, label)		if (NULL == (p)){goto label;}
#endif

#ifndef GOTO_IF_FAILED
#define GOTO_IF_FAILED(hr, label)		if (FAILED(hr)){goto label;}
#endif

#ifndef CONTINUE_IF_FALSE
#define CONTINUE_IF_FALSE(br)		if(!(br)){continue;}
#endif

#ifndef CONTINUE_IF_NULL
#define CONTINUE_IF_NULL(br)		if(NULL == (br)){continue;}
#endif

#ifndef CHECK
#define CHECK(hr) NLE_ASSERT(SUCCEEDED(hr) );
#endif

#ifndef MILISECONDS
#define MILISECONDS	1000
#endif

//秒转换到100纳秒
#ifndef S_TO_100NS
#define S_TO_100NS (10000000LL)
#endif

#define E_UNSEEK                    _HRESULT_TYPEDEF_(0x800000A1L)
#define E_UNKNOWLENGTH              _HRESULT_TYPEDEF_(0x800000A2L)
#define E_NOFILE					_HRESULT_TYPEDEF_(0x800000A3L)
#define S_NEWFILE					_HRESULT_TYPEDEF_(0x00010101L)
#define	NLE_PROJDOC_VERSIONERROR    _HRESULT_TYPEDEF_(0x00010102L)

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef NLE_MAX
#define NLE_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef NLE_MIN
#define NLE_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef SAFE_FREE_BSTR
#define SAFE_FREE_BSTR(p)			if((p) != NULL){SysFreeString((p)); (p) = NULL;}
#endif

#define GET_A_COLOR(clr) ((clr&0xFF000000)>>24)
#define GET_R_COLOR(clr) ((clr&0x00FF0000)>>16)
#define GET_G_COLOR(clr) ((clr&0x0000FF00)>>8)
#define GET_B_COLOR(clr) (clr&0x000000FF)
#define MAKE_COLOR(r, g, b) (0xff000000 | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff))
#define BLACK_COLOR 0xff000000
#define WHILE_COLOR 0xffffffff

#ifdef WIN32
#define _BACKSLASH		L"\\"
#else
#define _BACKSLASH		L"/"
#endif


#ifdef WIN32 

#if !defined(wcsdup)
#define wcsdup _wcsdup
#endif

#endif

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
	((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */

#define QWIDTHBYTES(cx, bit)  (((bit * cx + 31) & ~31) >> 3)

#define DIBSIZE(cx, cy) (QWIDTHBYTES( (cx), 32) * (cy) );

#define QUERY_INTERFACE(pObj, iid, ppvObj)  (NULL == pObj) ? E_INVALIDARG : (pObj)->QueryInterface(iid, (void **)ppvObj)
	


#define E_GET_MEDIA_INFO 0x80400000

#define _P(x) ((NLEPropComPtr)NLECommon::NLEGetProperties(x))

#define NLE_INFINITY (2147483647 - 1) //INT_MAX

#define AUDIO_BYTES_PER_SECOND(samperate, channels, bits_per_sample) ( (samperate) * (channels) * (bits_per_sample)>>3)

#ifndef SAFE_SET_VALUE
#define SAFE_SET_VALUE(ptr, value) if(ptr){ *ptr = value;}
#endif

#ifndef FRAME_ALIGN
#define FRAME_ALIGN(val, n) ( ((val) + (n) - 1) & ~((n) - 1))
#endif

#endif //_COMMON_MACRODEFINE_H