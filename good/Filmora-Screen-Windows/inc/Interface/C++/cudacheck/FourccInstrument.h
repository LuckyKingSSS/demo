//硬件编码检测相关头文件
//支持：cuda / intel / ati
#pragma once

#ifdef	WIN32

#else

#ifndef __stdcall
#define __stdcall 
#endif


#include "typedef.hpp"

#endif


//硬件加速类型
#ifndef __ACCELERATE_TYPE__
#define __ACCELERATE_TYPE__
enum ACCELERATETYPE
{
	HARDWARE_ACCELERATE_NONE = 0, 
	HARDWARE_ACCELERATE_CUDA,
	HARDWARE_ACCELERATE_ATI,
	HARDWARE_ACCELERATE_INTEL,
	HARDWARE_ACCELERATE_NVENC,
	HARDWARE_ACCELERATE_VCE,
	SOFTWARE_ACCELERATE_SYSTEM = 100,//USE SYSTEM ENCODERS
};
#endif //__ACCELERATE_TYPE__

#ifdef __cplusplus
extern "C" {
#endif

// 检测fourcc在当前显卡加速设置下，是否可以支持硬件加速
/* 调用方法:
	 1.createCheckFourccObj 
	 2.SetAcerType
	 3.CheckAcerFormate
	 4.destroyCheckFourccObj

	 结果由2 -3 决定

*/

void * __stdcall createCheckFourccObj(void);
BOOL __stdcall SetAcerType(int nAccelerateType, const wchar_t* pszDescript , void * pCheckFourccObj);
BOOL __stdcall CheckAcerFormate(DWORD& dwFileFourCC, DWORD& dwVideoFourCC, DWORD& dwAudioFourCC,void * pCheckFourccObj);
void __stdcall destroyCheckFourccObj(void * pCheckFourccObj);

//
//扩充函数暂时用于系统编解码替换
//
void * __stdcall createCheckFourccObjEx(void);
BOOL __stdcall SetAcerTypeEx(int nAccelerateType, const wchar_t* pszDescript , void * pCheckFourccObj);
BOOL __stdcall CheckAcerFormateEx(DWORD& dwFileFourCC, DWORD& dwVideoFourCC, DWORD& dwAudioFourCC,void * pCheckFourccObj);
void __stdcall destroyCheckFourccObjEx(void * pCheckFourccObj);

#ifdef __cplusplus
}
#endif