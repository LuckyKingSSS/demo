
#ifndef _H_NLECONFIG_H_
#define _H_NLECONFIG_H_

#if defined(WIN32)

#ifdef EXPORTS
#define NLE_API_C extern "C" __declspec( dllexport )
#else
#define NLE_API_C extern "C" __declspec( dllimport )
#endif

#else //WIN32
#define NLE_API_C
#endif //WIN32

#endif //_H_NLECONFIG_H_
