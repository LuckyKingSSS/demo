#include "stdafx.h"
#include "NLEValue.h"
#include "guidprc.hpp"

#include <typeinfo>

#ifndef WIN32
HRESULT StringFromCLSID( REFCLSID rclsid, LPOLESTR * lplpsz )
{
    uuid_string_t uuidStr;
    uuid_t uuid;
    
    memcpy(uuid, &rclsid, sizeof(uuid));
    uuid_unparse(uuid, uuidStr);
    
    mbstowcs(*lplpsz, uuidStr, sizeof(uuid) * sizeof(wchar_t));
    
    return S_OK;
}

HRESULT CLSIDFromString( LPCOLESTR lpsz, LPCLSID pclsid )
{
    uuid_string_t uuidStr;
    uuid_t uuid;
    
    wcstombs(uuidStr, lpsz, sizeof(uuidStr));
    uuid_parse(uuidStr, uuid);
    
    memcpy(pclsid, uuid, sizeof(uuid_t));
    
    return S_OK;
}
#endif

