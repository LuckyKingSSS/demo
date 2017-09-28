#pragma once

#include "NLEType.h"
#include "scombase.h"
#include "INLEService.h"
#include "PropertyMacro.h" 
#include "NLEConfig.h"
#include "NLEComPtr.h"
#include "INLELogger.h"
#include "INLESerialize.h"
#include "NLEVariant.h"
#include "NLEPropDecorator.h"
#include <atomic>

typedef struct
{
	NLEVariantType type;
	LPVOID pValuePtr;
}PropValue;

class  CNLEService:public INLEService,
	public INLEProperties
{

public:
	CNLEService();
	virtual ~CNLEService();
public:
	STDOVERRIDEMETHODIMP NonDelegatingQueryInterface(REFIID riid, LPVOID *ppObj);	

	virtual HRESULT __stdcall Init(void) override;

	virtual HRESULT __stdcall AddListener(LPCWSTR keyName, AnyFunction pfn, PropSetingClass* pThis = NULL);
	virtual void __stdcall RegisterListeners();

	virtual HRESULT __stdcall SetParent(IUnknown *pParent) override;
	//不需要释放!!!
	virtual IUnknown * __stdcall GetParent() override;
	virtual NLESession __stdcall GetSession() override { return m_nSession; }

public:
	virtual HRESULT __stdcall GetReconstructProperties(INLEProperties **ppProperties) override;
	virtual HRESULT __stdcall SetReconstructProperties(INLEProperties *pProperties) override;

public:
	DECLARE_IMP_GET
	DECLARE_IMP_SET
	DECLARE_IMP_DEL

	virtual HRESULT __stdcall SetValue(LPCWSTR keyName, CNLEValue& val) override;
	virtual HRESULT __stdcall GetValue(LPCWSTR keyName, CNLEValue& val) override;
	virtual HRESULT __stdcall DelValue(LPCWSTR keyName) override;

	virtual HRESULT __stdcall GetCount(INT *pCount) override;

	virtual HRESULT __stdcall SetInt(LPCWSTR keyName, INT nValue) override;

	virtual HRESULT __stdcall GetInt(LPCWSTR keyName, INT *pValue) override;

    virtual HRESULT __stdcall SetInt64(LPCWSTR keyName, NLElonglong nValue) override;

	virtual HRESULT __stdcall GetInt64(LPCWSTR keyName, NLElonglong *pValue) override;

	virtual HRESULT __stdcall SetString(LPCWSTR keyName, LPCWSTR szValue) override;

	virtual HRESULT __stdcall GetString(LPCWSTR keyName, LPWSTR szValue, int size) override;

	virtual HRESULT __stdcall SetDouble(LPCWSTR keyName, DOUBLE dValue) override;

	virtual HRESULT __stdcall GetDouble(LPCWSTR keyName, DOUBLE *pdValue) override;

	virtual HRESULT __stdcall SetRect(LPCWSTR keyName, const NLERect &rcValue) override;

	virtual HRESULT __stdcall SetRectF(LPCWSTR keyName, const NLERectF& rcValue) override;

	virtual HRESULT __stdcall GetRect(LPCWSTR keyName, NLERect *pNLERectValue) override;

	virtual HRESULT __stdcall GetRectF(LPCWSTR keyName, NLERectF *pNLERectFValue) override;

	virtual HRESULT __stdcall SetPoint(LPCWSTR keyName, const NLEPoint& ptValue) override;

	virtual HRESULT __stdcall SetPointF(LPCWSTR keyName, const NLEPointF& ptValue) override;

	virtual HRESULT __stdcall GetPoint(LPCWSTR keyName, NLEPoint *pPointValue) override;

	virtual HRESULT __stdcall GetPointF(LPCWSTR keyName, NLEPointF *pPointValue) override;

	virtual HRESULT __stdcall SetSize(LPCWSTR keyName, const NLESize& oSize) override;

	virtual HRESULT __stdcall SetSizeF(LPCWSTR keyName, const NLESizeF& oSize) override;

	virtual HRESULT __stdcall GetSize(LPCWSTR keyName, NLESize *pSize) override;

	virtual HRESULT __stdcall GetSizeF(LPCWSTR keyName, NLESizeF *pSize) override;

	virtual HRESULT __stdcall SetRange(LPCWSTR keyName, const NLERange& range) override;

	virtual HRESULT __stdcall GetRange(LPCWSTR keyName, NLERange *pRange) override;

	virtual HRESULT __stdcall SetBlob(LPCWSTR keyName, const NLEBlob& oBlob) override;

	virtual HRESULT __stdcall GetBlob(LPCWSTR keyName, NLEBlob* pBlob) override;

	virtual HRESULT __stdcall SetInterface(LPCWSTR keyName, IUnknown *pUnk) override;

	virtual HRESULT __stdcall GetInterface(LPCWSTR keyName, IUnknown **ppUnk) override;

	virtual HRESULT __stdcall SetGUID(LPCWSTR keyName, const GUID& guid) override;

	virtual HRESULT __stdcall GetGUID(LPCWSTR keyName, GUID *pGuid) override;

	virtual HRESULT __stdcall SetRational(LPCWSTR keyName, const NLERational& ral) override;

	virtual HRESULT __stdcall GetRational(LPCWSTR keyName, NLERational *pRal) override;

	HRESULT __stdcall CopyValue(INLEProperties* pDst, LPCWSTR keyNameFrom, LPCWSTR keyNameTo = NULL ) override;

	HRESULT __stdcall CopyValues(INLEProperties* pDst, ARRAY_WSTR arrKeyNames) override;

	HRESULT __stdcall CopyValues(INLEProperties* pDst, ARRAY_PAIRWSTR arrKeyNames) override;

	HRESULT __stdcall CopyAll(INLEProperties* pDst) override;

	HRESULT __stdcall Delete(LPCWSTR keyName) override;

	HRESULT __stdcall DeleteAll() override;

	HRESULT __stdcall Dump(INLELogger* pLog = NULL) override;

	HRESULT __stdcall GetKey(int nIdex, LPCWSTR *ppwszKeyName) override;

	virtual HRESULT __stdcall SetColorF(LPCWSTR keyName, NLEColorF color) override;

	virtual HRESULT __stdcall GetColorF(LPCWSTR keyName, NLEColorF *pColor) override;


public:
	///是否接受设置
	virtual HRESULT WillSetProperty(LPCWSTR keyName, CNLEValue &varValue) { return S_OK; }
	virtual void DoneSetProperty(LPCWSTR keyName) { m_nSession++; }
	
	///是否接受获得
	virtual HRESULT WillGetProperty(LPCWSTR keyName) { return S_OK; };
	virtual void DoneGetProperty(LPCWSTR keyName) {};

	///是否可删除
	virtual HRESULT WillDeleteProperty(LPCWSTR keyName) { return S_OK; };
	virtual void DoneDeleteProperty(LPCWSTR keyName) { m_nSession++; };

private:
	HRESULT __stdcall GetProperties(INLEProperties **pProperties);
	/**
	 *@brief  	获得键对应的监听列表
	 *@param	keyName 键名，若为NULL则返回所有列表
	 *@return 	NLEPropertyListeners 监听列表
	 */
	NLEPropertyListeners GetListeners(LPCWSTR keyName);

	template <typename FuncType,typename ParamType>
		HRESULT NotifyListeners(LPCWSTR keyName, ParamType param);
	
	template <typename FuncType, typename ParamType1, typename ParamType2, typename ParamType3>
		HRESULT NotifyListeners(LPCWSTR keyName, ParamType1 p1, ParamType2 p2, ParamType3 p3);

protected:
	IUnknown* m_pParent;
	NLEPropertyListeners m_listeners;
	NLEPropDecorator   m_pProperties;
	
private:
	NLESession  m_nSession;
};



