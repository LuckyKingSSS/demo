#ifndef _H_INLEPROPERTIES_H_
#define _H_INLEPROPERTIES_H_

#include "NLEType.h"
#include "NLEGraphics.h"
#include <Guiddef.h>                                                            
#include <objbase.h>
#include "INLELogger.h"
#include <vector>

//{0610B936-8051-4E51-9AC8-FC9F5B2D2FFD}
DEFINE_GUID(CLSID_NLEProperties, 0x0610B936, 0x8051, 0x4E51, 0x9A, 0xC8, 0xFC, 0x9F, 0x5B, 0x2D, 0x2F, 0xFD);

//{D497118B-7856-4F21-8023-8CB4C00DE144}
DEFINE_GUID(IID_INLEProperties, 0xD497118B, 0x7856, 0x4F21, 0x80, 0x23, 0x8C, 0xB4, 0xC0, 0x0D, 0xE1, 0x44);

//{5967B77B-7B3D-C4D2-066F-495FDB20D7D7}
DEFINE_GUID(IID_INLEPropInternal, 0x5967B77B, 0x7B3D, 0xC4D2, 0x06, 0x6F, 0x49, 0x5F, 0xDB, 0x20, 0xD7, 0xD7);                         



#include "NLEValue.h"



/**@ingroup GroupCommon 
 *@brief 属性表，使用键值对表示.离散值和动画值.
 */
class INLEProperties : public IUnknown
{
public:
	/**	
	警告：CNLEValue类为NLE内部使用类，禁止外部使用CNLEValue类与NLE库进行交互调用,
	SetValue, GetValue 接口为NLE内部使用接口，仅限NLE内部使用
	*/
	virtual HRESULT __stdcall SetValue(LPCWSTR keyName, CNLEValue& val) = 0;
	virtual HRESULT __stdcall GetValue(LPCWSTR keyName, CNLEValue& val) = 0;
	virtual HRESULT __stdcall DelValue(LPCWSTR keyName) = 0;
	
	/**
	 *@brief 获取属性个数
	 */
	virtual HRESULT __stdcall GetCount(INT *pCount) = 0;

	/**
	*@brief  	设置整数值
	*@param	&keyName [in] 键名称
	*@param	nValue [in] 键值
	*@return 	stand
	*	- S_OK	Successfully
	*	- Others Failed
	*/
	virtual HRESULT __stdcall SetInt(LPCWSTR keyName, INT nValue) = 0;
	virtual HRESULT __stdcall GetInt(LPCWSTR keyName, INT *pValue) = 0;

	virtual HRESULT __stdcall SetInt64(LPCWSTR keyName, NLElonglong nValue) = 0;
	virtual HRESULT __stdcall GetInt64(LPCWSTR keyName, NLElonglong *pValue) = 0;

	virtual HRESULT __stdcall SetString(LPCWSTR keyName, LPCWSTR szValue) = 0;	
	/**
	 *@brief  	获取字条串类型键值
	 *@param	keyName 键名
	 *@param	szValue 键值对应的字符串内存，由外部分配
	 *@param	nBufferSize	字符个数，该值必须至少比字符个数多1
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall GetString(LPCWSTR keyName, LPWSTR szValueBuffer, int nBufferSize) = 0;

	virtual HRESULT __stdcall SetDouble(LPCWSTR keyName, DOUBLE dValue) = 0;
	virtual HRESULT __stdcall GetDouble(LPCWSTR keyName, DOUBLE *pdValue) = 0;

	virtual HRESULT __stdcall SetRect(LPCWSTR keyName, const NLERect &rcValue) = 0;
	virtual HRESULT __stdcall GetRect(LPCWSTR keyName, NLERect *pNLERectValue) = 0;

	virtual HRESULT __stdcall SetRectF(LPCWSTR keyName, const NLERectF& rcValue) = 0;
	virtual HRESULT __stdcall GetRectF(LPCWSTR keyName, NLERectF *pNLERectFValue) = 0;
	
	virtual HRESULT __stdcall SetPoint(LPCWSTR keyName, const NLEPoint& ptValue) = 0;
	virtual HRESULT __stdcall GetPoint(LPCWSTR keyName, NLEPoint *pPointValue) = 0;

	virtual HRESULT __stdcall SetRange(LPCWSTR keyName, const NLERange& ptValue) = 0;
	virtual HRESULT __stdcall GetRange(LPCWSTR keyName, NLERange *pNLERangeValue) = 0;

	virtual HRESULT __stdcall SetPointF(LPCWSTR keyName, const NLEPointF& ptValue) = 0;
	virtual HRESULT __stdcall GetPointF(LPCWSTR keyName, NLEPointF *pPointValue) = 0;

	virtual HRESULT __stdcall SetSize(LPCWSTR keyName, const NLESize& oSize) = 0;
	virtual HRESULT __stdcall GetSize(LPCWSTR keyName, NLESize *pSize) = 0;

	virtual HRESULT __stdcall SetSizeF(LPCWSTR keyName, const NLESizeF& oSize) = 0;
	virtual HRESULT __stdcall GetSizeF(LPCWSTR keyName, NLESizeF *pSize) = 0;

	virtual HRESULT __stdcall SetBlob(LPCWSTR keyName, const NLEBlob& oBlob) = 0;
	virtual HRESULT __stdcall GetBlob(LPCWSTR keyName, NLEBlob* pBlob) = 0;

	/**
	 *@brief  	设置接口
	 *@param[in] &keyName
	 *@param[in] pUnk
	 *@return 	标准返回
	 *@see 		Delete,GetInterface
	 *@note		
	 *	- pUnk会增加引用计数，直到INLEProperties对象被释放，或使用Delete删除时,才会减少引用计数	 
	 *	- 可通过该接口实现监听者接口。
	 */
	virtual HRESULT __stdcall SetInterface(LPCWSTR keyName, IUnknown *pUnk) = 0;
	/**
	 *@brief  	获取接口
	 *@param[in]  &keyName 键值
	 *@param[out] ppUnk 接口值指针
	 *@return 	标准返回
	 *@see 		SetInterface
	 *@note		对应的被调用的接口计数会增加1，使用完ppUnk后记得Release
	 */
	virtual HRESULT __stdcall GetInterface(LPCWSTR keyName, IUnknown **ppUnk) = 0;


