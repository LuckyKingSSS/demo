#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLEBitmap.h"
#include "NLEGraphics.h"

//{372E55D4-765A-D971-3763-789091ACFEDC}
DEFINE_GUID(CLSID_NLEImageProc, 0x372E55D4, 0x765A, 0xD971, 0x37, 0x63, 0x78, 0x90, 0x91, 0xAC, 0xFE, 0xDC);                   
//{2B2E53CA-BA16-B9EC-A608-2C84E7E758B1}
DEFINE_GUID(IID_INLEImageProc, 0x2B2E53CA, 0xBA16, 0xB9EC, 0xA6, 0x08, 0x2C, 0x84, 0xE7, 0xE7, 0x58, 0xB1);          
///见SDK\Both\ipp\Doc\ippiman.pdf P213-P214 或 https://software.intel.com/en-us/node/503865

typedef enum 
{
	/**
	 *@brief OVER A occludes B
	 *-		color : alphaA * A + (1 - alphaA) * alphaB * B
	 *-		alpha : alphaA + (1-alphaA) * alphaB 
	 */
	AlphaType_ippAlphaOver, 

	/**@brief IN A within B. A acts as a matte for B. A shows only where B is visible. 
	*-		color: alphaA * A * alphaB 
	*-		alpha: alphaA * alphaB
	*/
	AlphaType_ippAlphaIn,

	/**
	 *@brief OUT A outside B. NOT-B acts as a matte for A. A shows only where B is not visible.
	 *-		color: alphaA * A *(1 - alphaB) 
	 *-		alpha: alphaA * (1 - alphaB) 
	 */
	AlphaType_ippAlphaOut,
	/**
	 *@brief ATOP Combination of (A IN B) and (B OUT A). B is both back-ground and matte for A.
	 *-		color: alphaA * A * alphaB+(1-alphaA) * alphaB * B 
	 *-		alpha: alphaA * alphaB + (1-alphaA ) * alphaB
	 */
	AlphaType_ippAlphaATop,
	/**
	 *@brief XOR Combination of (A OUT B) and (B OUT A). A and B mutually exclude each other.
	 *-		color: alphaA * A * (1-alphaB) + ( 1 - alphaA ) * alphaB * B 
	 *-		alpha: alphaA * (1-alphaB)+(1-alphaA) * alphaB
	 */
	AlphaType_ippAlphaXor,
	/**
	 *@brief PLUS 
	 *-		color: alphaA * A + alphaB * B 
	 *-		alpha: alphaA + alphaB
	 */
	AlphaType_ippAlphaPlus,

	AlphaType_ippAlphaOverPremul,
	AlphaType_ippAlphaInPremul,
	AlphaType_ippAlphaOutPremul,
	AlphaType_ippAlphaATopPremul,
	AlphaType_ippAlphaXorPremul,
	AlphaType_ippAlphaPlusPremul
} AlphaType;

typedef enum
{
	///直接将源填满目标区域{不一定保持源比例}
	ResizeType_Stretch,
	///拉远镜头，源全部显示在目标区域内{保持源比例}
	ResizeType_Scale_ZOOMOUT,
	///拉近镜头，填满目标区域{保持源比例}
	ResizeType_Scale_ZOOMIN,

	///ResizeType_xxx
}ResizeType;

typedef float NLEAngleValue;

typedef enum
{
	MirrorType_Vertical,
	MirrorType_Horizontal,
	MirrorType_Both
}MirrorType;

class INLEImageProc :public IUnknown
{
public:
	
	/**
	*@brief  	透明度混合
	*@param	hBackImage 底图
	*@param	hOverImage 前景图
	*@param	x 底层位置
	*@param	y 底层位置
	*@param	cx 混合大小
	*@param	cy 混合大小
	*@param hImageTemp 底层的复本，相当于临时内存，其长宽必须与hBackImage一致，用于内部算法优化，不为NULL时性能显著提高
	*@return 	HRESULT __stdcall
	*@note		前景图的起点为(0,0)位置; 若hBackCopy不为NULL时性能明显提升
	*/
	virtual HRESULT __stdcall AlphaComp(INLEImage *hBackImage, INLEImage * hOverImage,
		int x, int y, int cx = -1, int cy = -1, INLEImage* hImageTemp = NULL) = 0;

