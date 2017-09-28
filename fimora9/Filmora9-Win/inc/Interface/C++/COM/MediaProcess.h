/**
@file   MediaProcess.h
@brief  效果处理接口

	-# IVideoInput，IAudioInput，IVideoOutput，IAudioOutput接口用于设置视频流，音频流接口IMediaStream的输入以及获取输出。
	-# 除IVideoComposition接口外，其它接口都可以通过QueryInterface方法，查询这些接口。
	-# 若某接口支持音频的处理，那么可以查询出IAudioInput接口，该接口的SetAudioInput方法，用于设置音频流接口IMediaStream的输入， 
		对应的可以查询出IAudioOutput接口，该接口的GetAudioOutput方法，用于获取音频流接口IMediaStream的输出。
	-# 若支持视频数据处理，那么可以查询出IVideoInput接口，	该接口的SetVideoInput的方法，用于设置视频流接口的输入。对应的可以查询出IVideoOutput接口，
		该接口提供了GetVideoOutput方法，	用于获取视频流接口IMediaStream的输出。
*/

#pragma once
#include <scombase.h>
#include "CommonInterface.h"
#include "COMMacro.h"
#include "CodecDefine.h"
#include "DecodeParam.h"


/** 
@class 	 IAudioProcess 
@brief	 采样率以及声道数转换接口以及组件
*/
// {CDC6C251-70B0-42d0-B800-978AC93428A7}
DEFINE_GUID(IID_IAudioProcess,0xcdc6c251, 0x70b0, 0x42d0, 0xb8, 0x0, 0x97, 0x8a, 0xc9, 0x34, 0x28, 0xa7);
EXTERN_C const IID IID_IAudioProcess;
MACRO_MIDL_INTERFACE(IAudioProcess, "CDC6C251-70B0-42d0-B800-978AC93428A7")
: public IUnknown
{
public:

	/**
	@brief			设置采样率值
	@param nSampleRate [in] :  采样率
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetSampleRate(int nSampleRate) = 0;

	/**
	@brief			获取采样率值
	@return		int : 返回采样率值
	*/
	virtual STDMETHODIMP_(int) GetSampleRate(void) = 0;

	/**
	@brief			设置声道数值
	@param nChannels [in] :  声道数
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetChannels(int nChannels) = 0;

	/**
	@brief			获取采声道数值
	@return		int : 返回声道数值
	*/
	virtual STDMETHODIMP_(int) GetChannels(void) = 0;
};

//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CAudioProcess, 0xc1242906, 0x5995, 0x4ed4, 0xb1, 0xab, 0x9f, 0xae, 0xe1, 0xcf, 0xcc, 0xe7);
EXTERN_C const CLSID CLSID_CAudioProcess;

/** 
@class 	 IVolumeSetting 
@brief	 音量设置接口以及组件
*/

// {742F6246-6D45-4d96-9032-463749B59F67}
DEFINE_GUID(IID_IVolumeSetting,0x742f6246, 0x6d45, 0x4d96, 0x90, 0x32, 0x46, 0x37, 0x49, 0xb5, 0x9f, 0x67);
EXTERN_C const IID IID_IVolumeSetting;
MACRO_MIDL_INTERFACE(IVolumeSetting, "742F6246-6D45-4d96-9032-463749B59F67")
: public IUnknown
{
public:

	/**
	@brief			设置音量大小
	@param nVolume [in] :  音量值
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetVolume(int nVolume)=0;

	/**
	@brief			获取音量大小
	@return		int : 返回音量值
	*/
	virtual STDMETHODIMP_(int) GetVolume(void)=0;
};

//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CVolumeSetting, 0xe2737827, 0xe04a, 0x4969, 0xaa, 0xbb, 0xba, 0xdb, 0xd0, 0xcf, 0xe4, 0xf);
EXTERN_C const CLSID CLSID_CVolumeSetting;

												
/** 
@class 	 IAudioFade 
@brief	 淡入淡出接口以及组件
*/

