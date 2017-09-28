/*
*******************************************************************************

    macrodef.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_MACRO_DEF_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_MACRO_DEF_HPP

#ifndef	_WIN32

#include "Linux32_COM.h"

#ifndef	__cplusplus
#define	MACRO_EXTERN_C			extern
#define	MACRO_EXTERN_C_BEGIN
#define	MACRO_EXTERN_C_END
#else
#define	MACRO_EXTERN_C			extern "C"
#define	MACRO_EXTERN_C_BEGIN		extern "C" {
#define	MACRO_EXTERN_C_END		}
#endif

#ifndef	__cplusplus
#define	MACRO_ANONYMOUS_SPACE_BEGIN
#define	MACRO_ANONYMOUS_SPACE_END
#else
#define	MACRO_ANONYMOUS_SPACE_BEGIN	namespace {
#define	MACRO_ANONYMOUS_SPACE_END	}
#endif

#ifndef	EXTERN_C
#ifndef	__cplusplus
#define	EXTERN_C			extern
#else
#define	EXTERN_C			extern "C"
#endif
#endif

#endif	/* _WIN32 */

#endif	/* _MACRO_WONDERSHARE_LIBRARY_MACRO_DEF_HPP */