#define DECLARE_PROPERTY_LISTEN \
	virtual void STDMETHODCALLTYPE RegisterListeners();

#define PROPERTY_LISTEN_BEGIN(thisClass) \
 void STDMETHODCALLTYPE thisClass::RegisterListeners() \
 {\
	typedef thisClass TheClass; \

/**
 *@brief 设置整数值属性监听函数
 *@note	原型如下:
 *		- HRESULT SetInt(int iValue);
 */
#define PROPERTY_LISTEN_SET_INT(keyName, fnListener) \
	AddListener(keyName, (AnyFunction) (FunctionSetInt)  (&TheClass::fnListener),this);

/**
 *@brief 设置64位整数值属性监听函数
 *@note	原型如下:
 *		- HRESULT SetInt64(NLElonglong llValue);
 */
#define PROPERTY_LISTEN_SET_INT64(keyName, fnListener) \
	AddListener(keyName, (AnyFunction) (FunctionSetInt64)(&TheClass::fnListener),this);

/**
*@brief 设置Double数值属性监听函数
*@note	原型如下:
*		- HRESULT FunctionSetDouble(DOUBLE iValue)
*/
#define PROPERTY_LISTEN_SET_DOUBLE(keyName, fnListener) \
	AddListener(keyName, (AnyFunction) (FunctionSetDouble)(&TheClass::fnListener),this);

/**
 *@brief 设置字符串监听函数
 *@note 原型如下:
 *		- HRESULT SetString(WCHAR *pString)
 */
#define PROPERTY_LISTEN_SET_STRING(keyName, fnListener) \
	AddListener(keyName, (AnyFunction) (FunctionSetString)(&TheClass::fnListener),this);

/**
*@brief 设置Point属性监听函数
*@note	原型如下:
*		- HRESULT SetPoint(NLEPoint point)
*/
#define PROPERTY_LISTEN_SET_POINT(keyName, fnListener) \
	AddListener(keyName, (AnyFunction) (FunctionSetPoint)(&TheClass::fnListener),this);

/**
*@brief 设置Size属性监听函数
*@note	原型如下:
*		- HRESULT SetPoint(NLEPoint point)
*/
#define PROPERTY_LISTEN_SET_SIZE(keyName, fnListener) \
	AddListener(keyName, (AnyFunction) (FunctionSetSize)(&TheClass::fnListener),this);


/**
*@brief 设置NLEPoint数值属性监听函数
*@note	原型如下:
*		- HRESULT SetRect(NLERect rect)
*/
#define PROPERTY_LISTEN_SET_RECT(keyName, fnListener) \
	AddListener(keyName, (AnyFunction) (FunctionSetRect)(&TheClass::fnListener),this);

/**
*@brief 设置NLEPoint数值属性监听函数
*@note	原型如下:
*		- HRESULT SetRect(NLERect rect)
*/
#define PROPERTY_LISTEN_SET_RECTF(keyName, fnListener) \
	AddListener(keyName, (AnyFunction) (FunctionSetRectF)(&TheClass::fnListener),this);


/**
*@brief 设置DATA整数值属性监听函数
*@note	原型如下:
*		- HRESULT SetData(LPVOID pData, UINT uSize)
*/
#define PROPERTY_LISTEN_SET_DATA(keyName, fnListener) \
	AddListener(keyName, (AnyFunction) (FunctionSetData)(&TheClass::fnListener),this);

/**
*@brief 设置接口数值属性监听函数
*@note	原型如下:
*		- HRESULT SetUnknown(IUnknown *pUnk);
*/
#define PROPERTY_LISTEN_SET_INTERFACE(keyName, fnListener) \
	AddListener(keyName, (AnyFunction)(FunctionSetUnknown)(&TheClass::fnListener),this);

/**
*@brief 设置NLERational数值属性监听函数
*@note	原型如下:
*		- HRESULT SetRational(NLERational oRal);
*/
#define PROPERTY_LISTEN_SET_RATIONAL(keyName, fnListener) \
	AddListener(keyName, (AnyFunction)(FunctionSetRational)(&TheClass::fnListener),this);

/**
*@brief 设置GUID数值属性监听函数
*@note	原型如下:
*		- HRESULT SetGuid(const GUID& guid)
*/
#define PROPERTY_LISTEN_SET_GUID(keyName, fnListener) \
	AddListener(keyName, (AnyFunction)(FunctionSetGUID)(&TheClass::fnListener), this);

/**
*@brief 设置CNLEValue数值属性监听函数
*@note	原型如下:
*		- HRESULT SetGuid(const CNLEValue& val)
*/
#define PROPERTY_LISTEN_SET_VALUE(keyName, fnListener) \
	AddListener(keyName, (AnyFunction)(FunctionSetValue)(&TheClass::fnListener), this);

#define PROPERTY_LISTEN_END \
}

#define PROPERTY_LISTENERS_REGISTER(theClass)	theClass::RegisterListeners()