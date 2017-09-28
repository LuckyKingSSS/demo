/*
*******************************************************************************

    macosmem.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_MACOS_MEM_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_MACOS_MEM_HPP

#ifndef	_WIN32

#include "macrodef.hpp"

MACRO_EXTERN_C_BEGIN

void * CoTaskMemAlloc( unsigned int cb );

void * CoTaskMemRealloc( void * pv, unsigned int cb );

void CoTaskMemFree( void * pv );

void * GlobalAlloc( unsigned int cb );

void * GlobalReAlloc( void * pv, unsigned int cb );

void * GlobalFree( void * pv );

unsigned int GlobalSize( void * pv );

MACRO_EXTERN_C_END

#endif	/* _WIN32 */

#endif	/* _MACRO_WONDERSHARE_LIBRARY_MACOS_MEM_HPP */
