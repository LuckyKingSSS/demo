#ifndef NLECOMERROR_HEADER
#define NLECOMERROR_HEADER

#ifdef WIN32
#include <windows.h>
#else
#include "objbase.hpp"
#endif

///------------------------------------------------------------------------------------------
#define NLE_S_BASE_ADDRESS  0x00060000L

///空白片断
#define S_NLE_NO_RENDER		NLE_S_BASE_ADDRESS + 1 //0x00060001


///------------------------------------------------------------------------------------------
#define NLE_E_BASE_ADDRESS  0x80060000L

#define E_NLE_NOT_SET								_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 1)

#define E_NLE_DISP_E_DIVBYZERO						_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 2)

///系统异常
#define E_NLE_UNEXPECTED							_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 3)

///状态异常
#define E_NLE_NOT_VALID_STATE						_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 4)

///已经存在
#define E_NLE_ALLREADY_EXISTS						_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 5)

///属性拒绝访问:不允许获取或设置
#define E_NLE_ACCESS_DENIED							_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 6)

///没有父对象，例如:Clip设置Level时必须要有父对象
#define E_NLE_NO_PARENT								_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 7)

///不支持的文件
#define E_NLE_FILE_NOT_SURPPORT						_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 8)

///IPP库错误
#define E_NLE_IPP_ERROR								_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 9)

///已到达视频文件结尾
#define E_NLE_MEDIA_END								_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 10)

///特效引擎异常
#define E_NLE_EFFECT_ENGINE							_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 11)

///文件不存在
#define E_NLE_FILE_NOT_FOUND						_HRESULT_TYPEDEF_(NLE_E_BASE_ADDRESS + 12)

#endif

