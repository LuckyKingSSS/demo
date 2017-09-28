#pragma once

#include <errno.h>

#include <string>

#ifdef WIN32
#include <xstring>
#else
#include <typeinfo>
#endif

#include <Guiddef.h>
#include <objbase.h>

#include "NLEType.h"
#include "NLEGraphics.h"
#include <typeinfo>

#include "base64.h"
//#include "scombase.h"

#include <stdlib.h>
#include <locale.h>

class CNLEValue;

template<class T>
class CNLEPtr
{
public:
	CNLEPtr() : _pv(NULL) {
	}
	CNLEPtr(const CNLEPtr& ptr) : _pv(ptr._pv) {
	}
	CNLEPtr(T* const pv) : _pv(pv) {
	}
	virtual ~CNLEPtr() {
	}

public:
	virtual T* operator ->() {
		return _pv;
	}

	virtual operator T*() {
		return _pv;
	}

	virtual CNLEPtr& operator =(const CNLEPtr& ptr) {
		_pv = ptr._pv;
		return *this;
	}

private:
	T* _pv;
};

// 所谓的接口，做特殊处理
// 要求，仅接受IUnknown接口
#define VALUE_TYPE_IUNKNOWN	L"CNLEPtr<IUnknown>"

/**
警告：CNLEValue类为NLE内部使用类，禁止外部使用该类与NLE库进行交互调用
*/
class CNLEValue
{
public:
	CNLEValue() : _t(L""), _str(L"") {
	}
	CNLEValue(const CNLEValue& val) : _t(L""), _str(L"") {
		*this = val;
	}
	virtual ~CNLEValue() {
		if (_wcsicmp(_t.c_str(), VALUE_TYPE_IUNKNOWN) == 0) // 针对接口的特殊处理
		{
			IUnknown* x = NULL;
			swscanf_s(_str.c_str(), L"Interface:%p", &x);
			x->Release();
		}
	}

public:
	template<class T> int Set(/*const std::wstring& k,*/ T& v) {
		int ret = 0;

// 		printf("typeid: %s\n", typeid(v).name());
		ret = T2String(v, _t, _str);
		if (ret == 0)
		{
			if (_wcsicmp(_t.c_str(), VALUE_TYPE_IUNKNOWN) == 0) // 针对接口的特殊处理
			{
				((IUnknown*)*(CNLEPtr<IUnknown>*)&v)->AddRef();
			}
		}
		return ret;
	}

	template<class T> int Get(T& v) {
		int ret = 0;

		ret = String2Value(_t, _str, v);
		if (ret == 0)
		{
			if (_wcsicmp(_t.c_str(), VALUE_TYPE_IUNKNOWN) == 0) // 针对接口的特殊处理
			{
				((IUnknown*)*(CNLEPtr<IUnknown>*)&v)->AddRef();
			}
		}
		return ret;
	}

	template<class T>  operator T () 
	{
		T v;
		int ret = Get(v);
		return v;
	}

	/*template<class T> int operator= (const T v)
	{
		return Set(v);
	}*/

	std::wstring Type() {
		return _t;
	}

	std::wstring ToString() {
		return _str;
	}

	void fromString(WCHAR *str)
	{
		_str = str;
	}

public:
	virtual CNLEValue& operator =(const CNLEValue& val) {
		this->_t = val._t;
		this->_str = val._str;
		if (this->_t == VALUE_TYPE_IUNKNOWN) // 针对接口的特殊处理
		{
			IUnknown* x = NULL;
			swscanf_s(this->_str.c_str(), L"Interface:%p", &x);
			x->AddRef();
		}
		return *this;
	}

private:
	std::wstring _t;
	std::wstring _str;



public:

