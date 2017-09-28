#pragma once

#include <Unknwn.h>

class IRegHelper : public IUnknown
{
public:
	virtual bool __stdcall DoRegistration(char* pszEmail, char* pszProduct, char* pszRegCode) = 0;
	virtual void __stdcall DoUnregistered() = 0;
	virtual bool __stdcall GetRegistered() = 0;
	virtual char* __stdcall GetEmail() = 0;
	virtual char* __stdcall GetRegCode() = 0;
};

typedef void(__stdcall * CreateRegHelper)(char* pszCodeKey, char* pszValue1, char* pszValue2, char* pszProgId, IRegHelper* &pRegHelper);