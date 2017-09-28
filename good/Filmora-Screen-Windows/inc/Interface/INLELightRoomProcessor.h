/* ### WS@H Project:NLE ### */

#ifndef _INLELIGHTROOMPROCESSOR_H_
#define _INLELIGHTROOMPROCESSOR_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLEBitmap.h"
                                                                                
//{50222004-880D-1BA7-E0BC-DD1F150B8C04}
DEFINE_GUID(CLSID_NLECubeLUT, 0x50222004, 0x880D, 0x1BA7, 0xE0, 0xBC, 0xDD, 0x1F, 0x15, 0x0B, 0x8C, 0x04);
//{A1754721-5B48-F880-4709-5B1B121EC04F}
DEFINE_GUID(IID_INLECubeLUT, 0xA1754721, 0x5B48, 0xF880, 0x47, 0x09, 0x5B, 0x1B, 0x12, 0x1E, 0xC0, 0x4F);
                                                                         
class INLECubeLUT : public IUnknown
{
public:
	// bstrLutFile 非NULL字符串，lut文件路径(.3dl, .cube), L""空字符串 = 清除lut效果
	// bstrAliasName 非NULL字符串，外部用于标识的名称
	virtual HRESULT __stdcall LoadCubeLutFile(const BSTR bstrLutFile, const BSTR bstrAliasName) = 0;
	virtual HRESULT __stdcall GetCubeLutAliasName(BSTR* ppAliasName) = 0;
	virtual HRESULT __stdcall GetCubeLutFilePath(BSTR* ppLutFile) = 0;

	// 开关LUT效果
	virtual HRESULT __stdcall SetCubeLutEnable(BOOL bEnable) = 0;
	virtual HRESULT __stdcall GetCubeLutEnable(BOOL *pbEnable) = 0;

	//// 从xml配置读取lut
	//virtual HRESULT __stdcall LoadCubeLutFromXml(const BSTR bstrXmlFile) = 0;
};
                                                            
//{EBEF3B24-20FE-9DD8-128A-9D0F555861F9}
DEFINE_GUID(CLSID_NLELightRoomProcessor, 0xEBEF3B24, 0x20FE, 0x9DD8, 0x12, 0x8A, 0x9D, 0x0F, 0x55, 0x58, 0x61, 0xF9);                   
//{79B6E8A1-1BD1-205A-EAF9-D93427E9AD57}
DEFINE_GUID(IID_INLELightRoomProcessor, 0x79B6E8A1, 0x1BD1, 0x205A, 0xEA, 0xF9, 0xD9, 0x34, 0x27, 0xE9, 0xAD, 0x57);                    

class INLELightRoomProcessor : public IUnknown
{
public:
	// ------------------------------------------------------------
	// 色彩基本调节
	
	// 色温（color temperature adjust, scope is [1000k, 30000k]）
	virtual HRESULT __stdcall SetTemp( LONG lValue) = 0;
	virtual HRESULT __stdcall GetTemp( LONG* plValue) = 0;

	// 色调（color hint adjust, scope is [-100, 100]）
	virtual HRESULT __stdcall SetTint( LONG lValue) = 0;
	virtual HRESULT __stdcall GetTint( LONG* plValue) = 0;

	// 色相（color's hue adjustment, scope is [-100, 100] ）
	virtual HRESULT __stdcall SetHue( LONG lValue) = 0;
	virtual HRESULT __stdcall GetHue( LONG* plValue) = 0;

	// 自然饱和度（color vibrance adjust, scope is [-100, 100] ）
	virtual HRESULT __stdcall SetVibrance( LONG lValue) = 0;
	virtual HRESULT __stdcall GetVibrance( LONG* plValue) = 0;

	// 饱和度（color saturation adjust, scope is [-100, 100] ）
	virtual HRESULT __stdcall SetSaturation( LONG lValue) = 0;
	virtual HRESULT __stdcall GetSaturation( LONG* plValue) = 0;

	// 曝光度（color exposure ratio, scope is [-100, 100] ）
	virtual HRESULT __stdcall SetExposure( LONG lValue) = 0;
	virtual HRESULT __stdcall GetExposure( LONG* plValue) = 0;

	// 白平衡（white balance adjust threshold, scope is [0, 100]）
	virtual HRESULT __stdcall SetWhiteBalance( LONG lValue) = 0;
	virtual HRESULT __stdcall GetWhiteBalance( LONG* plValue) = 0;

	// 亮度（color brightness adjust, scope is [-100, 100] ）
	virtual HRESULT __stdcall SetBrightness( LONG lValue) = 0;
	virtual HRESULT __stdcall GetBrightness( LONG* plValue) = 0;