	template<class T>
	static int T2String(T& v, std::wstring& t, std::wstring& str) {
		int ret = 0;
		WCHAR tmpStr[128];

		// 	printf("T2String: type %s\n", typeid(v).name());

#define RTTI_TYPE_DEAL(v, type)		(typeid(v) == typeid(type))
#define SET_TYPE_X(v, t, type)			t = L ## #type; \
	type& x = (type&)v

		const char* psz = typeid(v).name();

		if (RTTI_TYPE_DEAL(v, char)) {
			SET_TYPE_X(v, t, char);
			swprintf(tmpStr, 128, L"%lc", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, unsigned char)) {
			SET_TYPE_X(v, t, unsigned char);
			swprintf(tmpStr, 128, L"%lc", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, short)) {
			SET_TYPE_X(v, t, short);
			swprintf(tmpStr, 128, L"%d", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, unsigned short)) {
			SET_TYPE_X(v, t, unsigned short);
			swprintf(tmpStr, 128, L"%d", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, int)) {
			SET_TYPE_X(v, t, int);
			swprintf(tmpStr, 128, L"%d", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, unsigned int)) {
			SET_TYPE_X(v, t, unsigned int);
			swprintf(tmpStr, 128, L"%u", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, long)) {
			SET_TYPE_X(v, t, long);
			swprintf(tmpStr, 128, L"%ld", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, unsigned long)) {
			SET_TYPE_X(v, t, unsigned long);
			swprintf(tmpStr, 128, L"%ld", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, long long)) {
			SET_TYPE_X(v, t, long long);
			swprintf(tmpStr, 128, L"%lld", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, float)) {
			SET_TYPE_X(v, t, float);
			swprintf(tmpStr, 128, L"%f", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, double)) {
			SET_TYPE_X(v, t, double);
			swprintf(tmpStr, 128, L"%lf", x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, std::wstring)) {
			SET_TYPE_X(v, t, std::wstring);
			str = x;
		}
		else

		if (RTTI_TYPE_DEAL(v, NLEPoint)) {
			SET_TYPE_X(v, t, NLEPoint);
			str = x.toString();
		}
		else

		if (RTTI_TYPE_DEAL(v, NLEPointF)) {
			SET_TYPE_X(v, t, NLEPointF);
			str = x.toString();
		}
		else

		if (RTTI_TYPE_DEAL(v, NLESize)) {
			SET_TYPE_X(v, t, NLESize);
			str = x.toString();
		}
		else

		if (RTTI_TYPE_DEAL(v, NLESizeF)) {
			SET_TYPE_X(v, t, NLESizeF);
			str = x.toString();
		}
		else

		if (RTTI_TYPE_DEAL(v, NLERect)) {
			SET_TYPE_X(v, t, NLERect);
			str = x.toString();
		}
		else

		if (RTTI_TYPE_DEAL(v, NLERectF)) {
			SET_TYPE_X(v, t, NLERectF);
			str = x.toString();
		}
		else

		if (RTTI_TYPE_DEAL(v, NLERational)) {
			SET_TYPE_X(v, t, NLERational);
            swprintf(tmpStr, 128, L"[%lld, %lld]: %.7lf ", x.num, x.den, x.num * 1.0 / x.den);
			str = tmpStr;
		}
		else 

		if (RTTI_TYPE_DEAL(v, NLEColorF)) {
			SET_TYPE_X(v, t, NLEColorF);			
			str = x.toString();
		}
		else

		if (RTTI_TYPE_DEAL(v, NLEBlob)) {
			SET_TYPE_X(v, t, NLEBlob);

			// 将二进制进行base64
			size_t tmpStrLen = 128 + 2 * x.uSize;
			WCHAR *pTmpStr = new WCHAR[tmpStrLen];
			RETURN_IF_NULL(pTmpStr, ENOMEM);

			char *dataStr = NULL;
			WCHAR *dataStr_ = NULL;
			if (x.pData != NULL && x.uSize > 0)
			{
				int strLen = 2 * x.uSize;
				dataStr = new char[strLen];
				dataStr_ = new WCHAR[strLen];
				RETURN_IF_NULL(dataStr, ENOMEM);
				Base64Encode(dataStr, x.pData, x.uSize);

				size_t retLen = 0;
				setlocale(LC_CTYPE, "");
#ifdef WIN32
				mbstowcs_s(&retLen, dataStr_, strLen, dataStr, strLen);
#else
                mbstowcs(dataStr_,dataStr,(size_t)strlen);
#endif
			}

			swprintf(pTmpStr, tmpStrLen, L"uDataSize:%d, bCopy:%d, pData:%s, pExtend:%ls",
				x.uSize, TRUE/*x.bCopy*/, // 保存的blob，必须是copy的。因为后续读出的时候，会分配内存的
				dataStr_ != NULL ? dataStr_ : L"",
				L"");

			str = pTmpStr;

			SAFE_DELETE_ARRAY(dataStr);
			SAFE_DELETE_ARRAY(dataStr_);
			SAFE_DELETE_ARRAY(pTmpStr);
		}
		else

		if (RTTI_TYPE_DEAL(v, GUID)) {
			SET_TYPE_X(v, t, GUID);
			swprintf(tmpStr, 128, L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
				x.Data1, x.Data2, x.Data3,
				x.Data4[0], x.Data4[1],
				x.Data4[2], x.Data4[3],
				x.Data4[4], x.Data4[5],
				x.Data4[6], x.Data4[7]);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, CNLEPtr<IUnknown>)) {
			SET_TYPE_X(v, t, CNLEPtr<IUnknown>);
			swprintf(tmpStr, 128, L"Interface:%p", (IUnknown*)x);
			str = tmpStr;
		}
		else

