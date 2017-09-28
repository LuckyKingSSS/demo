/*
*******************************************************************************

    typedef.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_TYPE_DEF_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_TYPE_DEF_HPP

#ifndef	_WIN32

#include "Linux32_Typedef.h"

typedef char			CHAR;
typedef wchar_t			WCHAR;
typedef unsigned int		UINT;
typedef  long long 		LONGLONG;

typedef CHAR *			PSTR;
typedef CHAR *			LPSTR;
typedef const CHAR *		PCSTR;
typedef const CHAR *		LPCSTR;
typedef WCHAR *			PWSTR;
typedef WCHAR *			LPWSTR;
typedef const WCHAR *		PCWSTR;
typedef const WCHAR *		LPCWSTR;

typedef WCHAR			OLECHAR;
typedef OLECHAR *		BSTR;
typedef BSTR *			LPBSTR;
typedef OLECHAR *		LPOLESTR;
typedef const OLECHAR *		LPCOLESTR;

#else

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#endif	/* _WIN32 */

#endif	/* _MACRO_WONDERSHARE_LIBRARY_TYPE_DEF_HPP */