	// 对比度（color contrast adjust, scope is [-100, 100] ）
	virtual HRESULT __stdcall SetContrast( LONG lValue) = 0;
	virtual HRESULT __stdcall GetContrast( LONG* plValue) = 0;

	// -------------------------------------------------------------
	// 高光（hight light adjust, scope is [-100, 100]）
	virtual HRESULT __stdcall SetHDRHighlight( LONG lValue) = 0;
	virtual HRESULT __stdcall GetHDRHighlight( LONG* plValue) = 0;
	
	// 阴影（shadow adjust, scope is [-100, 100] ）
	virtual HRESULT __stdcall SetHDRShadow( LONG lValue) = 0;
	virtual HRESULT __stdcall GetHDRShadow( LONG* plValue) = 0;

	// 白色色阶（HDR White level adjust, scope is [-100, 100] ）
	virtual HRESULT __stdcall SetHDRWhiteLevel( LONG lValue) = 0;
	virtual HRESULT __stdcall GetHDRWhiteLevel( LONG* plValue) = 0;

	// 黑色色阶（HDR Black level adjust, scope is [-100, 100] ）
	virtual HRESULT __stdcall SetHDRBlackLevel( LONG lValue) = 0;
	virtual HRESULT __stdcall GetHDRBlackLevel( LONG* plValue) = 0;

	// 眩光（glare value adjust, scope is [0, 100]）
	virtual HRESULT __stdcall SetLightDiffuse( LONG lValue) = 0;
	virtual HRESULT __stdcall GetLightDiffuse( LONG* plValue) = 0;

	// 去雾（drop haze level, scope is [-100, 100]）
	virtual HRESULT __stdcall SetLightDehaze( LONG lValue) = 0;
	virtual HRESULT __stdcall GetLightDehaze( LONG* plValue) = 0;

	// ------------------------------------------------------------
	// 细节-清晰度（clarity level, scope is [-100, 100] ）
	virtual HRESULT __stdcall SetDetialClarity( LONG lValue) = 0;
	virtual HRESULT __stdcall GetDetialClarity( LONG* plValue) = 0;

	// 细节-锐化（sharpen level, scope is [0, 100]）
	virtual HRESULT __stdcall SetDetialSharpen( LONG lValue) = 0;
	virtual HRESULT __stdcall GetDetialSharpen( LONG* plValue) = 0;

	// 降噪-色彩（noise level adjust, scope is [0, 100]）
	virtual HRESULT __stdcall SetDenoiseColor( LONG lValue) = 0;
	virtual HRESULT __stdcall GetDenoiseColor( LONG* plValue) = 0;

	// 降噪-明度（noise level adjust, scope is [0, 100]）
	virtual HRESULT __stdcall SetDenoiseLuminance( LONG lValue) = 0;
	virtual HRESULT __stdcall GetDenoiseLuminance( LONG* plValue) = 0;

	// 增加噪点-数量（noise level, scope is [0, 100] ）
	virtual HRESULT __stdcall SetNoiseAmount( LONG lValue) = 0;
	virtual HRESULT __stdcall GetNoiseAmount( LONG* plValue) = 0;

	// 增加噪点-大小（noise size level, scope is [0, 100]）
	virtual HRESULT __stdcall SetNoiseSize( LONG lValue) = 0;
	virtual HRESULT __stdcall GetNoiseSize( LONG* plValue) = 0;

	// 畸变（distort level, scope is [-100, 100]）
	virtual HRESULT __stdcall SetDistortLevel( LONG lValue) = 0;
	virtual HRESULT __stdcall GetDistortLevel( LONG* plValue) = 0;

	// 色差（fringing level, scope is [-100, 100]）
	virtual HRESULT __stdcall SetFringingLevel( LONG lValue) = 0;
	virtual HRESULT __stdcall GetFringingLevel( LONG* plValue) = 0;

	// ------------------------------------------------------------
	// 晕影 - 数量（vignette level, scope is [-100, 100]）
	virtual HRESULT __stdcall SetVignetteAmount( LONG lValue) = 0;
	virtual HRESULT __stdcall GetVignetteAmount( LONG* plValue) = 0;

	// 晕影 - 羽化（eclosion level, scope is [0, 100]）
	virtual HRESULT __stdcall SetVignetteFeather( LONG lValue) = 0;
	virtual HRESULT __stdcall GetVignetteFeather( LONG* plValue) = 0;