		if (RTTI_TYPE_DEAL(v, NLERange)) {
			SET_TYPE_X(v, t, NLERange);
			str = x.toString();
		}
		else

		if (RTTI_TYPE_DEAL(v, CNLEValue)){
			SET_TYPE_X(v, t, CNLEValue);

			size_t len = x.Type().length() + x.ToString().length() + 100;
			wchar_t *pStr = new wchar_t[len];

			swprintf(pStr, len, L"t:%ls, v:%ls", x.Type().c_str(), x.ToString().c_str());
			str = pStr;

			delete[] pStr;
		}
		else {
			NLE_ASSERT(0);
			return EINVAL;
		}

#undef SET_TYPE_X
#undef RTTI_TYPE_DEAL

		return 0;
	}

	static int String2Value(const std::wstring& k, const std::wstring& t, const std::wstring& str, CNLEValue& val) {

#define RTTI_TYPE_DEAL(t, type)		(t == L ## #type)
#define SET_TYPE_X(type) type x

		if (RTTI_TYPE_DEAL(t, char)) {
			SET_TYPE_X(char);
			swscanf_s(str.c_str(), L"%lc", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, unsigned char)) {
			SET_TYPE_X(unsigned char);
			swscanf_s(str.c_str(), L"%d", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, short)) {
			SET_TYPE_X(short);
			swscanf_s(str.c_str(), L"%d", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, unsigned short)) {
			SET_TYPE_X(unsigned short);
			swscanf_s(str.c_str(), L"%d", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, int)) {
			SET_TYPE_X(int);
			swscanf_s(str.c_str(), L"%d", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, unsigned int)) {
			SET_TYPE_X(unsigned int);
			swscanf_s(str.c_str(), L"%d", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, long)) {
			SET_TYPE_X(long);
			swscanf_s(str.c_str(), L"%ld", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, unsigned long)) {
			SET_TYPE_X(unsigned long);
			swscanf_s(str.c_str(), L"%ld", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, long long)) {
			SET_TYPE_X(long long);
			swscanf_s(str.c_str(), L"%lld", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, float)) {
			SET_TYPE_X(float);
			swscanf_s(str.c_str(), L"%f", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, double)) {
			SET_TYPE_X(double);
			swscanf_s(str.c_str(), L"%lf", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, std::wstring)) {
			SET_TYPE_X(std::wstring);
			x = str;
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, NLEPoint)) {
			SET_TYPE_X(NLEPoint);
			x.fromString((WCHAR*)str.c_str());
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, NLEPointF)) {
			SET_TYPE_X(NLEPointF);
			x.fromString((WCHAR*)str.c_str());
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, NLESize)) {
			SET_TYPE_X(NLESize);
			x.fromString((WCHAR*)str.c_str());
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, NLESizeF)) {
			SET_TYPE_X(NLESizeF);
			x.fromString((WCHAR*)str.c_str());
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, NLERect)) {
			SET_TYPE_X(NLERect);
			x.fromString((WCHAR*)str.c_str());
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, NLERectF)) {
			SET_TYPE_X(NLERectF);
			x.fromString((WCHAR*)str.c_str());
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, NLERational)) {
			SET_TYPE_X(NLERational);
			swscanf_s(str.c_str(), L"[%lld, %lld]: ", &x.num, &x.den);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, NLEColorF)) {
			SET_TYPE_X(NLEColorF);
			x.fromString((WCHAR*)str.c_str());
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, NLEBlob)) {
			SET_TYPE_X(NLEBlob);

			// 先拿到长度
			swscanf_s(str.c_str(), L"uDataSize:%d, bCopy:%d", &x.uSize, &x.bCopy);

			x.bCopy = FALSE;
			x.pData = NULL;
			x.pExtend = NULL;

			int strLen = 2 *x.uSize;
			if (strLen > 0)
			{
				WCHAR extendStr[64];
				WCHAR *dataStr_ = new WCHAR[strLen];
				RETURN_IF_NULL(dataStr_, ENOMEM);
				char *dataStr = new char[strLen];
				RETURN_IF_NULL(dataStr_, ENOMEM);
				x.pData = (LPBYTE)new char[x.uSize];
				RETURN_IF_NULL(x.pData, ENOMEM);

#ifdef WIN32
				swscanf_s(str.c_str(), L"uDataSize:%d, bCopy:%d, pData:%l[^,], pExtend:%ls", &x.uSize, &x.bCopy, dataStr_, strLen, extendStr, 64);
#else
				swscanf_s(str.c_str(), L"uDataSize:%d, bCopy:%d, pData:%l[^,], pExtend:%ls", &x.uSize, &x.bCopy, dataStr_, extendStr);
#endif // WIN32

				size_t retLen = 0;
				setlocale(LC_CTYPE, "");
				
#ifdef WIN32
                wcstombs_s(&retLen, dataStr, strLen, dataStr_, strLen);
#else
                wcstombs(dataStr,dataStr_,(size_t)strlen);
#endif
				x.uSize = Base64Decode(x.pData, dataStr, -1);
				x.bCopy = TRUE;

				SAFE_DELETE_ARRAY(dataStr_);
				SAFE_DELETE_ARRAY(dataStr);
			}

			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, GUID)) {
			SET_TYPE_X(GUID);
			CLSIDFromString(str.c_str(), &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, CNLEPtr<IUnknown>)) {
			
			NLE_ASSERT(false); // 不支持从外部字符串生成IUnknown接口
							   // 仅可直接从IUnknown生成CNLEValue

			SET_TYPE_X(CNLEPtr<IUnknown>);
			swscanf_s(str.c_str(), L"Interface:%p", &x);
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, NLERange)) {
			SET_TYPE_X(NLERange);
			x.fromString((WCHAR*)str.c_str());
			val.Set(x);
		}
		else

		if (RTTI_TYPE_DEAL(t, CNLEValue)) {
			SET_TYPE_X(CNLEValue);
			WCHAR typeStr[MAX_PATH];
			//WCHAR valStr[MAX_PATH];
			size_t len = str.length();
			wchar_t *valStr = new wchar_t[len];
			memset(valStr, 0x00, len *sizeof(wchar_t));
#ifdef WIN32
			swscanf_s(str.c_str(), L"t:%l[^,], v:%l[^\0]", typeStr, MAX_PATH, valStr, len);
#else
			swscanf_s(str.c_str(), L"t:%l[^,], v:%l[^\0]", typeStr, valStr);
#endif
			
			x._t = typeStr;
			x._str = valStr;

			delete[] valStr;

			val.Set(x);
		}
		else {
			NLE_ASSERT(0);
			return EINVAL;
		}