	/**
	 *@brief  	设置GUID
	 *@param	keyName 键名
	 *@param	guid 值
	 *@return 	标准返回
	 *@see 		GetGUID
	 */
	virtual HRESULT __stdcall SetGUID(LPCWSTR keyName, const GUID& guid) = 0;


	/**
	 *@brief  	获取GUID
	 *@param	keyName 键名
	 *@param	pGuid 值
	 *@return 	标准返回
	 */
	virtual HRESULT __stdcall GetGUID(LPCWSTR keyName, GUID *pGuid) = 0;

	/**
	 *@brief 获取Rational
	 *@param keyName 键名
	 *@param ral 值	
	 */
	virtual HRESULT __stdcall SetRational(LPCWSTR keyName, const NLERational& ral) = 0;

	/**
	 *@brief 获得Rational
	 */
	virtual HRESULT __stdcall GetRational(LPCWSTR keyName, NLERational *pRal) = 0;

	/**
	 *@brief 复制对应键的值
	 */
	virtual HRESULT __stdcall CopyValue(INLEProperties* pDst, LPCWSTR keyNameFrom, LPCWSTR keyNameTo = NULL) = 0;

	/**
	 *@brief 复制键组的值，只要有一个不存在或不能Copy都会返回失败
	 *@param arrKeyNames 原型std::vector<LPCWSTR>，要复制的键组。快速使用方式：{ kName1, kName2, kName3 }
	*/
	virtual HRESULT __stdcall CopyValues(INLEProperties* pDst, ARRAY_WSTR arrKeyNames) = 0;

	/**
	*@brief 复制所有值	
	*/
	virtual HRESULT __stdcall CopyAll(INLEProperties* pDst) = 0;

	/**
	*@brief 复制键对组的值，只要有一个不存在或不能Copy都会返回失败
	*@param arrKeyNames 原型std::vector<LPCWSTR， LPCWSTR>，要复制的键组。快速使用方式：{ （kName1, kName2） }
	*/
	virtual HRESULT __stdcall CopyValues(INLEProperties* pDst, ARRAY_PAIRWSTR arrKeyNames) = 0;

	/**
	 *@brief  	删除属性
	 *@param[in] &keyName
	 *@return 	标准返回
	 *@see 		SetInterface
	 */
	virtual HRESULT __stdcall Delete(LPCWSTR keyName) = 0;	

	/**
	 *@brief  	删除所有键值,相当于Clear
	 *@return 	HRESULT __stdcall	 
	 */
	virtual HRESULT __stdcall DeleteAll() = 0;
	
	virtual HRESULT __stdcall Dump(INLELogger* pLog = NULL) = 0;

	/**
	 *@brief 获取第nIndex属性的键名
	 */
	virtual HRESULT __stdcall GetKey(int nIdex, LPCWSTR *ppwszKeyName) = 0;

	virtual HRESULT __stdcall SetColorF(LPCWSTR keyName, NLEColorF color) = 0;
	virtual HRESULT __stdcall GetColorF(LPCWSTR keyName, NLEColorF *pColor) = 0;
};                                                        




/**
 *@} 
 */
#endif //_H_INLEPROPERTIES_H_
