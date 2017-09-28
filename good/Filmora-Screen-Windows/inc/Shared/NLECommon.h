#pragma once

#include <string>
#include <algorithm>

#include <objbase.h>
#include "NLEConfig.h"
#include "NLEComPtr.h"
#include "INLEProperties.h"
#include "INLEFrame.h"
#include "NLEMacroDefine.h"
#include "INLERepository.h"
#include "INLETimelineBase.h"
#include "INLEFactory.h"
#include "NLEPropDecorator.h"
#include "INLEBitmap.h"
#include "INLEImageProc.h"
#include "NLECommon_OS.h"
#include "NLECommon_Const.h"
#include "IThreadPool.h"
#include "GetFoldPath.h"
#include <cmath>
#include "INLEEnviroment.h"

#ifndef WIN32
#include <unistd.h>
#endif

namespace NLECommon
{
	NLECOMMON_API HRESULT WChar2Char(const WCHAR *pWCharIn, char *pCharOut, int nMaxCharCount);
	NLECOMMON_API HRESULT Char2WChar(const char *pCharIn, WCHAR *pWCharOut, int nMaxWCharCount);

	NLECOMMON_API int UTF8ToW(const char* a_szSrc, const int size, wchar_t* a_szDest, int a_nDestSize);
	NLECOMMON_API int WToUTF8(const wchar_t* a_szSrc, const int a_nSrcSize, char* a_szDest, int a_nDestSize);

	NLECOMMON_API HRESULT	 WChar2CodeStr(const wchar_t* wszSrc, char* szDest, size_t nlen);
	NLECOMMON_API HRESULT CodeStr2WChar(const char* szSrc, wchar_t* wszDest);

	__inline void StrToLower(WCHAR *pStr)
	{
		std::wstring tempStr(pStr);
		int size = (int)wcslen(pStr) + 1;
		std::transform(tempStr.begin(), tempStr.end(), tempStr.begin(), tolower);
#ifdef WIN32
		wcscpy_s(pStr, size, tempStr.c_str());
#else
		wcscpy(pStr, tempStr.c_str());
#endif // WIN32
	}

	__inline void StrToUpper(WCHAR *pStr)
	{
		std::wstring tempStr(pStr);
		int size = (int)wcslen(pStr) + 1;
		std::transform(tempStr.begin(), tempStr.end(), tempStr.begin(), toupper);
#ifdef WIN32
		wcscpy_s(pStr, size, tempStr.c_str());
#else
		wcscpy(pStr, tempStr.c_str());
#endif // WIN32
	}

	template <typename Type = NLEComPtr<IUnknown> >
	__inline  NLEPropDecorator  NLEGetProperties(Type pUnk)
	{
		NLEPropDecorator  pProperties;
		HRESULT hr = pUnk->QueryInterface(IID_INLEProperties, (LPVOID*)&pProperties); CHECK(hr);
		return pProperties;
	}
	
	template <typename Type = NLEComPtr<IUnknown> >
	__inline  NLEComPtr<INLETimelineBase> NLEGetRenderParent(Type pUnk)
	{
		NLEComPtr<INLETimelineBase> pTimelineBase = NULL;
		HRESULT hr = pUnk->QueryInterface(IID_INLETimelineBase, (LPVOID*)&pTimelineBase);
		return pTimelineBase;
	}

	NLECOMMON_API NLEPropDecorator CreateProperties();
	
	NLECOMMON_API NLEComPtr<INLEFactory> GetFactory(LPCWSTR pDirectory = NULL);

	NLECOMMON_API void ShowFrame(NLEComPtr<INLEFrame> pFrame);

	NLECOMMON_API NLEComPtr<IThreadPool> GetThreadPool();
		
	 ///获取FilterID的对应的Filter的信息
	 __inline NLEPropDecorator GetSpecAudioFilterInfo(LPCWSTR pwszFilterID)
	 {
		 NLEComPtr<INLEProperties> pAudioInfo = GetFactory()->GetRepository()->GetAudioFilterInfo(); 
		 NLE_ASSERT(pAudioInfo);

		 NLEComPtr<IUnknown> pUnk;
		 HRESULT hr = pAudioInfo->GetInterface(pwszFilterID, &pUnk); CHECK(hr);
		 RETURN_IF_FAILED(hr, NULL);
		 NLEComPtr<INLEProperties> pFilterInfo = NULL;
		 hr = pUnk->QueryInterface(IID_INLEProperties, (LPVOID*)&pFilterInfo); CHECK(hr);
		 return pFilterInfo;
	 }

