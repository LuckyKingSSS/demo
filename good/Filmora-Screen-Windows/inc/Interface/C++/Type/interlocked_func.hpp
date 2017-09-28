/*
*******************************************************************************

    interlocked_func.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_INTERLOCKED_FUNC_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_INTERLOCKED_FUNC_HPP

#ifndef	_WIN32

#include "typedef.hpp"
#include "macrodef.hpp"

MACRO_EXTERN_C_BEGIN

LONG InterlockedIncrement( LONG volatile * addend );

LONGLONG InterlockedDecrement64( LONGLONG volatile * addend );

LONG InterlockedDecrement( LONG volatile * addend );

LONGLONG InterlockedIncrement64( LONGLONG volatile * addend );

MACRO_EXTERN_C_END

#endif	/* _WIN32 */

#endif	/* _MACRO_WONDERSHARE_LIBRARY_INTERLOCKED_FUNC_HPP */