#undef SET_TYPE_X
#undef RTTI_TYPE_DEAL

		return 0;
	}

	template<class T> static int String2Value(const std::wstring& t, const std::wstring& str, T& v) {

#define RTTI_TYPE_DEAL(t, type)		(t == L ## #type)
#define SET_TYPE_X(type) type x

		if (RTTI_TYPE_DEAL(t, char)) {
			SET_TYPE_X(char);
			swscanf_s(str.c_str(), L"%lc", &x);
			memcpy((char*)&v, &x, sizeof(char));
		}
		else

		if (RTTI_TYPE_DEAL(t, unsigned char)) {
			SET_TYPE_X(unsigned char);
			swscanf_s(str.c_str(), L"%lc", &x);
			memcpy((char*)&v, &x, sizeof(unsigned char));
		}
		else

		if (RTTI_TYPE_DEAL(t, short)) {
			SET_TYPE_X(short);
			swscanf_s(str.c_str(), L"%hd", &x);
			memcpy((char*)&v, &x, sizeof(short));
		}
		else

		if (RTTI_TYPE_DEAL(t, unsigned short)) {
			SET_TYPE_X(unsigned short);
			swscanf_s(str.c_str(), L"%hd", &x);
			memcpy((char*)&v, &x, sizeof(unsigned short));
		}
		else

		if (RTTI_TYPE_DEAL(t, int)) {
			SET_TYPE_X(int);
			swscanf_s(str.c_str(), L"%d", &x);
			memcpy((char*)&v, &x, sizeof(int));
		}
		else

		if (RTTI_TYPE_DEAL(t, unsigned int)) {
			SET_TYPE_X(unsigned int);
			swscanf_s(str.c_str(), L"%d", &x);
			memcpy((char*)&v, &x, sizeof(unsigned int));
		}
		else

		if (RTTI_TYPE_DEAL(t, long)) {
			SET_TYPE_X(long);
			swscanf_s(str.c_str(), L"%ld", &x);
			memcpy((char*)&v, &x, sizeof(long));
		}
		else

		if (RTTI_TYPE_DEAL(t, unsigned long)) {
			SET_TYPE_X(unsigned long);
			swscanf_s(str.c_str(), L"%ld", &x);
			memcpy((char*)&v, &x, sizeof(unsigned long));
		}
		else

		if (RTTI_TYPE_DEAL(t, long long)) {
			SET_TYPE_X(long long);
			swscanf_s(str.c_str(), L"%lld", &x);
			memcpy((char*)&v, &x, sizeof(long long));
		}
		else

		if (RTTI_TYPE_DEAL(t, float)) {
			SET_TYPE_X(float);
			swscanf_s(str.c_str(), L"%f", &x);
			memcpy((char*)&v, &x, sizeof(float));
		}
		else

		if (RTTI_TYPE_DEAL(t, double)) {
			SET_TYPE_X(double);
			swscanf_s(str.c_str(), L"%lf", &x);
			memcpy((char*)&v, &x, sizeof(double));
		}
		else

		if (RTTI_TYPE_DEAL(t, std::wstring)) {
			SET_TYPE_X(std::wstring);
			x = str;
			*(std::wstring*)&v = x;
		}
		else

		if (RTTI_TYPE_DEAL(t, NLEPoint)) {
			SET_TYPE_X(NLEPoint);
			x.fromString((WCHAR*)str.c_str());
			memcpy((char*)&v, &x, sizeof(NLEPoint));
		}
		else

		if (RTTI_TYPE_DEAL(t, NLEPointF)) {
			SET_TYPE_X(NLEPointF);
			x.fromString((WCHAR*)str.c_str());
			memcpy((char*)&v, &x, sizeof(NLEPointF));
		}
		else

		if (RTTI_TYPE_DEAL(t, NLESize)) {
			SET_TYPE_X(NLESize);
			x.fromString((WCHAR*)str.c_str());
			memcpy((char*)&v, &x, sizeof(NLESize));
		}
		else

		if (RTTI_TYPE_DEAL(t, NLESizeF)) {
			SET_TYPE_X(NLESizeF);
			x.fromString((WCHAR*)str.c_str());
			memcpy((char*)&v, &x, sizeof(NLESizeF));
		}
		else

		if (RTTI_TYPE_DEAL(t, NLERect)) {
			SET_TYPE_X(NLERect);
			x.fromString((WCHAR*)str.c_str());
			memcpy((char*)&v, &x, sizeof(NLERect));
		}
		else

		if (RTTI_TYPE_DEAL(t, NLERectF)) {
			SET_TYPE_X(NLERectF);
			x.fromString((WCHAR*)str.c_str());
			memcpy((char*)&v, &x, sizeof(NLERectF));
		}
		else

		if (RTTI_TYPE_DEAL(t, NLEColorF)) {
			SET_TYPE_X(NLEColorF);
			x.fromString((WCHAR*)str.c_str());
			memcpy((char*)&v, &x, sizeof(NLEColorF));
		}
		else

		if (RTTI_TYPE_DEAL(t, NLERational)) {
			SET_TYPE_X(NLERational);
			swscanf_s(str.c_str(), L"[%lld, %lld]: ", &x.num, &x.den);
			memcpy((char*)&v, &x, sizeof(NLERational));
		}
		else

		if (RTTI_TYPE_DEAL(t, NLEBlob)) {
			SET_TYPE_X(NLEBlob);

			// 先拿到长度
			swscanf_s(str.c_str(), L"uDataSize:%d, bCopy:%d", &x.uSize, &x.bCopy);

			x.bCopy = FALSE;
			x.pData = NULL;
			x.pExtend = NULL;

			int strLen = 2 * x.uSize;
			if (strLen > 0)
			{
				WCHAR extendStr[64];
				WCHAR *dataStr_ = new WCHAR[strLen];
				RETURN_IF_NULL(dataStr_, ENOMEM);
				char *dataStr = new char[strLen];
				RETURN_IF_NULL(dataStr_, ENOMEM);
				x.pData = (LPBYTE)new char[x.uSize + 10];
				RETURN_IF_NULL(x.pData, ENOMEM);

#ifdef WIN32
				swscanf_s(str.c_str(), L"uDataSize:%d, bCopy:%d, pData:%l[^,], pExtend:%ls", &x.uSize, &x.bCopy, dataStr_, strLen, extendStr, 64);
#else
				swscanf_s(str.c_str(), L"uDataSize:%d, bCopy:%d, pData:%l[^,], pExtend:%ls", &x.uSize, &x.bCopy, dataStr_, extendStr);
#endif // WIN32

				size_t retLen = 0;
				setlocale(LC_CTYPE, "");
				
#ifdef WIN32
                wcstombs_s(&retLen, dataStr, strLen, dataStr_, strLen);
#else
                wcstombs(dataStr,dataStr_,(size_t)strlen);
#endif

				x.uSize = Base64Decode(x.pData, dataStr, -1);
				x.bCopy = FALSE;

				SAFE_DELETE_ARRAY(dataStr_);
				SAFE_DELETE_ARRAY(dataStr);
			}

			NLEBlob* ptr = (NLEBlob*)&v;
			ptr->uSize = x.uSize;
			ptr->bCopy = TRUE;
			ptr->pData = x.pData;
			ptr->pExtend = x.pExtend;
		}
		else

		if (RTTI_TYPE_DEAL(t, GUID)) {
			SET_TYPE_X(GUID);
			CLSIDFromString(str.c_str(), &x);
			memcpy((char*)&v, &x, sizeof(GUID));
		}
		else

		if (RTTI_TYPE_DEAL(t, CNLEPtr<IUnknown>)) {
			SET_TYPE_X(IUnknown*);
			swscanf_s(str.c_str(), L"Interface:%p", &x);

			CNLEPtr<IUnknown>* ptr = (CNLEPtr<IUnknown>*)&v;
			*ptr = x;
		}
		else

		if (RTTI_TYPE_DEAL(t, NLERange)) {
			SET_TYPE_X(NLERange);
			x.fromString((WCHAR*)str.c_str());
						
			memcpy((char*)&v, &x, sizeof(NLERange));
		}
		else

		if (RTTI_TYPE_DEAL(t, CNLEValue)) {
			SET_TYPE_X(CNLEValue);
			
			WCHAR typeStr[MAX_PATH];
			size_t len = str.length();
			wchar_t *valStr = new wchar_t[len];
			memset(valStr, 0x00, len *sizeof(wchar_t));

#ifdef WIN32
			swscanf_s(str.c_str(), L"t:%l[^,], v:%l[^\0]", typeStr, MAX_PATH, valStr, len);
#else 
			swscanf_s(str.c_str(), L"t:%l[^,], v:%l[^\0]", typeStr, valStr);
#endif

			int ret = String2Value(typeStr, valStr, v);

			delete[] valStr;
			return ret;
		}
		else {
			NLE_ASSERT(0);
			return EINVAL;
		}

#undef SET_TYPE_X
#undef RTTI_TYPE_DEAL

		return 0;
	}
};

#define NLEVALUE_GET(val, v)	(val).Get(v)
#define NLEVALUE_SET(val, k, v)	(val).Set(k, v)