// {E2EA8730-B88E-48a9-AE1E-09E9A5ECC99C}
DEFINE_GUID(IID_IAudioFade,0xe2ea8730, 0xb88e, 0x48a9, 0xae, 0x1e, 0x9, 0xe9, 0xa5, 0xec, 0xc9, 0x9c);
EXTERN_C const IID IID_IAudioFade;
MACRO_MIDL_INTERFACE(IAudioFade, "E2EA8730-B88E-48a9-AE1E-09E9A5ECC99C")
: public IUnknown
{
public:

	/**
	@brief			设置 淡入淡出 时间
	@param dFadeInStartTime [in] :  谈入开始时间
	@param dFadeInLength [in] : 淡入时间长度
	@param dFadeOutStartTime [in] :  谈出开始时间
	@param dFadeOutLength [in] : 淡出时间长度
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetFadeInOutTime(double dFadeInStartTime, double dFadeInLength, double dFadeOutStartTime, double dFadeOutLength) = 0;

	/**
	@brief			获得 淡入淡出 时间
	@param pFadeInStartTime [in] :  谈入开始时间
	@param pFadeInLength [in] : 淡入时间长度
	@param pFadeOutStartTime [in] :  谈出开始时间
	@param pFadeOutLength [in] : 淡出时间长度
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP GetFadeInOutTime(double *pFadeInStartTime, double *pFadeInLength, double *pFadeOutStartTime, double *pFadeOutLength) = 0;
};

//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CAudioFade, 0x68cce4e2, 0xe906, 0x47a8, 0x98, 0x20, 0xf7, 0x10, 0x89, 0xd, 0xa, 0x2a);
EXTERN_C const CLSID CLSID_CAudioFade;


/** 
@class 	 IAudioSlider 
@brief	 平滑音频输出接口以及组件   
*/
// {F7591170-F4C6-46ea-BA3B-6FF1C8942EEC}
DEFINE_GUID(IID_IAudioSlider,0xf7591170, 0xf4c6, 0x46ea, 0xba, 0x3b, 0x6f, 0xf1, 0xc8, 0x94, 0x2e, 0xec);
EXTERN_C const IID IID_IAudioSlider;
MACRO_MIDL_INTERFACE(IAudioSlider, "F7591170-F4C6-46ea-BA3B-6FF1C8942EEC")
: public IUnknown
{
public:

	/**
	@brief			设置音频对齐头
	@param bAlignHead [in] :  音频是否对其头
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	@note	该函数没有被使用
	*/
	virtual STDMETHODIMP SetAlignHead(int bAlignHead) = 0;

	/**
	@brief			获取音频对齐头
	@return		int : 返回音频对齐头
	@note	该函数没有被使用
	*/
	virtual STDMETHODIMP_(int) GetAlignHead(void) = 0;
};

//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CAudioSlider, 0x911b921b, 0xb28a, 0x44d1, 0xa0, 0xed, 0x4b, 0xde, 0xcf, 0x18, 0xa3, 0xbc);
EXTERN_C const CLSID CLSID_CAudioSlider;


/** 
@class 	 IVideoSlider 
@brief	 平滑视频输出接口以及组件
*/
// {52752A39-9BD7-4333-BA14-6982882255FB}
DEFINE_GUID(IID_IVideoSlider,0x52752a39, 0x9bd7, 0x4333, 0xba, 0x14, 0x69, 0x82, 0x88, 0x22, 0x55, 0xfb);
EXTERN_C const IID IID_IVideoSlider;
MACRO_MIDL_INTERFACE(IVideoSlider, "52752A39-9BD7-4333-BA14-6982882255FB")
: public IUnknown
{
public:

	/**
	@brief			设置视频输出帧率
	@param dFrameRate [in] :  视频输出帧率
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetOutputFrameRate(double dFrameRate) = 0;

	/**
	@brief			获取视频输出帧率
	@return		int : 返回视频输出帧率
	*/
	virtual STDMETHODIMP_(double) GetOutputFrameRate(void) = 0;

	/**
	@brief			设置视频对齐头
	@param bAlignHead [in] :  视频是否对其头
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	@note	该函数没有被使用
	*/
	virtual STDMETHODIMP SetAlignHead(int bAlignHead) = 0;
	
	/**
	@brief			获取视频对齐头
	@return		int : 返回视频对齐头
	@note	该函数没有被使用
	*/
	virtual STDMETHODIMP_(int) GetAlignHead(void) = 0;
};

//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CVideoSlider, 0x315ab12e, 0xabfb, 0x4327, 0xa3, 0x8a, 0x2b, 0xb, 0x88, 0x6d, 0xf8, 0x49);
EXTERN_C const CLSID CLSID_CVideoSlider;


/** 
@class 	 IVideoCrop 
@brief	 Crop处理接口以及组件 
*/
// {2081D016-BF75-4029-8E17-49A53C2F3541}
DEFINE_GUID(IID_IVideoCrop,0x2081d016, 0xbf75, 0x4029, 0x8e, 0x17, 0x49, 0xa5, 0x3c, 0x2f, 0x35, 0x41);
EXTERN_C const IID IID_IVideoCrop;
MACRO_MIDL_INTERFACE(IVideoCrop, "2081D016-BF75-4029-8E17-49A53C2F3541")
: public IUnknown
{
public:
	
	/**
	@brief			设置视频Crop范围
	@param cropRect [in] :  视频Crop范围
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetCropRect(const RECT cropRect) = 0;

	/**
	@brief			获取视频Crop范围
	@return		int : 返回视频Crop范围
	*/
	virtual STDMETHODIMP_(RECT) GetCropRect(void) = 0;
};

//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CVideoCrop, 0x465a9d1d, 0x55d6, 0x4bdd, 0x92, 0x32, 0x9b, 0x3, 0x5e, 0x79, 0x66, 0x67);
EXTERN_C const CLSID CLSID_CVideoCrop;


/** 
@class 	 IVideoEffect 
@brief	 明亮度对比度等效果处理接口以及组件
*/
// {AC3DDFA6-1974-42d5-880E-C0CEAA194D93}
DEFINE_GUID(IID_IVideoEffect,0xac3ddfa6, 0x1974, 0x42d5, 0x88, 0xe, 0xc0, 0xce, 0xaa, 0x19, 0x4d, 0x93);
EXTERN_C const IID  IID_IVideoEffect;
MACRO_MIDL_INTERFACE(IVideoEffect, "AC3DDFA6-1974-42d5-880E-C0CEAA194D93")
: public IUnknown
{
public:

	/**
	@brief			设置视频Filter使用类型
	@param uFlag [in] :  Filter使用类型，是flag的组合值
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	@note
		- ##define EX_FILTER_GRAY			0x0001	灰色
		- ##define EX_FILTER_EMBOSS			0x0002	浮雕
		- ##define EX_FILTER_NEGATIVE		0x0004	负片
		- ##define EX_FILTER_BLACK_WHITE		0x0008	黑白
		- ##define EX_FILTER_WHITE_BLACK		0x0010	白黑
		- ##define EX_FILTER_OLD_PHOTO		0x0020	老照片
	*/
    virtual STDMETHODIMP SetFilterFlag(UINT uFlag) = 0;

	/**
	@brief			获取视频Filter使用类型
	@return		UINT : 返回视频Filter使用类型
	*/
    virtual STDMETHODIMP_(UINT) GetFilterFlag(void) = 0;

	/**
	@brief			设置视频亮度
	@param nValue [in] :  视频亮度  [-100,100]
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetBrightness(int nValue) = 0;

	/**
	@brief			获取视频亮度 
	@return		int : 返回视频亮度
	*/
	virtual STDMETHODIMP_(int) GetBrightness(void) = 0;
	
	/**
	@brief			设置视频饱和度
	@param nValue [in] :  视频饱和度 [-100,100]
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetSaturation(int nValue) = 0;

	/**
	@brief			获取视频饱和度 
	@return		int : 返回视频饱和度
	*/
	virtual STDMETHODIMP_(int) GetSaturation(void) = 0;
	
	/**
	@brief			设置视频交织样式
	@param uDeintelace [in] :  视频交织样式
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	@note
	enum DEINTERLACE_STYLE \n
	{\n
		DIS_NONE,				不做处理\n
		DIS_ODD,				重复奇数行\n	
		DIS_EVEN,				重复偶数行	\n
		DIS_ODD_INTERPOLATE,	使用奇数行插值\n
		DIS_EVEN_INTERPOLATE,	使用偶数行插值\n
		DIS_AUTO,				自动处理（暂不支持）\n
	}\n
};
	
	*/
	virtual STDMETHODIMP SetDeintelace(DEINTERLACE_STYLE uDeintelace) = 0;

	/**
	@brief			获取视频交织样式
	@return		DEINTERLACE_STYLE : 返回视频交织样式
	*/
	virtual STDMETHODIMP_(DEINTERLACE_STYLE) GetDeintelace(void) = 0;
	
	/**
	@brief			设置视频对比度
	@param nValue [in] :  视频对比度 [-100,100]
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	
	virtual STDMETHODIMP SetContrast(int nValue) = 0;

	/**
	@brief			获取视频对比度
	@return		DEINTERLACE_STYLE : 返回视频对比度
	*/
	virtual STDMETHODIMP_(int) GetContrast(void) = 0;
	
};

//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CVideoEffect, 0x9f97a1bf, 0x884d, 0x4c14, 0xb9, 0xcc, 0x4d, 0xa7, 0xcd, 0xf3, 0x5b, 0xc7);
EXTERN_C const CLSID CLSID_CVideoEffect;

/** 
@class 	 IVideoCopy 
@brief	 数据拷贝的接口
*/
// {C2B0EEA2-6E70-45df-A06B-B096146EFB56}
DEFINE_GUID(IID_IVideoCopy,0xc2b0eea2, 0x6e70, 0x45df, 0xa0, 0x6b, 0xb0, 0x96, 0x14, 0x6e, 0xfb, 0x56);
EXTERN_C const IID  IID_IVideoCopy;
MACRO_MIDL_INTERFACE(IVideoCopy, "C2B0EEA2-6E70-45df-A06B-B096146EFB56")
: public IUnknown
{
public:

	/**
	@brief			设置视频帧是否需要COPY
	@param bNeed [in] :  是否需要copy视频帧标志
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetNeedCopyFrame(int bNeed) = 0;

	/**
	@brief			获取视频帧是否需要COPY
	@return		int : 返回是否需要copy视频帧标志
	*/
	virtual STDMETHODIMP_(int) GetNeedCopyFrame() = 0;
};

//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CVideoCopy, 0xc2aa0e9e, 0x3e1a, 0x48a9, 0xb2, 0x88, 0x99, 0xa5, 0x9d, 0xa0, 0xcb, 0x9d);
EXTERN_C const CLSID CLSID_CVideoCopy;

/** 
@class 	 IVideoRotation 
@brief	 视频旋转等效果处理接口以及组件 
*/

// {37300932-932C-42a6-852F-B3C9CFD84C83}
DEFINE_GUID(IID_IVideoRotation,0x37300932, 0x932c, 0x42a6, 0x85, 0x2f, 0xb3, 0xc9, 0xcf, 0xd8, 0x4c, 0x83);
EXTERN_C const IID IID_IVideoRotation;
MACRO_MIDL_INTERFACE(IVideoRotation, "37300932-932C-42a6-852F-B3C9CFD84C83")
: public IUnknown
{
public:

	/**
	@brief			图像翻转设置
	@param uFlag [in] :  是否图像翻转,一个flag运算组合值
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	@note :
			 - ##define FLIP_HORIZONTAL			0x0001  水平翻转
			 - ##define FLIP_VERTICAL			0x0002	垂直翻转
	*/
	virtual STDMETHODIMP SetFlipFlag(UINT uFlag)=0;

	/**
	@brief			获取图像翻转设置
	@return		UINT : 返回图像翻转设置
	*/
	virtual STDMETHODIMP_(UINT) GetFlipFlag()=0;


	/**
	@brief			图像旋转设置
	@param nRotation [in] :  图像翻转角度，仅仅90的整数倍
	@param nDrag [in] :  未使用，填 0
	@param flg [in] :  未使用，填 0
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetRotation(int nRotation, int nDrag, int flg) = 0;

	/**
	@brief			获取图像旋转设置
	@param pRotation [in] :  图像翻转角度
	@param pDrag [in] :  未使用，填 0
	@param pFlg [in] :	未使用，填 0
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP GetRotation(int *pRotation, int *pDrag, int *pFlg) = 0;
};

//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CVideoRotation, 0x807dea6d, 0x2e28, 0x41b4, 0x89, 0xa8, 0x2d, 0x33, 0xc, 0xba, 0x4d, 0xfc);
EXTERN_C const CLSID CLSID_CVideoRotation;

/** 
@class 	 IVideoResizer 
@brief	 视频缩放处理接口以及组件 
*/

// {D7F70D4F-AAD6-4d92-9CE4-B95641E2D865}
DEFINE_GUID(IID_IVideoResizer,0xd7f70d4f, 0xaad6, 0x4d92, 0x9c, 0xe4, 0xb9, 0x56, 0x41, 0xe2, 0xd8, 0x65);
EXTERN_C const IID IID_IVideoResizer;
MACRO_MIDL_INTERFACE(IVideoResizer, "D7F70D4F-AAD6-4d92-9CE4-B95641E2D865")
: public IUnknown
{
public:
	
	/**
	@brief			设置输出图像大小
	@param nWidth [in] :  图像宽
	@param nHeight [in] : 图像高
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetOutputSize(int nWidth, int nHeight) = 0;
	
	/**
	@brief			获取输出图像大小
	@param pWidth [in] :  图像宽
	@param pHeight [in] : 图像高
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP GetOutputSize(int *pWidth, int *pHeight) = 0;

	/**
	@brief			设置图像缩放时，插值算法
	@param interpolationMode [in] :  图象插值算法
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	@see	#IMAGE_INTERPOLATION	
	*/
	virtual STDMETHODIMP SetInterpolationMode(IMAGE_INTERPOLATION interpolationMode) = 0;
	
	/**
	@brief			获取图像缩放时，插值算法
	@return IMAGE_INTERPOLATION :  图象插值算法
	*/
	virtual STDMETHODIMP_(IMAGE_INTERPOLATION) GetInterpolationMode(void) = 0;

	/**
	@brief			设置图像缩放时所使用模型
	@param resizeMode [in] :  缩放时所使用模型
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	@see	#IMAGE_RESIZE_METHOD
	*/
	virtual STDMETHODIMP SetResizeMode(IMAGE_RESIZE_METHOD resizeMode) = 0;

	/**
	@brief			获取图像缩放时所使用模型
	@return IMAGE_RESIZE_METHOD :  图像缩放时所使用模型
	*/
	virtual STDMETHODIMP_(IMAGE_RESIZE_METHOD) GetResizeMode(void) = 0;
};


//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CVideoResizer, 0xfc2c8098, 0x6c82, 0x492a, 0x92, 0x6c, 0xcb, 0xb9, 0x64, 0xd1, 0x2d, 0x3f);
EXTERN_C const CLSID CLSID_CVideoResizer;




/************************************************************************/
/*						视频叠加接口以及组件						    */
/************************************************************************/

/*!
* \class IVideoCompositonItem
* \brief 视频叠项设置接口。 
*/

// {7060C309-9996-49ff-B5E3-74093A6ABB98}
DEFINE_GUID(IID_IVideoCompositonItem,0x7060c309, 0x9996, 0x49ff, 0xb5, 0xe3, 0x74, 0x9, 0x3a, 0x6a, 0xbb, 0x98);
EXTERN_C const IID IID_IVideoCompositonItem;
MACRO_MIDL_INTERFACE(IVideoCompositonItem, "7060C309-9996-49ff-B5E3-74093A6ABB98")
: public IUnknown
{
public:

	/*! \fn  virtual STDMETHODIMP SetRect(const FRECT Rect))
	* \brief  设置叠加区域
	* \FRECT [in] 叠加区域
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetRect(const FRECT Rect) = 0;

	/*! \fn  virtual STDMETHODIMP_(FRECT) GetRect(void)
	* \brief  获取当前叠加区域
	* \retval 当前叠加区域
	*/
	virtual STDMETHODIMP_(FRECT) GetRect(void) = 0;

	/*! \fn  virtual STDMETHODIMP SetOpacity(int nOpacity)
	* \brief  设置透明度
	* \param nOpacity [in] 透明度
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetOpacity(int nOpacity) = 0;

	/*! \fn  virtual STDMETHODIMP_(int) GetOpacity(void)
	* \brief  获取当前的透明度设置
	* \retval 当前的透明度设置
	*/
	virtual STDMETHODIMP_(int) GetOpacity(void) = 0;

	/*! \fn  virtual STDMETHODIMP SetStartTime(__int64 startTime)
	* \brief 设置叠加的开始时间
	* \param startTime [in] 叠加的开始时间
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetStartTime(__int64 startTime) = 0;

	/*! \fn  virtual STDMETHODIMP_(__int64) GetStartTime(void)
	* \brief 获取叠加的开始时间
	* \retval 叠加的开始时间
	*/
	virtual STDMETHODIMP_(__int64) GetStartTime(void) = 0;

	/*! \fn  virtual STDMETHODIMP SetDuration(__int64 duration)
	* \brief 获取叠加的持续时长
	* \param duration [in] 叠加的持续时长
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetDuration(__int64 duration) = 0;

	/*! \fn  virtual STDMETHODIMP_(__int64) GetDuration(void)
	* \brief 获取当前设置的叠加持续时长
	* \retval 叠加持续时长
	*/
	virtual STDMETHODIMP_(__int64) GetDuration(void) = 0;
};

/*!
* \class IVideoCompositor
* \brief 视频叠加接口，通过该接口，可以叠加视频流，文字，以及图像。 
*/

// {A32480A3-582E-4785-AA98-B46B2C317252}
DEFINE_GUID(IID_IVideoCompositor,0xa32480a3, 0x582e, 0x4785, 0xaa, 0x98, 0xb4, 0x6b, 0x2c, 0x31, 0x72, 0x52);
EXTERN_C const IID IID_IVideoCompositor;
MACRO_MIDL_INTERFACE(IVideoCompositor, "A32480A3-582E-4785-AA98-B46B2C317252")
: public IUnknown
{
public:

	/*! \fn  virtual STDMETHODIMP_(IVideoCompositonItem *) AddByMediaStream(IMediaStream * pMediaStream)
	* \brief  添加一路视频流， 用于叠加通过IVideoInput 设置进的视频流。
	* \pMediaStream [in] 视频流
	* \retval 叠加设置接口
	* \see IVideoCompositonItem IMediaStream
	*/
	virtual STDMETHODIMP_(IVideoCompositonItem *) AddByMediaStream(IMediaStream * pMediaStream) = 0;

	/*! \fn  virtual STDMETHODIMP_(IVideoCompositonItem *) AddByFileName(const BSTR pFileName)
	* \brief  添加一个外挂字幕的路径， 用于叠加通过IVideoInput 设置进的视频流。
	* \pFileName [in] 外挂字幕的路径
	* \retval 叠加设置接口
	* \see IVideoCompositonItem
	*/
	virtual STDMETHODIMP_(IVideoCompositonItem *) AddByFileName(const BSTR pFileName) = 0;

	/*! \fn  virtual STDMETHODIMP_(IVideoCompositonItem *) AddByBitmap(IDibImage * pDIBImage)
	* \brief  添加位图接口， 用于叠加通过IVideoInput 设置进的视频流。
	* \pDIBImage [in] 位图接口
	* \retval 叠加设置接口
	* \see IVideoCompositonItem IDibImage
	*/
	virtual STDMETHODIMP_(IVideoCompositonItem *) AddByBitmap(IDibImage * pDIBImage) = 0;

	/*! \fn  virtual STDMETHODIMP_(int) GetCount(void)
	* \brief  获取添加的叠加项的数量
	* \retval 添加的叠加项的数量
	*/
	virtual STDMETHODIMP_(int) GetCount(void) = 0;

	/*! \fn  virtual STDMETHODIMP GetItem(int nIndex, IVideoCompositonItem **ppVideoCompositonItem)
	* \brief  获取指定的叠加项接口，
    * \param nIndex [in] 叠加索引号
	* \param ppVideoCompositonItem [in / out] 叠加项接口指针
	* \retval 获取结果
	*/
	virtual STDMETHODIMP GetItem(int nIndex, IVideoCompositonItem **ppVideoCompositonItem) = 0;

	/*! \fn  virtual STDMETHODIMP Remove(IVideoCompositonItem* pVideoCompositonItem, int * pIndex)
	* \brief  移除一个叠加项接口，并返回该项的索引号
	* \param pVideoCompositonItem [in] 叠加项接口
	* \param pIndex [in / out] 该接口在列表中的索引号
	* \retval 获取结果
	*/
	virtual STDMETHODIMP Remove(IVideoCompositonItem* pVideoCompositonItem, int * pIndex) = 0;
};

//定义组件
// {375DAF31-A4B7-40b0-9F2B-3B90AC944536}
MIDL_DEFINE_GUID(CLSID, CLSID_CVideoCompositor, 0x375daf31, 0xa4b7, 0x40b0, 0x9f, 0x2b, 0x3b, 0x90, 0xac, 0x94, 0x45, 0x36);
EXTERN_C const CLSID CLSID_CVideoCompositor;


/** 
@class 	 IAudioPackFrame 
@brief	 音频分帧处理接口以及组件 
*/
// {F289BB70-0986-4b23-8F34-A870F1F83D0F}
DEFINE_GUID(IID_IAudioPackFrame, 0xf289bb70, 0x986, 0x4b23, 0x8f, 0x34, 0xa8, 0x70, 0xf1, 0xf8, 0x3d, 0xf);
EXTERN_C const IID IID_IAudioPackFrame;
MACRO_MIDL_INTERFACE(IAudioPackFrame, "F289BB70-0986-4b23-8F34-A870F1F83D0F")
: public IUnknown
{
public:

	/*
	设置 Forward() 音频数据输出间隔，以毫秒为单位
	*/
	virtual STDMETHODIMP SetOutPutInterval( double millisecond ) = 0;

};

//定义组件
MIDL_DEFINE_GUID(CLSID, CLSID_CAudioPackFrame, 0xb26ee074, 0xd0fc, 0x41d9, 0x9b, 0x63, 0xb2, 0xda, 0xa8, 0x4b, 0x80, 0x58);
EXTERN_C const CLSID CLSID_CAudioPackFrame;