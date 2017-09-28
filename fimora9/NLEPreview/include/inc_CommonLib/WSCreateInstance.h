#pragma once
#include "guiddef.h"

#include "inc_CommonLib/commonlib_global.h"

class COMMONLIB_EXPORT WSCreateInstance
{
public:
	static WSCreateInstance* Instance();

	bool wsCreateInstance(
		REFCLSID	rclsid,
		LPUNKNOWN	outer,
		DWORD		context,
		REFIID		riid,
		LPVOID *	ppv
		);

	void Release();

	WSCreateInstance();
	~WSCreateInstance();

private:
	HMODULE m_hmodule;
};
