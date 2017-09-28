//硬件编码检测相关头文件
//支持：cuda / intel / ati
#pragma once

#include "FourccInstrument.h"

#ifdef	WIN32

//操作系统类型
#define WIND_ME				1    // ME
#define WIND_98             2    // 98
#define WIND_95             3    // 95
#define WIND_2000           4    // 2000
#define WIND_XP             5    // XP
#define WIND_2003           6    // 2003
#define WIND_Vista          7    // Vista
#define WIND_2008           8    // server 2008
#define WIND_2008R2         9    // server 2008 rc2  
#define WIND_Win7           10   // Win7
#define WIND_UNK            0    // UNKNOWN

//操作系统位数
#define WIND_BITTYPE_AMD64  1    //64
#define WIND_BITTYPE_IA64	2    //64
#define WIND_BITTYPE_X86	3    //32


#else

#ifndef __stdcall
#define __stdcall 
#endif


#ifndef BOOL
#define BOOL int 
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

//************************************
// Method:    IsSupportCUDA
// FullName:  IsSupportCUDA
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 判断系统编码是否支持NVIDIA的硬件加速技术（CUDA），如果硬件支持，但编码不支持，则需要更新最新驱动，更新驱动不支持表示显卡该功能暂时被屏蔽
//************************************
BOOL __stdcall IsSupportCUDA();


//************************************
// Method:    IsNeedTwoNvcuvencDLL
// FullName:  IsNeedTwoNvcuvencDLL
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 判断nvidia显卡支持，但是驱动缺少库，由产品安装时写入
//************************************
BOOL __stdcall IsNeedTwoNvcuvencDLL(void);


//************************************
// Method:    IsSupportNvEncGpu
// FullName:  IsSupportNvEncGpu
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 判断nvidia显卡支持，但是驱动缺少库，由产品安装时写入
//************************************
BOOL __stdcall IsSupportNvEncGpu(void);


//************************************
// Method:    IsSupportCodecH264
// FullName:  IsSupportCodecH264
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 判断是否支持CUDA的h264编码加速
//************************************
BOOL __stdcall IsSupportEncoderH264();


//************************************
// Method:    IsSupportCodecVC1
// FullName:  IsSupportCodecVC1
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 是否支持CUDA的VC1编码加速
//************************************
BOOL __stdcall IsSupportEncoderVC1();

//************************************
// Method:    IsDeviceSupportCUDA
// FullName:  IsDeviceSupportCUDA
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 检测硬件是否支持CUDA的加速，如果硬件支持，用IsSupportCUDA检测编码是否支持
//************************************
#ifdef WIN32
BOOL __stdcall IsDeviceSupportCUDA();
#else
BOOL __stdcall IsDeviceSupportCUDA(int argc = 1, char * const argv[] = 0);	
#endif

//************************************
// Method:    IsATISupport
// FullName:  IsATISupport
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 检测是否支持ATI的加速 0, 不支持 1, 支持
BOOL __stdcall IsATISupport(void);

//************************************
// Method:    IsATIGpuDeviceSupport
// FullName:  IsATIGpuDeviceSupport
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 检测是否是ati显卡 0, 不是 1, 是
BOOL __stdcall IsATIGpuDeviceSupport(void);

BOOL __stdcall TempIsATIGpuDeviceSupport(void);
void __stdcall CrashIsATIGpuDeviceSupport(void);

//************************************
// Method:    IsIntelGPUEncodeSupport
// FullName:  IsIntelGPUEncodeSupport
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 检测是否支持Intel硬件编码加速 0, 不支持 1, 支持
BOOL __stdcall IsIntelGPUEncodeSupport(void);

//************************************
// Method:    IsIntelSupport
// FullName:  IsIntelSupport
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 检测是否支持Intel的加速 0, 不支持 1, 支持
BOOL __stdcall IsIntelSupport(void);

//************************************
// Method:    IsVCESupport
// FullName:  IsVCESupport
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 检测是否支持AMD VCE的加速 0, 不支持 1, 支持
BOOL __stdcall IsVCESupport(void);

//************************************
// Method:    GetWinOSVersion
// FullName:  GetWinOSVersion
// Access:    public 
// Returns:   int __stdcall
// Qualifier: 返回操作系统类型，返回值对应上面系统枚举值
//************************************
int __stdcall GetWinOSVersion();

//************************************
// Method:    GetWinOSBit
// FullName:  GetWinOSBit
// Access:    public 
// Returns:   int __stdcall
// Qualifier: 返回操作系统位数类型，返回值对应面位数类型枚举值
//************************************
int __stdcall GetWinOSBit();

//************************************
// Method:    IsDeviceSupportATI  
// FullName:  IsDeviceSupportATI
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 检测硬件是否支持ATI的加速，如果硬件支持，用IsDeviceSupportATI检测编码是否支持,仅仅用于 MAC OS
//************************************
BOOL  __stdcall  IsDeviceSupportATI(int argc, char * const argv[]);


//************************************
// Method:    AutoGetGpuCaps  
// FullName:  AutoGetGpuCaps
// Access:    public 
// Returns:   BOOL __stdcall
// Qualifier: 检测硬件是否支持GPU加速编码
//************************************
ACCELERATETYPE  __stdcall AutoGetGpuCaps(void);

#ifdef WIN32

//************************************
// Method:    IsIntelHw  
// FullName:  IsIntelHw
// Access:    public 
// Returns:   int __stdcall
// Qualifier: 检测INTEL当前硬件是否在支持GPU编码的白名单中
//************************************
int  __stdcall IsIntelHw(int *nIsInXml);

//************************************
// Method:    IsAtiAPP  
// FullName:  IsAtiAPP
// Access:    public 
// Returns:   int __stdcall
// Qualifier: 检测AMD当前硬件是否在支持GPU编码的白名单中
//************************************
int __stdcall  IsAtiAPP(int *nIsInXml);

//************************************
// Method:    IsNvidiaCuda  
// FullName:  IsNvidiaCuda
// Access:    public 
// Returns:   int __stdcall
// Qualifier: 检测NVIDIA当前硬件是否在支持GPU编码的白名单中
//************************************
int  __stdcall IsNvidiaCuda(int *nIsInXml);
//pdisplayCadName 外部分配
//返回值为1表示获取到显卡型号

//************************************
// Method:    IsNvidiaCuda  
// FullName:  IsNvidiaCuda
// Access:    public 
// Returns:   int __stdcall
// Qualifier: 检测NVIDIA当前硬件是否在支持GPU编码的白名单中
//************************************
int __stdcall  wsGetdisplaycardName(wchar_t pdisplayCadName[]);

#endif




#ifdef __cplusplus
}
#endif