/**
@file TextToDib.h
@brief 创建文字水印
*/
#pragma once
#include <scombase.h>
#include "COMMacro.h"
#include "CommonInterface.h"
#include "TextDef.h"


/** 
@class 	 ITextItem 
@brief	 文本描述类.
*/

// {EDA25D65-A4EE-4d00-B2D0-B37DB619F145}
DEFINE_GUID(IID_ITextItem,0xeda25d65, 0xa4ee, 0x4d00, 0xb2, 0xd0, 0xb3, 0x7d, 0xb6, 0x19, 0xf1, 0x45);
EXTERN_C const IID IID_ITextItem;
MACRO_MIDL_INTERFACE(ITextItem, "EDA25D65-A4EE-4d00-B2D0-B37DB619F145")
: public IUnknown
{
public:

	/**
	@brief			设置文本内容
	@param textContent [in] :  文本内容
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetContent(const BSTR textContent) = 0;

	/**
	@brief			获取文本内容
	@param pContent [out] :  获取文本内容
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP GetContent(BSTR *pContent) const = 0;
	
	/**
	@brief			设置文本字体名
	@param strFontName [in] :  字体名
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetFontName(BSTR strFontName) = 0;

	/**
	@brief			获取文本字体名
	@param strFontName [out] :  获取字体名
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP GetFontName(BSTR *strFontName) const = 0;
	
	/**
	@brief			设置文本颜色
	@param crFont [in] :  字体颜色
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetFontColor(COLORREF crFont) = 0;

	/**
	@brief			获取文本颜色
	@return		COLORREF : 返回颜色值
	*/
	virtual STDMETHODIMP_(COLORREF) GetFontColor(void) const = 0;
	
	/**
	@brief			设置文本字体大小
	@param nFontSize [in] :  字体大小
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetFontSize(int nFontSize) = 0;

	/**
	@brief			获取文本字体大小
	@return		int : 返回字体大小
	*/
	virtual STDMETHODIMP_(int) GetFontSize(void)const = 0;
	
	/**
	@brief			设置文本字体样式值
	@param uStyle [in] :  字体类型
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetStyle(UINT uStyle) = 0;

	/**
	@brief			获取文本字体样式值
	@return		UINT : 返回字体样式值

	*/
	virtual STDMETHODIMP_(UINT) GetStyle(void) const = 0;
	
	/**
	@brief			设置文本字体是否使用阴影
	@param bShadow [in] :  是否使用阴影
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetUseShadow(int bShadow) = 0;

	/**
	@brief			获取文本字体是否使用阴影
	@return		int : 返回是否使用阴影
	*/
	virtual STDMETHODIMP_(int) GetUseShadow() const = 0;
	
	/**
	@brief			设置文本字体阴影样式
	@param paramShadow [in] :  阴影样式
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetShadowParam(SHADOWPARAM	paramShadow) = 0;

	/**
	@brief			获取文本字体是否使用阴影
	@return		int : 返回阴影样式
	*/
	virtual STDMETHODIMP_(SHADOWPARAM) GetShadowParam(void) const = 0;
	
	/**
	@brief			设置文本字体光晕值 与 光晕颜色
	@param nHalation [in] :  光晕值
	@param halationColor [in] :  光晕颜色
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetHalation(int nHalation, COLORREF halationColor) = 0;

	/**
	@brief			获取文本字体光晕值 与 光晕颜色
	@param pHalation [out] :  光晕值
	@param pHalationColor [out] :  光晕颜色
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP GetHalation(int *pHalation, COLORREF *pHalationColor) const = 0;
	
	/**
	@brief			设置文本字体旋转角度
	@param nAngle [in] :  旋转角度
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetAngle(int nAngle)=0;


	/**
	@brief			获取文本字体旋转角度
	@return		int : 返回文本字体旋转角度
	*/
	virtual STDMETHODIMP_(int) GetAngle() const = 0;
	
	/**
	@brief			设置文本字体在X与Y方向的缩放比例
	@param xScale [in] :  X方向缩放比例
	@param yScale [in] :  Y方向缩放比例
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP SetScale(int xScale, int yScale) = 0;

	/**
	@brief			获取文本字体在X与Y方向的缩放比例
	@param pXScale [out] :  X方向缩放比例
	@param pYScale [out] :  Y方向缩放比例
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP GetScale(int *pXScale, int *pYScale) const = 0;
};

/** 
@class 	 IPCText 
@brief	 文字水印创建相关的类.
*/

// {E8E0B4CB-9E9A-46f6-AA7D-FA547A3EA1F4}
DEFINE_GUID(IID_IPCText,0xe8e0b4cb, 0x9e9a, 0x46f6, 0xaa, 0x7d, 0xfa, 0x54, 0x7a, 0x3e, 0xa1, 0xf4);
EXTERN_C const IID IID_IPCText;
MACRO_MIDL_INTERFACE(IPCText, "E8E0B4CB-9E9A-46f6-AA7D-FA547A3EA1F4")
: public IUnknown
{
public:

	/**
	@brief			加载图像文件,并转换为32位的dibimage
	@param picName [in] : 图像文件名
	@return		IDibImage * : 返回 颜色位数为32位的IDibImage指针
	*/
	virtual STDMETHODIMP_(IDibImage *) LoadImage32B(BSTR picName) = 0;

	/**
	@brief			分配 ITextItem 对象空间
	@return		ITextItem * : 返回 ITextItem的指针
	*/
	virtual STDMETHODIMP_(ITextItem *) CreateTextSetting(void) = 0;

	/**
	@brief			获取系统所支持的字体数
	@return		int : 返回 系统所支持的字体数
	*/
	virtual STDMETHODIMP_(int) GetSupportFontCount(void) = 0;

	/**
	@brief			获取索引nIndex 对应的字体名
	@param nIndex [in]: 字体索引值
	@param pFontName [out]: 字体名
	@return		HRESULT : 函数调用状态
	- 1.S_OK    : 调用成功;
	- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP	   GetSupportFontName(int nIndex, BSTR *pFontName) = 0;

	/**
	@brief			根据 LPTEXTTRANSFORM 结构体信息，来创建 一张图片
	@param pItem [in]: 文本描述类指针
	@param lpTextTransform [in]: LPTEXTTRANSFORM结构体，定义字体位置等相关信息
	@param pImageTexture [in]: IDibImage指针，用于叠加
	@return		IDibImage : 指向创建的图片的指针
	*/
	virtual STDMETHODIMP_(IDibImage *) CreateNormalTextByTrans(const ITextItem *pItem,const LPTEXTTRANSFORM lpTextTransform ,IDibImage *pImageTexture = NULL) = 0;

	/**
	@brief			根据 设置图片 高宽等信息，来创建 一张图片
	@param pItem [in]: 文本描述类指针
	@param width [in]: 图片宽
	@param height [in]: 图片高
	@param pImageTexture [in]: IDibImage指针，用于叠加
	@return		IDibImage : 指向创建的图片的指针
	*/
	virtual STDMETHODIMP_(IDibImage *) CreateNormalTextBySize(const ITextItem *pItem,int width = 0, int height = 0, IDibImage *pImageTexture = NULL) = 0;

	/**
	@brief			获取系统所支持的艺术字体数
	@return		int : 返回 系统所支持的艺术字体数
	*/
	virtual STDMETHODIMP_(int) GetArtTextCount(void) = 0;

	/**
	@brief			获取nIndex对应艺术字相关信息
	@param nIndex [in]: 索引值
	@return		const LPARTTEXTINFO : 描述艺术字信息的结构体指针
	*/
	virtual STDMETHODIMP_(const LPARTTEXTINFO) GetArtTextInfo(int nIndex) = 0;

	/**
	@brief			获取pArtID对应艺术字的索引值
	@param pArtID [in]: 艺术字ID
	@return		int : 返回艺术字索引值
	*/
	virtual STDMETHODIMP_(int) GetIndexFromID(const BSTR * pArtID) = 0;
	
	/**
	@brief			根据 LPTEXTTRANSFORM 结构体信息，来创建 一张艺术字图片
	@param pArtItem [in]: 艺术字结构体指针
	@param lpTextTransform [in]: LPTEXTTRANSFORM结构体，定义字体位置等相关信息
	@return		IDibImage : 指向创建的图片的指针
	*/
	virtual STDMETHODIMP_(IDibImage *) GenerateArtTextByTrans(const LPARTTEXTITEM pArtItem, const LPTEXTTRANSFORM lpTextTransform) = 0; 

	/**
	@brief			根据 设置图片 高宽等信息，来创建 一张图片
	@param pArtItem [in]: 艺术字结构体指针
	@param width [in]: 图片宽
	@param height [in]: 图片高
	@return		IDibImage : 指向创建的图片的指针
	*/
	virtual STDMETHODIMP_(IDibImage *) GenerateArtTextBySize(const  LPARTTEXTITEM pArtItem, int width, int height)=0;

};

//定义组件对象
MIDL_DEFINE_GUID(CLSID, CLSID_CPCSText, 0xc3297339, 0x6fc4, 0x4b45, 0x81, 0xcc, 0x57, 0x8b, 0x39, 0xbf, 0xac, 0x45 );
EXTERN_C const CLSID CLSID_CPCSText;
// {c3297339-6fc4-4b45-81cc-578b39bfac45}
	
