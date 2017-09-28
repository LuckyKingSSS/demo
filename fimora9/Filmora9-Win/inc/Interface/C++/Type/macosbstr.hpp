/*
*******************************************************************************

    macosbstr.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_MACOS_BSTR_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_MACOS_BSTR_HPP

#ifndef	_WIN32

#include "typedef.hpp"
#include "macrodef.hpp"

MACRO_EXTERN_C_BEGIN

BSTR SysAllocString( LPCOLESTR str );

BSTR SysAllocStringLen( LPCOLESTR str, UINT len );

BSTR SysAllocStringByteLen( LPCSTR str, UINT len );

int SysReAllocString( LPBSTR src, LPCOLESTR str );

int SysReAllocStringLen( LPBSTR src, LPCOLESTR str, UINT len );

void SysFreeString( BSTR bstr );

UINT SysStringLen( BSTR bstr );

UINT SysStringByteLen( BSTR bstr );

MACRO_EXTERN_C_END

#endif	/* _WIN32 */

#endif	/* _MACRO_WONDERSHARE_LIBRARY_MACOS_BSTR_HPP */