	/**
	 *@brief  	AlphaComp透明度混合
	 *@brief	将ImgA的区域(xA,yA, nWidth, nHeight)与ImgB的区域(xB,yB, nWidth, nHeight)区域混合，结果放到pImgC的(0,0,nWidth,nHeight)中
	 *@author	wangyx
	 *@param	pImgA [in] 源图A
	 *@param	pImgB [in] 源图B
	 *@param	pImgC [out] 输出图C
	 *@param	type 混合类型，见AlphaType_xxx	
	 *@param	xB 源图B的起始点Y坐标，注意范围>=0
	 *@param	yB 源图B的起始点Y坐标，注意范围>=0
	 *@param	xA 源图A的起始点X坐标，注意范围>=0
	 *@param	yA 源图A的起始点Y坐标，注意范围>=0
	 *@param	xC 源图C的起始点X坐标，注意范围>=0
	 *@param	yC 源图C的起始点Y坐标，注意范围>=0
	 *@param	nWidth 混合最大宽度，默认时为源图B宽度
	 *@param	nHeight  混合最大宽度，默认时为源图B长度
	 *@return 	HRESULT	 
	 *@note		1.若输出尺寸图大于混合尺寸，超出范围部分保持C图原像素值不变
	 *			2.pImgA,pImgB,pImgC值可相同，但相同情况下，运行效率会下降（约%75)，不建议这样做
	 */
	virtual HRESULT __stdcall AlphaComp(INLEImage* pImgB, INLEImage* pImgA, INLEImage* pImgC,
								AlphaType type,
								unsigned int xB = 0, unsigned int yB = 0,
								unsigned int xA = 0, unsigned int yA = 0,
								unsigned int xC = 0, unsigned int yC = 0,
								unsigned int nWidth = -1, unsigned int nHeight = -1) = 0;
	/**
	 *@brief  	将源图hSrc拉伸到目标图hDst大小
	 *@param	hSrc 源图
	 *@param	hDst 目标图
	 *@param	type 取值如下:
	 *-			ResizeType_Stretch  直接拉伸
	 *-			ResizeType_Scale	按源图宽高比例等比拉伸
	 *@return 	HRESULT __stdcall
	 *@see 		ResizeType
	 *@note		等比绽放时，填充部分像素值为0x00 00 00 00
	 */
	virtual HRESULT __stdcall Resize(const INLEImage* hSrc, INLEImage* hDst, ResizeType type) = 0;

	
	
	/**
	 *@brief  	图像旋转：按中心点旋转
	 *@param	pSrc 源
	 *@param	pDst 目标，取代如下:
	 *-			== NULL 此时会计算旋转后的图像大小，到pOutSize中
	 *-			!= NULL 若图像大小匹配，则进行旋转，旋转后的大小也会放到pOutSize中
	 *@param	fAngle 旋转角度，单位为度，例如：90度-垂直旋转,180--水平旋转,正负意义如下:
	 *-			> 0 顺时针方向
	 *-			< 0 逆时针方向
	 *@param	pOutSize 输出图像的长宽
	 *@return 	HRESULT __stdcall	 
	 *@note		注意事项
	 *-			1.若pDst为NULL，则计算出旋转后图像长宽
	 *-			2.填充黑色部分为透明度为0,即像素值为0x00 00 00 00
	 */
	virtual HRESULT __stdcall Rotate(const INLEImage* pSrc,
									INLEImage* pDst, 
									float fAngle, 
									NLESize* pOutSize) = 0;

	
	/**
	 *@brief  	图像裁剪
	 *@param	pSrc 源图
	 *@param	pDst目标图
	 *@param	rcCrop crop范围，不得越界
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		pDst的Width/Height必须与rcCrop的Width/Height值相等
	 */
	virtual HRESULT __stdcall CropRect(const INLEImage* pSrc, INLEImage* pDst, const NLERect rcCrop) = 0;

	/**
	 *@brief  	图像镜像
	 *@param	pSrc 源图
	 *@param	pDst 目标图
	 *@param	type 类型 MirrorType_xxx
	 *@return 	HRESULT __stdcall
	 */
	virtual HRESULT __stdcall Mirror(const INLEImage* pSrc, INLEImage* pDst, const MirrorType type) = 0;

	/**
	 *@brief  	旋转+缩放
	 *@param	pSrc 源
	 *@param	pDst 目标图 
	 *@param	fAngle 角度 
	 *@param	fScaleX Width缩放倍数
	 *@param	fScaleY Height缩放倍数
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料 GetTransformBound
	 *@note		pDst对象要先存在，pDst的Width/Height长度可通过GetTransformBound得到
	 *@demo		NLEBitmapTest.cpp/TestImageProc
	 */
	virtual HRESULT __stdcall RotateScale(const INLEImage* pSrc,
										INLEImage* pDst,
										float fAngle, 										
										float fScaleX, float fScaleY) = 0;

	/**
	 *@brief  	计算变换后的外接矩形
	 *@param	pSrc 源图
	 *@param	pBound 外接矩形大小，可为NULL
	 *@param	dAngle 角度
	 *@param	dMoveX X平移
	 *@param	dMoveY Y平移
	 *@param	dScaleX X轴缩放倍数
	 *@param	dScaleY Y轴缩放倍数	 
	 *@param	pMatrix 变换矩阵，可为NULL
	 *@param	
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料 
	 */
	virtual HRESULT __stdcall GetTransformBound(const INLEImage* pSrc, 
												NLERect* pBound, 
												float dAngle, 
												float dMoveX, float dMoveY, 
												float dScaleX, float dScaleY,
												double (*pMatrix)[3][3] = NULL) = 0;