	 __inline std::wstring GetAudioFilterLibFullPath(LPCWSTR pszFilterID)
	 {
		 HRESULT hr = E_FAIL;
		 NLEComPtr<INLERepository> pRepository = NLECommon::GetFactory()->GetRepository();
		 NLEComPtr<INLEProperties> pAudioResourceInfo = pRepository->GetAudioFilterInfo();
		 WCHAR pszFilterDir[256];
		 hr = pAudioResourceInfo->GetString(NLEKey::Repository::kAudioPluginsDir, pszFilterDir, 256); CHECK(hr);
		 INT nFilterCount = 0;
		 pAudioResourceInfo->GetInt(NLEKey::Filter::kFilterCount, &nFilterCount); CHECK(hr); NLE_ASSERT(nFilterCount > 0);

		 NLEComPtr<IUnknown> pUnk;
		 NLEComPtr<INLEProperties> pFilterInfo;
		 hr = pAudioResourceInfo->GetInterface(pszFilterID, &pUnk); CHECK(hr);
		 hr = QUERY_INTERFACE(pUnk, IID_INLEProperties, &pFilterInfo); CHECK(hr);
		 WCHAR pszLibName[50];
		 hr = pFilterInfo->GetString(NLEKey::Filter::kFilterLibName, pszLibName, 50); CHECK(hr);

		 std::wstring strFilterFullPath = pszFilterDir + (std::wstring)pszLibName + NLECommon::OS::GetDynamicLibraryPostfix();
		 return strFilterFullPath;
	 }

	 NLECOMMON_API HMODULE NLELoadDllByName(const wchar_t *dllName);
	 NLECOMMON_API HRESULT GetInterface(REFCLSID rclsid, LPUNKNOWN outer, DWORD context, REFIID riid, LPVOID *ppv);

	__inline  bool IsEqualGUID(const GUID *guid1, const GUID *guid2)
	{
		return memcmp(guid1, guid2, sizeof(GUID)) == 0;
	}

	namespace Time
	{
		/**@brief 秒转换为毫秒*/
		__inline double SecToMSec(double fSecTime)
		{
			return (double)(fSecTime * 1000.0f);
		}
		/**@brief 毫秒转换为纳秒*/
		__inline NLElonglong MSecToNSec(double mSecTime)
		{
			return (NLElonglong)(mSecTime * 10000);

		}
		/**@brief 秒转换为纳秒*/
		__inline NLElonglong SecToNSec(double fSecTime)
		{
			double mSec = SecToMSec(fSecTime);
			return (NLElonglong)MSecToNSec(mSec);
		}
		/**@brief 毫秒转换为秒*/
		__inline double MSecToSec(double mSecTime)
		{
			return (mSecTime / 1000.0f);
		}
		/**@brief 纳秒转换为毫秒*/
		__inline double NSecToMSec(double nSecTime)
		{
			return (nSecTime / 10000.0f);
		}
		/**@brief 纳秒转换为秒*/
		__inline double NSecToSec(double nSecTime)
		{
			double mSec = NSecToMSec(nSecTime);
			return MSecToSec(mSec);
		}
#define NSec2Sec(ns) NLECommon::Time::NSecToSec((ns))
#define Sec2NSec(sec) NLECommon::Time::SecToNSec((sec))
#define MSec2NSec(ms) NLECommon::Time::MSecToNSec((ms))
#define MSec2Sec(ms)  NLECommon::Time::MSecToSec((ms))

#ifdef WIN32
		static LONGLONG GetSecondCount()
		{
			static LARGE_INTEGER liCounter = { 0 };
			if (0 == liCounter.QuadPart)
				QueryPerformanceFrequency(&liCounter);

			return liCounter.QuadPart;
		}

		// unit: Microsecond
		static NLElonglong NLEGetMicrosecond()
		{
			LARGE_INTEGER liCurrent = { 0 };
			QueryPerformanceCounter(&liCurrent);
            return (NLElonglong)(liCurrent.QuadPart * 1000000 / GetSecondCount());
		}

#else
		#define Sleep(a) usleep((a) * 1000)
		NLElonglong NLEGetMicrosecond();		
#endif
		/**@brief 微秒(us)为单位*/
        __inline NLElonglong GetCurrentTimeMicrosecond()
		{
			return NLEGetMicrosecond();
		}

