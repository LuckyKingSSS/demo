// GPUChecker.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"


int GetGpuCaps();
int _tmain(int argc, _TCHAR* argv[])
{
	return GetGpuCaps();
}

typedef int(_stdcall * funGetGpu)();
int GetGpuCaps()
{
	HMODULE hModule = LoadLibrary(L"wsCUDA.dll");
	funGetGpu func = (funGetGpu)GetProcAddress(hModule, "AutoGetGpuCaps");
	
	int res = 0;
	__try
	{ 
		res = func();
	}
	__except (1)
	{
		FreeLibrary(hModule);
		return 0;
	}
	FreeLibrary(hModule);
	return res;
}

