#ifdef WIN32
#include "qt_windows.h"
#else
#include <dlfcn.h>
#endif
#include "WSCreateInstance.h"


static WSCreateInstance *g_WSCreateInstanceMgr = nullptr;
WSCreateInstance* WSCreateInstance::Instance()
{
	if (g_WSCreateInstanceMgr == nullptr)
		g_WSCreateInstanceMgr = new WSCreateInstance;
	return g_WSCreateInstanceMgr;
}

bool WSCreateInstance::wsCreateInstance(REFCLSID rclsid, LPUNKNOWN outer, DWORD context, REFIID riid, LPVOID * ppv)
{
	typedef HRESULT(STDAPICALLTYPE *WSCREATEINSTANCE)(
		REFCLSID	rclsid,
		LPUNKNOWN	outer,
		DWORD		context,
		REFIID		riid,
		LPVOID *	ppv
		);

#ifdef WIN32
	if (!m_hmodule)
	{
		m_hmodule = LoadLibraryW(TEXT("NLECOMSupport.dll"));
		if (!m_hmodule) 
			return false;
	}
	WSCREATEINSTANCE fun = (WSCREATEINSTANCE)GetProcAddress(m_hmodule, "WSCreateInstance");
    if (!fun) 
		return false;
    return SUCCEEDED(fun(rclsid, outer, context, riid, ppv));

#else
	if (!m_hmodule)
	{
		m_hmodule = dlopen("./libNLEComSupport.dylib", RTLD_GLOBAL);
		if (!m_hmodule) return false;
	}
	WSCREATEINSTANCE fun = (WSCREATEINSTANCE)dlsym(m_hmodule, "WSCreateInstance");
	if (!fun) 
		return false;
	return fun(rclsid, outer, context, riid, ppv) >= 0;
#endif
	
}

WSCreateInstance::WSCreateInstance()
{
	m_hmodule = 0;
}

WSCreateInstance::~WSCreateInstance()
{
	if (m_hmodule)
    {
#ifdef WIN32
		FreeLibrary(m_hmodule);
#else
        dlclose(m_hmodule);
#endif
		m_hmodule = 0;
	}
}

void WSCreateInstance::Release()
{
	if (g_WSCreateInstanceMgr)
	{
		delete g_WSCreateInstanceMgr;
		g_WSCreateInstanceMgr = nullptr;
	}
}