		class CTimeCost
		{
		public:
			CTimeCost()
			{
				beginTime = GetCurrentTimeMicrosecond();
			}
			~CTimeCost()
			{
				endTime = GetCurrentTimeMicrosecond();
				//printf("cost time %d\n", endTime - beginTime);
			}

		private:
            NLElonglong beginTime;
            NLElonglong endTime;
		};
	};
    

	__inline int DecodeCharUTF16(const unsigned short*& pwszUTF16, const unsigned short* pszUTF16End/*=NULL*/)
	{
		// Return Unicode code point and increment pwszUTF16 past 1 or 2 (if surrogrates) UTF-16 code points
		// pszUTF16End can be NULL if pszUTF16 is zero terminated
		int nUChar = *pwszUTF16;
		++pwszUTF16;
		if ((nUChar & ~0x000007ff) == 0xd800) // W1
		{
			if (pwszUTF16 == pszUTF16End || !(*pwszUTF16)) // W2
				return -1; // incorrect UTF-16
			nUChar = (((nUChar & 0x3ff) << 10) | (*pwszUTF16 & 0x3ff)) + 0x10000;
			++pwszUTF16;
		}
		return nUChar;
	}

	__inline void EncodeCharUTF16(int nUChar, unsigned short* pwszUTF16, int& nUTF16Len)
	{
		// Write UTF-16 sequence to pwszUTF16 for Unicode code point nUChar and update nUTF16Len
		// Be sure pwszUTF16 has room for up to 2 wide chars
		if (nUChar & ~0xffff)
		{
			if (pwszUTF16)
			{
				// Surrogate pair
				nUChar -= 0x10000;
				pwszUTF16[nUTF16Len++] = (unsigned short)(((nUChar >> 10) & 0x3ff) | 0xd800); // W1
				pwszUTF16[nUTF16Len++] = (unsigned short)((nUChar & 0x3ff) | 0xdc00); // W2
			}
			else
				nUTF16Len += 2;
		}
		else
		{
			if (pwszUTF16)
				pwszUTF16[nUTF16Len++] = (unsigned short)nUChar;
			else
				++nUTF16Len;
		}
	}

    __inline int DecodeCharUTF32( const unsigned int*& pwszUTF32, const unsigned int* pszUTF32End/*=NULL*/ )
    {
        int nUChar = *pwszUTF32;
        ++pwszUTF32;
        if ( (nUChar & ~0x000007ff) == 0xd800 ) // W1
        {
            if ( pwszUTF32 == pszUTF32End || ! (*pwszUTF32) ) // W2
                return -1; // incorrect UTF-16
            nUChar = (((nUChar & 0x3ff) << 10) | (*pwszUTF32 & 0x3ff)) + 0x10000;
            ++pwszUTF32;
        }
        return nUChar;
    }
    
    __inline void EncodeCharUTF8( int nUChar, char* pszUTF8, int& nUTF8Len )
    {
        // Write UTF-8 sequence to pszUTF8 for Unicode code point nUChar and update nUTF8Len
        // Be sure pszUTF8 has room for up to 4 bytes
        if ( ! (nUChar & ~0x0000007f) ) // < 0x80
        {
            if ( pszUTF8 )
                pszUTF8[nUTF8Len++] = (char)nUChar;
            else
                ++nUTF8Len;
        }
        else if ( ! (nUChar & ~0x000007ff) ) // < 0x800
        {
            if ( pszUTF8 )
            {
                pszUTF8[nUTF8Len++] = (char)(((nUChar&0x7c0)>>6)|0xc0);
                pszUTF8[nUTF8Len++] = (char)((nUChar&0x3f)|0x80);
            }
            else
                nUTF8Len += 2;
        }
        else if ( ! (nUChar & ~0x0000ffff) ) // < 0x10000
        {
            if ( pszUTF8 )
            {
                pszUTF8[nUTF8Len++] = (char)(((nUChar&0xf000)>>12)|0xe0);
                pszUTF8[nUTF8Len++] = (char)(((nUChar&0xfc0)>>6)|0x80);
                pszUTF8[nUTF8Len++] = (char)((nUChar&0x3f)|0x80);
            }
            else
                nUTF8Len += 3;
        }
        else // < 0x110000
        {
            if ( pszUTF8 )
            {
                pszUTF8[nUTF8Len++] = (char)(((nUChar&0x1c0000)>>18)|0xf0);
                pszUTF8[nUTF8Len++] = (char)(((nUChar&0x3f000)>>12)|0x80);
                pszUTF8[nUTF8Len++] = (char)(((nUChar&0xfc0)>>6)|0x80);
                pszUTF8[nUTF8Len++] = (char)((nUChar&0x3f)|0x80);
            }
            else
                nUTF8Len += 4;
        }
    }
    
