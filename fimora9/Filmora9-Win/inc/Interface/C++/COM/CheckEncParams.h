/*! \file CheckEncParams.h
* \brief							校正外部传入的编码参数
* Detailed							用于对外部传入的音视频编码参数，查找对应的音视频编码器，如果有
								对应的参数校正函数，进行校正。
*/
#pragma once
#include <scombase.h>
#include "COMMacro.h"
#include "ConverterType.h"
struct EncodeParam;
/*!
* \class ICheckEncParams
* \brief 对编码参数进行校正
*/

// {58FE100C-DCD4-49db-9930-336174CC0ECC}
DEFINE_GUID(IID_ICheckEncParams, 
			0x58fe100c, 0xdcd4, 0x49db, 0x99, 0x30, 0x33, 0x61, 0x74, 0xcc, 0xe, 0xcc);
EXTERN_C const IID IID_IID_ICheckEncParams;
MACRO_MIDL_INTERFACE(ICheckEncParams, "58FE100C-DCD4-49db-9930-336174CC0ECC")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP CheckEncParams(EncodeParam *m_EncParams) 
	* \brief  获取流的名称
	* \param  m_EncParams [in / out] 编码参数
	* \retval 传入带校正的参数，校正后传出
	*/
	virtual STDMETHODIMP CheckEncParams(EncodeParam *m_EncParams) = 0;
};

//定义组件对象
MIDL_DEFINE_GUID(CLSID, CLSID_CCheckEncParams, 0xf1eb87e5, 0x6940, 0x42b6, 0xb4, 0xce, 0x20, 0x8a, 0xa7, 0x69, 0x00, 0x20);

EXTERN_C const CLSID CLSID_CCheckEncParams;