	// 晕影 - 高亮（highlight level, scope is [0, 100]）
	virtual HRESULT __stdcall SetVignetteHighlights( LONG lValue) = 0;
	virtual HRESULT __stdcall GetVignetteHighlights( LONG* plValue) = 0;

	// 晕影 - 大小（size level, scope is [0, 100]）
	virtual HRESULT __stdcall SetVignetteSize( LONG lValue) = 0;
	virtual HRESULT __stdcall GetVignetteSize( LONG* plValue) = 0;

	// 晕影 - 圆度（roundness level, scope is [0, 100]）
	virtual HRESULT __stdcall SetVignetteRoundness( LONG lValue) = 0;
	virtual HRESULT __stdcall GetVignetteRoundness( LONG* plValue) = 0;

	// 晕影 - 曝光度（exposure level, scope is [-100, 100]）
	virtual HRESULT __stdcall SetVignetteExposure( LONG lValue) = 0;
	virtual HRESULT __stdcall GetVignetteExposure( LONG* plValue) = 0;

	// --------------------------------------------------------------------
	// HLS色彩调节
	// lRegion(0:Red,1:Orange,2Yellow,3:Green,4:Magenta,5:Purple,6:Blue,7Aqua)
	
	// HLS色相（hue adjust, scope is [-100, 100]）
	virtual HRESULT __stdcall SetHLSHue( LONG lRegion, LONG lValue) = 0;
	virtual LONG __stdcall GetHLSHue( LONG lRegion) = 0;

	// HLS亮度（bright adjust, scope is [-100, 100]）
	virtual HRESULT __stdcall SetHLSBrightness( LONG lRegion, LONG lValue) = 0;
	virtual LONG __stdcall GetHLSBrightness( LONG lRegion) = 0;

	// HLS饱和度（saturation adjust, scope is [-100, 100]）
	virtual HRESULT __stdcall SetHLSSaturation( LONG lRegion, LONG lValue) = 0;
	virtual LONG __stdcall GetHLSSaturation( LONG lRegion) = 0;

	// 开关白平衡组
	virtual HRESULT __stdcall SetWhiteBalanceGroup(BOOL bEnable) = 0;
	virtual HRESULT __stdcall GetWhiteBalanceGroup(BOOL *pbEnable) = 0;

	// 开关色调组
	virtual HRESULT __stdcall SetTintGroup(BOOL bEnable) = 0;
	virtual HRESULT __stdcall GetTintGroup(BOOL *pbEnable) = 0;

	// 开关光效组
	virtual HRESULT __stdcall SetLightGroup(BOOL bEnable) = 0;
	virtual HRESULT __stdcall GetLightGroup(BOOL *pbEnable) = 0;

	// 开关晕影组
	virtual HRESULT __stdcall SetVignetteGroup(BOOL bEnable) = 0;
	virtual HRESULT __stdcall GetVignetteGroup(BOOL *pbEnable) = 0;

	// 开关细节组
	virtual HRESULT __stdcall SetDetailGroup(BOOL bEnable) = 0;
	virtual HRESULT __stdcall GetDetailGroup(BOOL *pbEnable) = 0;

	// 开关修复组
	virtual HRESULT __stdcall SetAdvanceFixGroup(BOOL bEnable) = 0;
	virtual HRESULT __stdcall GetAdvanceFixGroup(BOOL *pbEnable) = 0;

	// 开关HSL组
	virtual HRESULT __stdcall SetHSLColorGroup(BOOL bEnable) = 0;
	virtual HRESULT __stdcall GetHSLColorGroup(BOOL *pbEnable) = 0;

	//// 设置输出模式
	//// lMode(0:normal,1:LeftRightTwo,2:LeftRightHalf,3:TopBottomHalf,4:Original)
	//virtual HRESULT __stdcall SetOutputMode(LONG lMode) = 0;
	//virtual HRESULT __stdcall GetOutputMode(LONG *plMode) = 0;

	//// 从其他的已存在的调色对象复制调色参数
	//virtual HRESULT __stdcall ClonePropertyForm(INLELightRoomProcessor *pOther) = 0;

	//// 获取调色前的位图数据
	//virtual HRESULT __stdcall GetOrigImage(INLEImage** pOrigImage) = 0;

	//// 获取调色后的位图数据
	//virtual HRESULT __stdcall GetResultImage(INLEImage** pResultImage) = 0;

	//// 处理位图（处理完的数据可以从GetResultImage得到）
	//virtual HRESULT __stdcall RunOnce(LPBITMAPINFOHEADER lpbiHeader,LPBYTE lpData) = 0;
};


#endif //_INLELIGHTROOMPROCESSOR_H_