    __inline int UTF32To8( char* pszUTF8, const unsigned int* pwszUTF32, int nUTF8Count )
    {
        int nUChar, nUTF8Len = 0;
        while ( *pwszUTF32 )
        {
            // Decode UTF-16
            nUChar = DecodeCharUTF32( pwszUTF32, NULL );
            if ( nUChar == -1 )
                nUChar = '?';
            
            // Encode UTF-8
            if ( pszUTF8 && nUTF8Len + 4 > nUTF8Count )
            {
                int nUTF8LenSoFar = nUTF8Len;
                EncodeCharUTF8( nUChar, NULL, nUTF8Len );
                if ( nUTF8Len > nUTF8Count )
                    return nUTF8LenSoFar;
                nUTF8Len = nUTF8LenSoFar;
            }
            EncodeCharUTF8( nUChar, pszUTF8, nUTF8Len );
        }
        if ( pszUTF8 && nUTF8Len < nUTF8Count )
            pszUTF8[nUTF8Len] = 0;
        return nUTF8Len;
    }
    
    __inline int DecodeCharUTF8( const char*& pszUTF8, const char* pszUTF8End/*=NULL*/ )
    {
        // Return Unicode code point and increment pszUTF8 past 1-4 bytes
        // pszUTF8End can be NULL if pszUTF8 is null terminated
        int nUChar = (unsigned char)*pszUTF8;
        ++pszUTF8;
        if ( nUChar & 0x80 )
        {
            int nExtraChars;
            if ( ! (nUChar & 0x20) )
            {
                nExtraChars = 1;
                nUChar &= 0x1f;
            }
            else if ( ! (nUChar & 0x10) )
            {
                nExtraChars = 2;
                nUChar &= 0x0f;
            }
            else if ( ! (nUChar & 0x08) )
            {
                nExtraChars = 3;
                nUChar &= 0x07;
            }
            else
                return -1;
            while ( nExtraChars-- )
            {
                if ( pszUTF8 == pszUTF8End || ! (*pszUTF8 & 0x80) )
                    return -1;
                nUChar = nUChar<<6;
                nUChar |= *pszUTF8 & 0x3f;
                ++pszUTF8;
            }
        }
        return nUChar;
    }
    
    __inline int UTF8To32( unsigned int* pwszUTF32, const char* pszUTF8, int nUTF8Count )
    {
        const char* pszPosUTF8 = pszUTF8;
        const char* pszUTF8End = pszUTF8 + nUTF8Count;
        int nUChar, nUTF8Len = 0, nUTF32Len = 0;
        while ( pszPosUTF8 != pszUTF8End )
        {
            nUChar = DecodeCharUTF8( pszPosUTF8, pszUTF8End );
            if ( ! nUChar )
            {
                if ( pwszUTF32 )
                    pwszUTF32[nUTF32Len] = 0;
                break;
            }
            else if ( nUChar == -1 )
                nUChar = '?';
            
            if ( pwszUTF32 )
                pwszUTF32[nUTF32Len++] = nUChar;
            else
                ++nUTF32Len;
        }
        nUTF8Len = (int)(pszPosUTF8 - pszUTF8);
        if ( ! pwszUTF32 )
            return nUTF32Len;
        return nUTF8Len;
    }
		
	__inline int UTF16To8(char *pszUTF8, const unsigned short* pwszUTF16, int nUTF8Count)
	{
		int nUChar, nUTF8Len = 0;
		while (*pwszUTF16)
		{
			// Decode UTF-16
			nUChar = DecodeCharUTF16(pwszUTF16, NULL);
			if (nUChar == -1)
				nUChar = '?';

			// Encode UTF-8
			if (pszUTF8 && nUTF8Len + 4 > nUTF8Count)
			{
				int nUTF8LenSoFar = nUTF8Len;
				EncodeCharUTF8(nUChar, NULL, nUTF8Len);
				if (nUTF8Len > nUTF8Count)
					return nUTF8LenSoFar;
				nUTF8Len = nUTF8LenSoFar;
			}
			EncodeCharUTF8(nUChar, pszUTF8, nUTF8Len);
		}
		if (pszUTF8 && nUTF8Len < nUTF8Count)
			pszUTF8[nUTF8Len] = 0;
		return nUTF8Len;
	}

