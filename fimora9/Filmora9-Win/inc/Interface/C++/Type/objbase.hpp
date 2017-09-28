/*
*******************************************************************************

    objbase.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_OBJ_BASE_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_OBJ_BASE_HPP

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>

#include "typedef.hpp"

#ifndef	_WIN32
#include "guiddef.hpp"
#include "guidprc.hpp"
#include "macrodef.hpp"
#include "macosmem.hpp"
#include "macosbstr.hpp"
#include "uuidofdef.hpp"
#include "interlocked_func.hpp"
#else
#include "objbase.h"
#endif

#ifndef	_WIN32
#define	MACRO_MIDL_INTERFACE( name, guid )	struct name; type_bind_guid( name, guid ); struct name
#else
#ifndef	_MSC_VER
#define	MACRO_MIDL_INTERFACE( name, guid )	struct name; type_bind_guid( name, "{"guid"}" ); struct name
#else
#define	MACRO_MIDL_INTERFACE( name, guid )	MIDL_INTERFACE( guid ) name
#endif
#endif

#ifndef	_WIN32

#define	STDMETHODCALLTYPE
#define STDAPICALLTYPE
#define STDAPIVCALLTYPE
#define	DECLARE_INTERFACE( iface )	struct iface
#define STDMETHOD( method )		virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_( type, method )	virtual type STDMETHODCALLTYPE method
#define STDMETHODV( method )		virtual HRESULT STDMETHODVCALLTYPE method
#define STDMETHODV_( type, method )	virtual type STDMETHODVCALLTYPE method
#define	STDAPI				EXTERN_C HRESULT STDAPICALLTYPE
#define	STDAPI_( type )			EXTERN_C type STDAPICALLTYPE
#define STDMETHODIMP			HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_( type )		type STDMETHODCALLTYPE

#ifdef	INITGUID
#define	DEFINE_GUID( name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8 )	EXTERN_C const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#else
#define	DEFINE_GUID( name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8 )	EXTERN_C const GUID name
#endif

typedef struct IUnknown *		LPUNKNOWN;
typedef struct IClassFactory *		LPCLASSFACTORY;

MACRO_MIDL_INTERFACE( IUnknown, "00000000-0000-0000-C000-000000000046" )
{
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, void ** ppv ) = 0;

	virtual ULONG STDMETHODCALLTYPE AddRef( void ) = 0;

	virtual ULONG STDMETHODCALLTYPE Release( void ) = 0;
};

MACRO_MIDL_INTERFACE( IClassFactory, "00000001-0000-0000-C000-000000000046" ): public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE CreateInstance( IUnknown * pUnkOuter, REFIID riid, void ** ppv ) = 0;

	virtual HRESULT STDMETHODCALLTYPE LockServer( int fLock ) = 0;
};

EXTERN_C HANDLE CoLoadLibraryA( const char * filename, int autofree );

EXTERN_C HANDLE CoLoadLibraryW( LPOLESTR filename, int autofree );

#define	CoLoadLibrary	CoLoadLibraryW

EXTERN_C void CoFreeLibrary( HANDLE inst );

EXTERN_C void CoFreeAllLibraries( void );

EXTERN_C void CoFreeUnusedLibraries( void );

EXTERN_C HRESULT CoCreateInstance(
	REFCLSID	rclsid,
	LPUNKNOWN	outer,
	DWORD		context,
	REFIID		riid,
	LPVOID *	ppv
);

EXTERN_C HRESULT SetCOMProfile( const char * filename );

#endif

#endif	/* _MACRO_WONDERSHARE_LIBRARY_OBJ_BASE_HPP */
