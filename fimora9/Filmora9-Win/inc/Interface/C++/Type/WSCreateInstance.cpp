/*
*******************************************************************************

    WSCreateInstance.cpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#include <windows.h>
#include <msxml2.h>
#include <string>

#include "TCOMPtrFree.hpp"

#pragma comment( lib, "msxml2.lib" )

//-----------------------------------------------------------------------------

static std::wstring _S_COMProfile( L"macoscom.xml" );

//-----------------------------------------------------------------------------

static bool IXMLDOMDocumentLoad( IXMLDOMDocument * pdoc )
{
	VARIANT		xmlpath;
	VARIANT_BOOL	xmlbool;

	VariantInit( &xmlpath );

	xmlpath.vt      = VT_BSTR;
	xmlpath.bstrVal = SysAllocString( _S_COMProfile.c_str() );

	HRESULT hr = pdoc->load( xmlpath, &xmlbool );

	VariantClear( &xmlpath );

	return SUCCEEDED( hr ) && xmlbool != 0;
}

//-----------------------------------------------------------------------------

static bool GetDylibNameFromCLSID( REFCLSID rclsid, OLECHAR * buffer )
{
	IXMLDOMDocument * pdoc = 0;

	if ( FAILED( CoCreateInstance( CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, __uuidof( IXMLDOMDocument ), ( void ** )&pdoc ) ) )
	{
		return false;
	}

	MACRO_DECLARE_COM_PTR_AUTO_RELEASE( IXMLDOMDocument, pdoc );

	if ( ! IXMLDOMDocumentLoad( pdoc ) )
	{
		return false;
	}

	IXMLDOMNodeList * plst = 0;

	if ( FAILED( pdoc->selectNodes( L"Root/CLSID", &plst ) ) )
	{
		return false;
	}

	MACRO_DECLARE_COM_PTR_AUTO_RELEASE( IXMLDOMNodeList, plst );

	LONGLONG count = 0;

	if ( FAILED( plst->get_length( &count ) ) )
	{
		return false;
	}

	bool result = false;

	LPOLESTR psz = 0;

	if ( FAILED( StringFromCLSID( rclsid, &psz ) ) )
	{
		return false;
	}

	for ( int index = 0; index < count; ++index )
	{
		// TODO:

		IXMLDOMNode * pnode = 0;

		if ( FAILED( plst->get_item( index, &pnode ) ) )
		{
			continue;
		}

		MACRO_DECLARE_COM_PTR_AUTO_RELEASE( IXMLDOMNode, pnode );

		// TODO:

		IXMLDOMNamedNodeMap * pattr = 0;

		if ( FAILED( pnode->get_attributes( &pattr ) ) )
		{
			continue;
		}

		MACRO_DECLARE_COM_PTR_AUTO_RELEASE( IXMLDOMNamedNodeMap, pattr );

		// TODO:

		IXMLDOMNode * pid   = 0;
		IXMLDOMNode * pname = 0;

		if ( FAILED( pattr->get_item( 0, &pid ) ) )
		{
			continue;
		}

		MACRO_DECLARE_COM_PTR_AUTO_RELEASE( IXMLDOMNode, pid );

		if ( FAILED( pattr->get_item( 1, &pname ) ) )
		{
			continue;
		}

		MACRO_DECLARE_COM_PTR_AUTO_RELEASE( IXMLDOMNode, pname );

		if ( pid )
		{
			VARIANT value;

			VariantInit( &value );

			if ( SUCCEEDED( pid->get_nodeValue( &value ) ) )
			{
				result = ! lstrcmpiW( psz, value.bstrVal );
			}

			VariantClear( &value );
		}

		if ( ! result ) continue;

		result = false;

		if ( pname )
		{
			VARIANT value;

			VariantInit( &value );

			if ( SUCCEEDED( pname->get_nodeValue( &value ) ) )
			{
				lstrcpyW( buffer, value.bstrVal );

				result = true;
			}

			VariantClear( &value );
		}

		break;
	}

	CoTaskMemFree( psz );

	return result;
}

//-----------------------------------------------------------------------------

STDAPI WSCreateInstance(
	REFCLSID	rclsid,
	LPUNKNOWN	outer,
	DWORD		context,
	REFIID		riid,
	LPVOID *	ppv
)
{
	context;

	OLECHAR dlname[ MAX_PATH ];

	if ( ! GetDylibNameFromCLSID( rclsid, dlname ) )
	{
		return E_ABORT;
	}

	HMODULE inst = LoadLibraryW( dlname );

	if ( ! inst )
	{
		std::wstring filename( dlname );

		std::string::size_type n = filename.find_last_of( L"\\/" );

		if ( n != std::string::npos ) filename.erase( 0, n + 1 );

		n = _S_COMProfile.find_last_of( L"\\/" );

		if ( std::string::npos == n ) return E_ABORT;

		std::wstring path = _S_COMProfile.substr( 0, n );

		path += '/';

		path += filename;

		inst = LoadLibraryW( path.c_str() );

		if ( ! inst ) return E_ABORT;
	}

	HRESULT ( STDAPICALLTYPE * DllGetClassObject )( const CLSID &, const IID &, void ** );

	DllGetClassObject = ( HRESULT ( STDAPICALLTYPE * )( const CLSID &, const IID &, void ** ) )GetProcAddress( inst, "DllGetClassObject" );

	if ( ! DllGetClassObject )
	{
		return E_ABORT;
	}

	IClassFactory * ptr = 0;

	HRESULT hr = DllGetClassObject( rclsid, IID_IClassFactory, ( void ** )&ptr );

	if ( SUCCEEDED( hr ) && ptr )
	{
		hr = ptr->CreateInstance( outer, riid, ppv );

		ptr->Release();
	}

	return hr;
}

//-----------------------------------------------------------------------------

STDAPI SetCOMProfile( const char * filename )
{
	WCHAR buffer[ MAX_PATH ];

	MultiByteToWideChar( CP_ACP, 0, filename, -1, buffer, MAX_PATH );

	_S_COMProfile = buffer;

	return S_OK;
}

//-----------------------------------------------------------------------------