	__inline int UTF8To16(unsigned short* pwszUTF16, const char* pszUTF8, int nUTF8Count)
	{
		const char* pszPosUTF8 = pszUTF8;
		const char* pszUTF8End = pszUTF8 + nUTF8Count;
		int nUChar, nUTF8Len = 0, nUTF16Len = 0;
		while (pszPosUTF8 != pszUTF8End)
		{
			nUChar = DecodeCharUTF8(pszPosUTF8, pszUTF8End);
			if (!nUChar)
			{
				if (pwszUTF16)
					pwszUTF16[nUTF16Len] = 0;
				break;
			}
			else if (nUChar == -1)
				nUChar = '?';

			// Encode UTF-16
			EncodeCharUTF16(nUChar, pwszUTF16, nUTF16Len);
		}
		nUTF8Len = (int)(pszPosUTF8 - pszUTF8);
		if (!pwszUTF16)
			return nUTF16Len;
		return nUTF8Len;
	}

	namespace Math
	{
		/**
		 *@brief 求m和n的最大公约数,例如100和60的最大公约数为20
		 *@note 辗转相除法求最大公约数
		 */
		NLECOMMON_API NLElonglong GetGCD(NLElonglong m, NLElonglong n);
		/**
		 *@brief 约分，例如输入值为(10, 20),则输出为(1,2)
		 */
		NLECOMMON_API void ReduceRational(const NLERational& ralIn, NLERational* pRalOut);
		 /**
		  *@brief 精确到小数后多少位
		  */
		NLECOMMON_API void CutRational(const NLERational &ralIn, NLERational *pRalOut, unsigned int nBits = 2);
	}
//     
// 
//     __inline int64_t PTSToFrameNumber(int64_t pts, double framerate)
//     {
//         double nFrameNumber = pts* framerate / S_TO_100NS;
//         return (int64_t)(nFrameNumber + 0.5);
//     }

	__inline void VideoFrameCopy(NLEComPtr<INLEFrame> pDst, NLEComPtr<INLEFrame> pSrc)
	{
		LPVOID pDataSrc = pSrc->GetData();
		LPVOID pDataData = pDst->GetData();

        NLEMediaInfo srcMedia, dstMedia;
        pSrc->GetMediaInfo(&srcMedia);
        pDst->GetMediaInfo(&dstMedia);
        NLE_ASSERT(dstMedia.video.nWidth == srcMedia.video.nWidth);
		NLE_ASSERT(dstMedia.video.nHeight == srcMedia.video.nHeight);
		NLE_ASSERT(pSrc->GetDataSize() == pDst->GetDataSize());

		memcpy(pDataData, pDataSrc, pDst->GetDataSize());
	}

	__inline std::wstring ReplaceEnvByValue(std::wstring &envStr)
	{
		std::wstring valueStr;
		NLEComPtr<INLEEnviroment> pEnv = NULL;
		NLECommon::GetInterface(CLSID_NLEEnviroment, NULL, 0, IID_INLEEnviroment, (void**)&pEnv);
		pEnv->ReplaceEnvByValue(envStr, valueStr);
		return valueStr;
	}

	namespace Path
	{
		__inline int UPathToAPath(const wchar_t* pwszSrc, char* pszDst, int nDstLen)
		{
#ifdef WIN32
			return WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nDstLen, NULL, NULL);
#else
			return UTF32To8(pszDst, (const unsigned int*)pwszSrc, nDstLen);
#endif
		}