	virtual HRESULT __stdcall GetTransformBound(const INLEImage* pSrc,
		const NLETransformMatrix &matTransform, NLERect* pBound) = 0;

	virtual HRESULT __stdcall Transform(const INLEImage* pSrc, 
										INLEImage* pDst, 
										const NLETransformMatrix &matTransform) = 0;
	/**
	*@brief  	生成新掩码图像
	*@param		pSrc 源图
	*@param		pSrcMask 源掩码图
	*@param		pDstMask 目标掩码图
	*@param		nOffX 掩码图像与原始图边界X距离
	*@param		nOffY 掩码图像与原始图边界Y距离
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall GenNewMaskImage(const INLEImage* pSrc,
		const INLEImage* pSrcMask,
		INLEImage* pDstMask,
		MASKPARAM umask) = 0;

	/**
	*@brief  	改变图像alpha值
	*@param		pSrc 源图
	*@param		newAlphaValue alpha值
	*@param		pDst 目标图
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall ChangeImageAlpha(const INLEImage* pSrc,
		float newAlphaValue,
		INLEImage* pDst) = 0;

	/**
	*@brief  	计算掩码后的图像
	*@param		pSrc 源图
	*@param		pMask 掩码图
	*@param		pDst 目标图
	*@param		nOffX 掩码图像与原始图边界X距离
	*@param		nOffY 掩码图像与原始图边界Y距离
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall DoMask(const INLEImage* pSrc,
		const INLEImage* pMask,
		INLEImage* pDst,
		MASKPARAM umask) = 0;

	/**
	*@brief  	计算帧KEYINGPARAM
	*@param		pSrc 源图
	*@param		ukeyParam 结果
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall GenerateKeyColor(const INLEImage* pSrc,
		KEYINGPARAM* ukeyParam) = 0;

	/**
	*@brief  	计算ChromaKey Green Screen后的图像
	*@param		pSrc 源图
	*@param		ukeyParam 抠像颜色参数
	*@param		pDst 目标图
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall GenerateKeyImage(const INLEImage* pSrc,
		const KEYINGPARAM ukeyParam,
		INLEImage* pDst) = 0;

	/**
	*@brief  	计算Border后的图像
	*@param		pSrc 源图
	*@param		uBorderParam 抠像颜色参数
	*@param		pDst 目标图
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall GenerateBorderImage(const INLEImage* pSrc,
		const BORDERPARAM uBorderParam,
		INLEImage* pDst) = 0;

	/**
	*@brief  	计算Shadow后的图像
	*@param		pSrc 源图
	*@param		uShadowParam 抠像颜色参数
	*@param		pDst 目标图
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall GenerateShadowImage(const INLEImage* pSrc,
		const SHADOWSPARAM uShadowParam,
		INLEImage* pDst) = 0;

	/**
	 *@brief  	贴图功能，将hOverImage渲染到hBackImage图片上
	 *@param	hBackImage 底图
	 *@param	hOverImage 要贴的图
	 *@param	x  底图的x轴位置，可为负数
	 *@param	y  底图的x轴位置，可为负数
	 *@param	cx 帧图宽度最大值,为-1时内部计算最大宽度
	 *@param	cy 帧图高度最大值,为-1时内部计算最大高度
	 *@param	xSrc 源的x轴位置
	 *@param	ySrc 源的y轴位置
	 *@return 	HRESULT
	 *@see 		NLEBitmapTest 
	 *@note		像素值拷贝，不做透明度叠加.
	 */
	virtual HRESULT __stdcall BitBlt(INLEImage *hBackImage, INLEImage * hOverImage,
		UINT x, UINT y, UINT cx = -1, UINT cy = -1,
		UINT xSrc = 0, UINT ySrc = 0) = 0;

	

	virtual int __stdcall RGBToHLS(INLEImage* pDst, INLEImage* pSrc) = 0;
	virtual int __stdcall HLSToRGB(INLEImage* pDst, INLEImage* pSrc) = 0;
	virtual int __stdcall RGBToHSV(INLEImage* pDst, INLEImage* pSrc) = 0;
	virtual int __stdcall HSVToRGB(INLEImage* pDst, INLEImage* pSrc) = 0;

	virtual int __stdcall SmoothImage(INLEImage* pDst, INLEImage* pSrc, int nMode, int nLevs) = 0;

	virtual int __stdcall SetAlpha(INLEImage *pDst, unsigned char alpha) = 0;

	virtual int __stdcall HarfMergeImage(INLEImage *pDst, INLEImage *pSrcA, INLEImage *pSrcB) = 0;

	virtual BOOL __stdcall IsBlankImage(INLEImage *pImage) = 0;

#ifdef _DEBUG
	virtual int __stdcall Test() = 0;
#endif // _DEBUG
};                                                                              
