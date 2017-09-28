#include "NLECommon_OS.h"
#ifdef _MSC_VER
    #include <io.h>
#else
    #include <unistd.h>
#endif
#include <stdlib.h>

#include "NLEComPtr.h"

using namespace  NLECommon::OS;

NLECOMMON_API SystemID NLECommon::OS::GetSystemID()
{
#ifdef _WINDOWS_
	return SystemID_Windows;
#endif
	return SystemID_Mac;
}

NLECOMMON_API LPCWSTR NLECommon::OS::GetSystemVersion()
{
	NLE_ASSERT(FALSE);
	return L"Unknown";
}

typedef struct
{
	SystemID id;
	LPCWSTR postfix;
}SystemDylibPostfixTable;

NLECOMMON_API LPCWSTR NLECommon::OS::GetDynamicLibraryPostfix()
{
	static const SystemDylibPostfixTable tbl[] = 
	{
		{ SystemID_Windows, L".dll" },
		{ SystemID_Mac,		L".dylib" },
	};
	static int tblSize = sizeof(tbl) / sizeof(tbl[0]);

	SystemID curSysID = GetSystemID();
	for (int i = 0; i < tblSize; i++)
	{
		if (tbl[i].id == curSysID)
		{
			return tbl[i].postfix;
		}
	}
	NLE_ASSERT(FALSE);
	return NULL;
}



