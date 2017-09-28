/*
*******************************************************************************

    guidprc.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_GUID_PRC_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_GUID_PRC_HPP

#ifndef	_WIN32

#include "typedef.hpp"
#include "guiddef.hpp"
#include "macrodef.hpp"

MACRO_EXTERN_C_BEGIN

extern const GUID	GUID_NULL;
extern const IID	IID_IUnknown;
extern const IID	IID_IClassFactory;

HRESULT StringFromCLSID( REFCLSID rclsid, LPOLESTR * lplpsz );

HRESULT CLSIDFromString( LPCOLESTR lpsz, LPCLSID pclsid );

MACRO_EXTERN_C_END

#endif	/* _WIN32 */

#endif	/* _MACRO_WONDERSHARE_LIBRARY_GUID_PRC_HPP */
