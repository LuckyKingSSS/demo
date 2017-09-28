#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>      
#include "NLECaptionDefine.h"

#ifndef _NLE_TEXT_PROPERTY_TYPE_
#define _NLE_TEXT_PROPERTY_TYPE_


typedef HRESULT(__stdcall *TEXTCHANGE_CALLBACK)(void* pUserObj, wchar_t* str, int nCaretRow, int nCaretCol);

// {80B8CC7F-B73F-428d-A7C0-152105D75F15}
DEFINE_GUID(CLSID_INLECaptionTextObj,
	0x80b8cc7f, 0xb73f, 0x428d, 0xa7, 0xc0, 0x15, 0x21, 0x5, 0xd7, 0x5f, 0x15);

// {13B24A70-AD13-4b21-8B18-7AC24F6E9454}
DEFINE_GUID(IID_INLECaptionTextObj,
	0x13b24a70, 0xad13, 0x4b21, 0x8b, 0x18, 0x7a, 0xc2, 0x4f, 0x6e, 0x94, 0x54);


class INLECaptionTextObj :public IUnknown
{
public:
	/**
	*@brief     设置文本字符串
	*@param strText  字符串 
	*@return
	*/
	virtual bool __stdcall SetText(BSTR strText) = 0;
	/**
	*@brief     设置FONT
	*@param  Font相关参数
	*@return
	*/
	virtual bool __stdcall SetFont(const NLEFont* pFont) = 0;
	/**
	*@brief     获取FONT
	*@param  Font相关参数
	*@return
	*/
	virtual bool __stdcall GetFont(NLEFont* pFont) = 0;
	/**
	*@brief     设置FontName
	*@param  strFontName   FontName
	*@return
	*/
	virtual bool __stdcall SetFontName(BSTR strFontName) = 0;
	/**
	*@brief     设置FontSize
	*@param  nSize   大小
	*@return
	*/
	virtual bool __stdcall SetFontSize(int nSize) = 0;
	/**
	*@brief     设置FontBold
	*@param  nBold 是否加粗
	*@return
	*/
	virtual bool __stdcall SetFontBold(int nBold) = 0;
	/**
	*@brief     设置FontItalic
	*@param  nItalic 是否斜体
	*@return
	*/
	virtual bool __stdcall SetFontItalic(int nItalic) = 0;
	/**
	*@brief     设置Shadow
	*@param  Shadow相关参数
	*@return
	*/
	virtual bool __stdcall SetShadow(const NLEShadow* pShadow) = 0;
	/**
	*@brief     获取Shadow
	*@param  Shadow相关参数
	*@return
	*/
	virtual bool __stdcall GetShadow(NLEShadow* pShadow) = 0;
	/**
	*@brief     设置是否启用Shadow
	*@param  nEnable  是否启用
	*@return
	*/
	virtual bool __stdcall SetShadowEnable(int nEnable) = 0;
	/**
	*@brief     设置Shadow颜色
	*@param  nColor  颜色值
	*@return
	*/
	virtual bool __stdcall SetShadowColor(DWORD nColor) = 0;
	/**
	*@brief     设置Shadow透明度
	*@param  nAlpha  透明度
	*@return
	*/
	virtual bool __stdcall SetShadowAlpha(int nAlpha) = 0;
	/**
	*@brief     设置Shadow模糊度
	*@param  nBlurRadius  模糊度
	*@return
	*/
	virtual bool __stdcall SetShadowBlurRadius(int nBlurRadius) = 0;
	/**
	*@brief     设置Shadow 距离
	*@param  nDistance  距离
	*@return
	*/
	virtual bool __stdcall SetShadowDistance(int nDistance) = 0;
	/**
	*@brief     设置Shadow 方向
	*@param  nDirection  方向
	*@return
	*/
	virtual bool __stdcall SetShadowDirection(int nDirection) = 0;
	/**
	*@brief     设置Border
	*@param  Border相关参数
	*@return
	*/
	virtual bool __stdcall SetBorder(const NLEBorder* pBorder) = 0;
	/**
	*@brief     获取Border
	*@param  Border相关参数
	*@return
	*/
	virtual bool __stdcall GetBorder(NLEBorder* pBorder) = 0;
	/**
	*@brief     设置是否启用Border
	*@param  nEnable 是否启用
	*@return
	*/
	virtual bool __stdcall SetBorderEnable(int nEnable) = 0;
	/**
	*@brief     设置Border大小
	*@param  nSize 大小
	*@return
	*/
	virtual bool __stdcall SetBorderSize(int nSize) = 0;
	/**
	*@brief     设置Border Color1
	*@param  Color1  颜色值
	*@return
	*/
	virtual bool __stdcall SetBorderColor1(DWORD Color1) = 0;
	/**
	*@brief     设置Border 透明度
	*@param  nAlpha  透明度
	*@return
	*/
	virtual bool __stdcall SetBorderAlpha(int nAlpha) = 0;
	/**
	*@brief     设置Border 模糊度
	*@param  nBlurRadius  模糊度
	*@return
	*/
	virtual bool __stdcall SetBorderBlurRadius(int nBlurRadius) = 0;
	/**
	*@brief     设置Face
	*@param Face相关参数
	*@return
	*/
	virtual bool __stdcall SetFace(const NLEFace* pFace) = 0;
	/**
	*@brief     获取Face
	*@param Face相关参数
	*@return
	*/
	virtual bool __stdcall GetFace(NLEFace* pFace) = 0;
	/**
	*@brief    是否启用Face
	*@param nEnable 是否启用
	*@return
	*/
	virtual bool __stdcall SetFaceEnable(int nEnable) = 0;
	/**
	*@brief    Face Color
	*@param Color 颜色值
	*@return
	*/
	virtual bool __stdcall SetFaceColor(DWORD Color) = 0;
	/**
	*@brief    Face 透明度
	*@param nAlpha 透明度
	*@return
	*/
	virtual bool __stdcall SetFaceAlpha(int nAlpha) = 0;
	/**
	*@brief    Face 模糊度
	*@param nBlurRadius 模糊
	*@return
	*/
	virtual bool __stdcall SetFaceBlurRadius(int nBlurRadius) = 0;
	/**
	*@brief    Face 效果
	*@param nEffect 效果，DType有提供一些表面渲染效果，通过此参数设置表面渲染效果
	*@return
	*/
	virtual bool __stdcall SetFaceEffect(int nEffect) = 0;
	/**
	*@brief     设置Face 填充模式
	*@param  nFillType 纯色，渐变，纹理
	*@return
	*/
	virtual bool __stdcall SetFillType(NLEFaceFillType nFillType) = 0;
	/**
	*@brief     获取纹理填充信息
	*@param  pTexture 纹理填充信息
	*@return
	*/
	virtual bool __stdcall GetTexture(NLETexture* pTexture) = 0;
	/**
	*@brief     设置纹理路径
	*@param  strFilePath 纹理路径
	*@return
	*/
	virtual bool __stdcall SetTexturePath(BSTR strFilePath) = 0;