		__inline int APathToUPath(const char* pszSrc, wchar_t* pwszDst, int nDstLen)
		{
#ifdef WIN32
			return MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nDstLen);
#else
			return UTF8To32((unsigned int*)pwszDst, pszSrc, strlen(pszSrc));
#endif
		}

		__inline std::wstring GuidToString(const GUID &guid)
		{
			wchar_t buf[64] = { 0 };
			swprintf(buf, sizeof(buf), L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
				guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1],
				guid.Data4[2], guid.Data4[3],
				guid.Data4[4], guid.Data4[5],
				guid.Data4[6], guid.Data4[7]);
			return std::wstring(buf);
		}

		__inline HRESULT GetAudioFilterFullPath(LPCWSTR pszFilterID, WCHAR *pszFullPath, int nBufferSize = 256)
		{
			NLEComPtr<INLEProperties> pAudioInfo = GetFactory()->GetRepository()->GetAudioFilterInfo();
			WCHAR szTmp[256] = { 0 };
			HRESULT hr = pAudioInfo->GetString(NLEKey::Repository::kAudioPluginsDir, szTmp, 256); CHECK(hr);
			RETURN_IF_FAILED(hr, hr);
			wcscpy_s(pszFullPath, nBufferSize, szTmp);

			NLEComPtr<IUnknown> pUnk;
			hr = pAudioInfo->GetInterface(pszFilterID, &pUnk); CHECK(hr);
			RETURN_IF_FAILED(hr, hr);
			NLEComPtr<INLEProperties> pFilterInfo = NULL;
			pUnk->QueryInterface(IID_INLEProperties, (LPVOID*)&pFilterInfo);

			hr = pFilterInfo->GetString(NLEKey::Filter::kFilterLibName, szTmp, 256);
			RETURN_IF_FAILED(hr, hr);

#ifdef WIN32
			wcscat_s(pszFullPath, nBufferSize, szTmp);
			wcscat_s(pszFullPath, nBufferSize, OS::GetDynamicLibraryPostfix());
#else
			wcscat(pszFullPath, szTmp);
			wcscat(pszFullPath, OS::GetDynamicLibraryPostfix());
#endif
			return S_OK;
		}

		__inline bool IsFileExist(LPCWSTR pszFilePath)
		{
			bool bFileExit = false;

#if 1 //使用环境变量
			NLEComPtr<INLEEnviroment> pEnv = NULL;
			NLECommon::GetInterface(CLSID_NLEEnviroment, NULL, 0, IID_INLEEnviroment, (void**)&pEnv);
			std::wstring envStr = pszFilePath;
			std::wstring fullPath;
			pEnv->ReplaceEnvByValue(envStr, fullPath);
#endif

#ifdef WIN32
			bFileExit = _waccess(fullPath.c_str(), 0) != -1;
#else
			bFileExit = NLEIsFileExist(fullPath.c_str());
#endif
			return bFileExit;
		}
        
		__inline bool IsDirExist(const wchar_t* pszFilePath)
		{
			CHECK_POINTER(pszFilePath, FALSE);

#if 1 //使用环境变量
			NLEComPtr<INLEEnviroment> pEnv = NULL;
			NLECommon::GetInterface(CLSID_NLEEnviroment, NULL, 0, IID_INLEEnviroment, (void**)&pEnv);
			std::wstring envStr = pszFilePath;
			std::wstring fullPath;
			pEnv->ReplaceEnvByValue(envStr, fullPath);
#endif

#ifdef WIN32
			//Win使用_wfopen是为了解决fopen无法打开英文系统的中文路径问题

			DWORD dwAttributes = GetFileAttributes(fullPath.c_str());
			return (dwAttributes != -1 && (dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
#else
			char _fullpath[MAX_PATH] = "\0";
			WChar2Char(fullPath.c_str(), _fullpath, MAX_PATH);
			struct stat sb;
			int ret = stat(_fullpath, &sb);
            return (ret >= 0 && (S_IFDIR & sb.st_mode) == S_IFDIR);
#endif
		}

		inline bool IsImageFile(LPCWSTR pFileName)
		{
			CHECK_POINTER(pFileName, FALSE);
			bool bRet = false;

//            std::wstring strFileName = pFileName;
//            strFileName.find_last_of(L'.');
            const wchar_t *dotptr = std::wcsrchr(pFileName, L'.');
			if (dotptr == NULL)
			{
				return false;
			}

			std::wstring pszExt = dotptr;
			std::wstring strExt;// = _wcslwr((wchar_t *)pszExt);
			std::transform(pszExt.begin(), pszExt.end(), pszExt.begin(), towlower);
			strExt = pszExt.c_str();
			strExt += L";";
			std::wstring strAllImageFormat = L"*.bmp;*.jpg;*.jpeg;*.jp2;*.png;*.ico;*.html;*.info;*.miff;*.m2v;*.mpc;*.ico;"
				L"*.mtv;*.pdb;*.rgb;*.sfw;*.sgi;*.tif;*.tiff;*.uyvy;*.viff;*.ycbcr;*.yuv;*.emf;*.cmyk;*.dot;*.fax;"
				L"*.art;*.jng;*.pcl;*.map;*.win;*.logo;*.rose;*.app1;*.icc;*.tile;*.vid;*.picon;*.scan;*.cur;*.wmf;*.avs;*.cgm;"
				L"*.cin;*.crw;*.cut;*.dcm;*.dcr;*.dcx;*.dib;*.dng;*.dpx;*.epi;*.eps;*.ept;*.msl;*.mvg;*.man;*.mat;*.mng;"
				L"*.sun;*.svg;*.tga;*.tim;*.vmf;*.dng;*.pict;*.print;*.pnm;*.ppm;*.fig;*.epdf;*.pbm;*.pcx;*.ps;"
				L"*.cr2;*.djvu;*.exr;*.fits;*.fpx;*.gplt;*.gray;*.jpc;*.mrw;*.otb;*.pam;*.pef;*.pfm;*.pxi;*.psd;"
				L"*.pwp;*.raf;*.rla;*.xbm;*.xpm;*.xwd;*.x3f;*.iptc;*.jfif;*.jpe;";

            if (std::wcsstr(strAllImageFormat.c_str(), strExt.c_str()))
			{
				bRet = true;
			}

			return bRet;
		}

        __inline bool ConvertToUnixPath(LPWSTR path)
        {
            if (!path)
                return false;
            
            wchar_t *p = path;
            while (*p)
            {
                if (*p == L'\\')
                {
                    *p = L'/';
                }
                p++;
            }
            return true;
        }

		typedef enum eNLEDirType
		{
			NLE_DIR_TYPE_APP = 1, //程序运行目录
			NLE_DIR_TYPE_RESOURCE = 2, //资源目录
			NLE_DIR_TYPE_LIBRARY = 3, //其他依赖库目录
			NLE_DIR_TYPE_TEMP_FOR_CACHE = 4, //用于缓存的临时目录
            NLE_DIR_TYPE_LOG = 5, //日志目录
		}eNLEDirType;
		/**
		设置和获取程序的相关目录,目录以 '/'或'\\'分隔符结尾
		*/
		NLECOMMON_API HRESULT NLESetDirectory(const WCHAR *pDir, NLECommon::Path::eNLEDirType eType);
		NLECOMMON_API HRESULT NLEGetDirectory(WCHAR *pDir, NLECommon::Path::eNLEDirType eType);

	}

	__inline int CalcAuidoFrameSize(NLERational framerate, int nBytesPerSec, int index)
	{
		int64_t nCurTotalCount = (int64_t)((int64_t)index * nBytesPerSec / framerate + 0.5);
		int64_t nNextTotalCount = (int64_t)((int64_t)(index + 1) * nBytesPerSec / framerate + 0.5);

		int nCount = (int)( (nNextTotalCount & ~3) - (nCurTotalCount & ~3) );
#ifdef _DEBUG
		double dTheorySize = nBytesPerSec / framerate;
		NLE_ASSERT(abs(dTheorySize - nCount * 1.0) < 4.0);
#endif
		return nCount;
	}

	__inline NLEFrameIndex CalcAudioFrameIndex(NLEFrameIndex srcIndex, NLERational srcFramerate, NLERational dstFramerate)
	{
		return NLEFrameIndex(srcIndex * dstFramerate / srcFramerate + 0.5f);
	}

	////////////////////////////////EXPORT////////////////////////////////////////// 
	NLECOMMON_API UINT  CreateUniqID();
	NLECOMMON_API GUID  CreateGuid();
	
	namespace Guid
	{
		/** "{4CB86487-E934-459c-8484-A746CF7E257D}.xxx" 变换为
		 *   "4CB86487-E934-459c-8484-A746CF7E257D"
		 */
		NLECOMMON_API void RemoveBraces(const wchar_t* pwstrGUID, wchar_t *pwstrOut);
	};

	NLECOMMON_API NLEComPtr<INLETimelineBase> CreateMediaTimelineFromPath(LPCWSTR pFullFilePath, INLEProperties *pPropSource = NULL);
};
