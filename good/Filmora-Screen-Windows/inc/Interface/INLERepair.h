#pragma once

#include "NLEType.h"
#include <Guiddef.h>
#include <objbase.h>
                                                                                
//{E73A17E7-F77A-DE11-16CB-BEF7FECDD50E}
DEFINE_GUID(IID_INLERepair, 0xE73A17E7, 0xF77A, 0xDE11, 0x16, 0xCB, 0xBE, 0xF7, 0xFE, 0xCD, 0xD5, 0x0E);

enum RepairType
{
	RepairType_Correction = 0,
	RepairType_Stablization,
	RepairType_Denoise,
	RepairType_Defog,
};


class INLERepair :public IUnknown
{
public:

	virtual HRESULT __stdcall SetEnable(RepairType eType, BOOL bEnable) = 0;
	virtual HRESULT __stdcall GetEnable(RepairType eType, BOOL* bEnable) = 0;

	/**
	*@brief  	设置去鱼眼参数
	*@param	type 相机类型,相机生成的参数配置文件，比如 "c:\\RICHO_WG-M1_1080_W.xml"
	*@param	nLevel 适配程度，范围[0, 100]
	*@return 	HRESULT __stdcall
	*@see 		GetCorrection
	*@note		注意事项
	*/
	virtual HRESULT __stdcall SetCorrection(LPCWSTR lpwszType, INT nLevel) = 0;
	virtual HRESULT __stdcall GetCorrection(LPCWSTR *lppwszType, INT* pLevel) = 0;

	/**
	*@brief  	SetStalization，稳像
	*@param	nLevel，稳像程度，范围[0, 100]，为0则为不开启
	*@return 	HRESULT __stdcall
	*@see 		GetStablization
	*/
	virtual HRESULT __stdcall SetStalization(INT nLevel = 50) = 0;
	virtual HRESULT __stdcall GetStablization(INT* pLevel) = 0;


	/**
	*@brief  	SetDenoise,音频降噪
	*@param	nLevel 降噪程度，范围[0, 100],为0则为不开启
	*@return 	HRESULT __stdcall
	*@see 		GetDenoise
	*/
	virtual HRESULT __stdcall SetDenoise(INT nLevel = 50) = 0;
	virtual HRESULT __stdcall GetDenoise(INT *pLevel) = 0;

	/**
	*@brief  	SetDefog,去雾
	*@param	nLevel，去雾程度，范围[0, 100],为0则为不开启
	*@return 	HRESULT __stdcall
	*@see 		GetDefog
	*/
	virtual HRESULT __stdcall SetDefog(INT nLevel = 50) = 0;
	virtual HRESULT __stdcall GetDefog(INT* pLevel) = 0;
};                                                                              