	/**
	*@brief     获取渐变填充信息
	*@param  pGradient 渐变信息
	*@return
	*/
	virtual bool __stdcall GetGradient(NLEGradient* pGradient) = 0;
	/**
	*@brief     设置渐变起始颜色
	*@param  Color1 起始颜色值
	*@return
	*/
	virtual bool __stdcall SetGradientColor1(DWORD Color1) = 0;
	/**
	*@brief     设置渐变终止颜色
	*@param  Color2 终止颜色值
	*@return
	*/
	virtual bool __stdcall SetGradientColor2(DWORD Color2) = 0;
	/**
	*@brief     设置渐变方向
	*@param  nGradType 渐变方向
	*@return
	*/
	virtual bool __stdcall SetGradientGradType(int nGradType) = 0;

	/**
	*@brief     设置行距
	*@param  nLineSpace 行距
	*@return
	*/
	virtual bool __stdcall SetLineSpace(float nLineSpace) = 0;
	/**
	*@brief     获取行距
	*@param  nLineSpace 行距
	*@return
	*/
	virtual bool __stdcall GetLineSpace(float* pnLineSpace) = 0;
	/**
	*@brief     设置字距
	*@param  nCharSpace 字距
	*@return
	*/
	virtual bool __stdcall SetCharSpace(float nCharSpace) = 0;
	/**
	*@brief     获取字距
	*@param  pnCharSpace 字距
	*@return
	*/
	virtual bool __stdcall GetCharSpace(float* pnCharSpace) = 0;
	/**
	*@brief     设置文字对其模式
	*@param  nAlignType 对齐模式
	*@return
	*/
	virtual bool __stdcall SetAlignType(NLEAlignType nAlignType) = 0;
	/**
	*@brief     获取文字对其模式
	*@param  nAlignType 对齐模式
	*@return
	*/
	virtual bool __stdcall GetAlignType(NLEAlignType* nAlignType) = 0;
	/**
	*@brief     设置文字动画信息
	*@param animParam 动画配置结构
	*@return
	*/
	virtual bool __stdcall  AddAnimation(NLEAnimationConfig* animParam) = 0;


	virtual bool __stdcall SetAnimationFile(BSTR animationFilePath) = 0;
	/**
	*@brief    选中全部文字 
	*@return
	*/
	virtual bool __stdcall SelectAllChar() = 0;
	/**
	*@brief    文本变化通知回调函数
	*@param pCallBack 回调函数
	*@param pUserObj 识别指针
	*@return
	*/

	virtual bool __stdcall SetCharSelect(int nStart, int nLength, int bStartPos) = 0;

	virtual HRESULT __stdcall SetTextChangeCallBack(TEXTCHANGE_CALLBACK pCallBack, void *pUserObj) = 0;

};

#endif