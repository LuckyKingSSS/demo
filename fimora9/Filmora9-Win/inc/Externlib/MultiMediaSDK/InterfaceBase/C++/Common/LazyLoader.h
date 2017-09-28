// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LAZYLOADER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LAZYLOADER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LAZYLOADER_EXPORTS
#define LAZYLOADER_API __declspec(dllexport)
#else
#define LAZYLOADER_API __declspec(dllimport)
#endif

void __stdcall LLAddLoadPath(const char* search_path);

void* __stdcall LLGetFuncAddress(const char* module_name, const char* func_name);
