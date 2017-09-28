
#include "NLEType.h"

#include "NLECommon.h"
#include "NLEKey.h"
#include "INLEProperties.h"
#include "NLEComPtr.h"
#include "NLEMacroDefine.h"
 #include "strconv.h"
#include "INLEFrameRateCalculator.h"
#include "INLESource.h"
#include "INLEMedia.h"
#include "NLEBasePropComPtr.h"
#include "INLEPreProcess.h"

#include <stdio.h>
#include <string.h>

#include "NLEComError.h"

#ifdef WIN32
extern wchar_t g_wszCurDir[];
#else
#include <iconv.h>
#include "WSLog.h"
static wchar_t g_wszCurDir[MAX_PATH] = { 0 };
#endif
#include "INLETimelineInfo.h"


static NLEComPtr<INLEFactory> g_pFactory = NULL;
static NLEComPtr<IThreadPool> g_pPool = NULL;


namespace NLECommon
{	
	NLECOMMON_API void ShowFrame(NLEComPtr<INLEFrame> pFrame)
	{
		BITMAPINFO bmi = { 0 };
        const NLEMediaInfo *pMediaInfo = NULL;
        NLEMediaInfo mediaInfo;
        pFrame->GetMediaInfo(&mediaInfo);
        int iWidth = mediaInfo.video.nWidth;
        int iHeight = mediaInfo.video.nHeight;

		NLE_ASSERT(iWidth > 0 && iHeight > 0);
		bmi.bmiHeader.biWidth = iWidth;
		bmi.bmiHeader.biHeight = iHeight;
		bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biSizeImage = pFrame->GetDataSize();
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biPlanes = 1;
		
        int linesize = iWidth * 4;
        static int nBufSize = linesize * iHeight;
        static uint8_t *pDstData = new uint8_t[nBufSize];
        if (nBufSize < linesize * iHeight)
        {
            SAFE_DELETE(pDstData);
            nBufSize = linesize * iHeight;
            pDstData = new uint8_t[nBufSize];
        }

        //bmp格式需将图像倒置
        uint8_t *pSrc = (uint8_t*)pFrame->GetData();
        uint8_t *pDst = pDstData + linesize * (iHeight - 1);

        for (int h = 0; h < iHeight; h++)
        {
            memcpy(pDst, pSrc, linesize);

            pSrc += linesize;
            pDst -= linesize;
        }
                
//         NLE_ASSERT(0); //we don't should use this function
	}

	NLECOMMON_API UINT  CreateUniqID()
	{
		volatile static UINT sUID = 1000;
		return sUID++;
	}

	NLECOMMON_API GUID  CreateGuid()
    {
        GUID guid;
#ifdef _WIN32
        CoCreateGuid(&guid);
#else
        uuid_generate(reinterpret_cast<unsigned char *>(&guid));
#endif
        return guid;
    }

	NLECOMMON_API NLEComPtr<INLETimelineBase> CreateMediaTimelineFromPath(LPCWSTR pFullFilePath, INLEProperties *pPropSource /*= NULL*/)
	{
		HRESULT hr = E_NLE_UNEXPECTED;
		NLEComPtr<INLETimelineBase> ret = NULL;

		do
		{
			NLEComPtr<INLETimelineBase> pMedia = NULL;
			hr = NLECommon::GetInterface(CLSID_NLEMedia, NULL, 0, IID_INLETimelineBase, (LPVOID *)&pMedia);
			BREAK_IF_FAILED(hr);
			NLEPropDecorator propMedia = NLECommon::NLEGetProperties(pMedia); _ASSERT(propMedia);
			hr = ((NLEBasePropComptr)propMedia)->SetName(L"MediaTimeline");
			if (pPropSource != NULL)
			{
				// 优先设置源的属性
				hr = pPropSource->CopyAll(propMedia);
				BREAK_IF_FAILED(hr);
			}
			hr = propMedia->SetString(NLEKey::Base::kStrFileFullPath, (LPWSTR)pFullFilePath);
			BREAK_IF_FAILED(hr);

			NLEComPtr<INLETimelineInfo> pInfo = NULL;
			hr = QUERY_INTERFACE(pMedia, IID_INLETimelineInfo, &pInfo);
			hr = pInfo->UpdateRenderInfo();
			BREAK_IF_FAILED(hr);

            // 只有音频的则需要将视频隐藏，否则会出现覆盖的问题
            int hasVideo = 0, hasAudio = 0;
            if (FAILED(propMedia.GetInt(NLEKey::Source::kSource_HasVideoStream, &hasVideo)) || !hasVideo) {
                propMedia.SetInt(NLEKey::Render::kRender_VideoEnable, 0);
            }
			if (FAILED(propMedia.GetInt(NLEKey::Source::kSource_HasAudioStream, &hasAudio)) || !hasAudio) {
                propMedia.SetInt(NLEKey::Render::kRender_AudioEnable, 0);
            }
            
			ret = pMedia;

		} while (false);

		if (NULL == ret)
		{
			//文件不存在时，我们创建一个空的timeline，然后返回S_FALSE，以支持当文件不存在时也可以完成工程加载过程
			NLEComPtr<INLETimelineBase> pTimelineBase = NULL;
			hr = NLECommon::GetInterface(CLSID_NLETimelineBase, NULL, 0, IID_INLETimelineBase, (LPVOID *)&pTimelineBase);
			NLEPropDecorator   prop = NLECommon::NLEGetProperties(pTimelineBase);
			_ASSERT(prop);

			if (pPropSource)
			{
				HRESULT hr = S_FALSE;

				NLEComPtr<INLEService> pService;
				hr = pTimelineBase->QueryInterface(IID_INLEService, (void**)&pService); _ASSERT(SUCCEEDED(hr));

				hr = pService->SetReconstructProperties(pPropSource); _ASSERT(SUCCEEDED(hr));
			}
			hr = prop->SetString(NLEKey::Base::kStrFileFullPath, (LPWSTR)pFullFilePath); CHECK(hr);
			hr = prop->SetInt(NLEKey::Source::kSource_HasVideoStream, 0); CHECK(hr);
			hr = prop->SetInt(NLEKey::Source::kSource_HasAudioStream, 0); CHECK(hr);

			ret = pTimelineBase;
		}

		return ret;
	}

	NLECOMMON_API HRESULT WChar2Char(const WCHAR *pWCharIn, char *pCharOut, int nMaxCharCount)
	 {
#ifdef WIN32		 
		int nCount = WideCharToMultiByte(CP_ACP, 0, pWCharIn, -1, pCharOut, nMaxCharCount, NULL, NULL);
		 return nCount > 0 ? S_OK : E_FAIL;
#else		 
		 WtoUTF8(pWCharIn, -1, pCharOut, nMaxCharCount, false);
		 return S_OK;
#endif
	 }

	NLECOMMON_API HRESULT Char2WChar(const char *pCharIn, WCHAR *pWCharOut, int nMaxWCharCount)
	 {
#ifdef WIN32
		 int nCount = MultiByteToWideChar(CP_ACP, 0, pCharIn, -1, pWCharOut, nMaxWCharCount);
		 return nCount > 0 ? S_OK : E_FAIL;
#else
		 //mbstowcs(buf, s, count);
		 UTF8toW(pCharIn, -1, pWCharOut, nMaxWCharCount, false);
		 return S_OK;
#endif
	 }
	 namespace Guid
	 {
		 NLECOMMON_API void RemoveBraces(const wchar_t* pwstrGUID, wchar_t *pwstrOut)
		 {
			 const WCHAR* pStart = wcschr(pwstrGUID, L'{');
			 const WCHAR* pEnd = wcschr(pwstrGUID, L'}');

			 if (pStart && pEnd)
			 {
				 pStart++;
#ifdef WIN32
				 wcsncpy_s(pwstrOut, 128, pStart, pEnd - pStart);
#else
				 wcsncpy_s(pwstrOut, pStart, pEnd - pStart);
#endif
			 }
			 else
			 {
				 wcscpy_s(pwstrOut, 128, pwstrGUID);
			 }
		 }
	 };

	 NLECOMMON_API NLEPropDecorator   CreateProperties()
	{
		NLEPropDecorator  pProp;
		HRESULT hr = GetInterface(CLSID_NLEProperties,NULL, 0, IID_INLEProperties, (LPVOID *)&pProp);
		if (SUCCEEDED(hr) )
		{
			return pProp;
		}
		return NULL;
	}
    
	 NLECOMMON_API NLEComPtr<INLEFactory> GetFactory(LPCWSTR pDirectory /*= NULL*/)
	{
		if (g_pFactory == NULL)
		{
			HRESULT hr; 
			if (FAILED(hr = GetInterface(CLSID_NLEFactory, NULL, 0, IID_INLEFactory, (LPVOID *)&g_pFactory)))
			{
				_ASSERT(FALSE);
				return NULL;
			}
			hr = g_pFactory->Init(pDirectory);
		}
		return g_pFactory;
	}

	 NLECOMMON_API NLEComPtr<IThreadPool> GetThreadPool()
	{
		if (g_pPool == NULL)
		{
			HRESULT hr = NLECommon::GetInterface(CLSID_ThreadPool, NULL, 0, IID_IThreadPool, (LPVOID*)&g_pPool); _ASSERT(SUCCEEDED(hr));

			// 用于提前启动线程池
			do 
			{
				BREAK_IF_FAILED(hr);

				NLEComPtr<INLETask> pTask = NULL;
				hr = g_pPool->FetchTask(&pTask);
				BREAK_IF_FAILED(hr);

				hr = g_pPool->ReturnTask(pTask);
			} while (false);
		}
		return g_pPool;
	}


	namespace Time
	{
#ifdef WIN32
#else
#include <sys/times.h>
		NLElonglong NLEGetMicrosecond()
		{
			tms tm;
			return (times(&tm) * 10000);
		}
#endif
	};

	static WCHAR g_appDir[MAX_PATH] = { 0 };
	static WCHAR g_resourceDir[MAX_PATH] = { 0 };
	static WCHAR g_libraryDir[MAX_PATH] = { 0 };
	static WCHAR g_tempDirForCache[MAX_PATH] = { 0 };
	static WCHAR g_logDir[MAX_PATH] = { 0 };
    
	static WCHAR* GetRuntimePath()
	{
		if (g_wszCurDir[0])
		{
			return g_wszCurDir;
		}

		WCHAR *wszCurPath = g_wszCurDir;

#ifdef _MSC_VER	
		//LOGGER_ERROR_(L"get current directory path failed");
#else
		char exe_path_temp[MAX_PATH] = {0};
		uint32_t executable_length = 0;
		_NSGetExecutablePath(NULL, &executable_length);
		_NSGetExecutablePath(exe_path_temp, &executable_length);
		mbstowcs(wszCurPath, exe_path_temp, executable_length);

		*(wcsrchr(wszCurPath, L'/') + 1) = L'\0';
#endif

		return wszCurPath;
	}

	NLECOMMON_API HRESULT Path::NLESetDirectory(const WCHAR *pDir, eNLEDirType eType)
	{
		RETURN_IF_NULL(pDir, E_INVALIDARG);

		switch (eType)
		{
		case NLE_DIR_TYPE_APP:
			wcscpy_s(g_appDir, MAX_PATH, pDir);
			break;

		case NLE_DIR_TYPE_RESOURCE:
			wcscpy_s(g_resourceDir, MAX_PATH, pDir);
			break;

		case NLE_DIR_TYPE_LIBRARY:
			wcscpy_s(g_libraryDir, MAX_PATH, pDir);
            break;

		case NLE_DIR_TYPE_TEMP_FOR_CACHE:
			wcscpy_s(g_tempDirForCache, MAX_PATH, pDir);
			break;
                
        case NLE_DIR_TYPE_LOG:
            wcscpy_s(g_logDir, MAX_PATH, pDir);
            break;

		default:
			break;
		}

		return S_OK;
	}

	NLECOMMON_API HRESULT Path::NLEGetDirectory(WCHAR *pDir, eNLEDirType eType)
	{
		RETURN_IF_NULL(pDir, E_INVALIDARG);

		switch (eType)
		{
		case NLE_DIR_TYPE_APP:
			if (L'\0' == g_appDir[0])
			{
				LOGGER_WARN_(L"user doesn't set app directory, we use current dir for it");
				WCHAR* p = GetRuntimePath();
				if (p)
				{
					wcscpy_s(g_appDir, MAX_PATH, p);
				}
			}
			wcscpy_s(pDir, MAX_PATH, g_appDir);
			break;

		case NLE_DIR_TYPE_RESOURCE:
			if (L'\0' == g_resourceDir[0])
			{
				LOGGER_WARN_(L"user doesn't set resource directory, we use current dir for it");
				WCHAR* p = GetRuntimePath();
				if (p)
				{
					wcscpy_s(g_resourceDir, MAX_PATH, p);
				}
			}
			wcscpy_s(pDir, MAX_PATH, g_resourceDir);
			break;

		case NLE_DIR_TYPE_LIBRARY:
			if (L'\0' == g_libraryDir[0])
			{
				LOGGER_WARN_(L"user doesn't set library directory, we use current dir for it");
				WCHAR* p = GetRuntimePath();
				if (p)
				{
					wcscpy_s(g_libraryDir, MAX_PATH, p);
				}
			}
			wcscpy_s(pDir, MAX_PATH, g_libraryDir);
			break;

		case NLE_DIR_TYPE_TEMP_FOR_CACHE:
			if (L'\0' == g_tempDirForCache[0])
			{
				//必须要先设置才能使用
				NLE_ASSERT(FALSE); 

				LOGGER_WARN_(L"user doesn't set library directory, we use current dir for it");
				WCHAR* p = GetRuntimePath();
				if (p)
				{
					wcscpy_s(g_tempDirForCache, MAX_PATH, p);
				}
			}
			wcscpy_s(pDir, MAX_PATH, g_tempDirForCache);
			break;

        case NLE_DIR_TYPE_LOG:
            if (L'\0' == g_logDir[0])
            {
#ifdef WIN32
				WCHAR* p = GetRuntimePath();
				if (p)
				{
					wcscpy_s(g_logDir, MAX_PATH, p);
				}
#else
                GetFoldPath(MacApplicationSupport, g_logDir, MAX_PATH);
                wcscat(g_logDir, L"/");
#endif
            }
            wcscpy_s(pDir, MAX_PATH, g_logDir);
            break;
		default:
			break;
		}

		return S_OK;
	}

	
#ifndef WIN32
	static HMODULE MacLoadLibraryW(LPCWSTR lpLibFileName)
	{
        USES_CONVERSION
		char *pName = W2A(lpLibFileName);

//		LOGGER_INFO_(L"load dylib:%ls\n", lpLibFileName);
		HMODULE hMod = dlopen(pName, RTLD_NOW);


		return hMod;
	}
#endif

	static void GetDllPath(WCHAR *pBuf, LPCWSTR dllName)
	{
		NLECommon::Path::NLEGetDirectory(pBuf, NLECommon::Path::NLE_DIR_TYPE_APP);
		wcscat(pBuf, dllName);
#ifndef WIN32
        if (!NLEIsFileExist(pBuf))
        {
            NLECommon::Path::NLEGetDirectory(pBuf, NLECommon::Path::NLE_DIR_TYPE_LIBRARY);
            wcscat(pBuf, dllName);
        }
#endif
	}




	NLECOMMON_API HRESULT GetInterface(REFCLSID rclsid, LPUNKNOWN outer, DWORD context, REFIID riid, LPVOID *ppv)
	{
		HRESULT hr = E_FAIL;
		typedef 	HRESULT(STDAPICALLTYPE* GETOBJECTFN)(REFCLSID rclsid, LPUNKNOWN outer, DWORD context, REFIID riid, LPVOID *ppv);
		static GETOBJECTFN  fnWSCreateInstance = NULL;

		if (!fnWSCreateInstance)
		{
			HMODULE    hDll = NLELoadDllByName(NLEKey::ComSupport::kDllName); NLE_ASSERT(hDll);
			if (NULL == hDll)
			{
				LOGGER_ERROR_(L"load NLEComSupport library failed\n");
			}
			fnWSCreateInstance = (GETOBJECTFN)GetProcAddress(hDll, "WSCreateInstance");
		}

		if (fnWSCreateInstance)
		{
			hr = fnWSCreateInstance(rclsid, outer, CLSCTX_INPROC_SERVER, riid, ppv);
		}

		return hr;
	}

	NLECOMMON_API int UTF8ToW(const char* a_szSrc, const int size, wchar_t* a_szDest, int a_nDestSize)
	{
#ifdef WIN32  
		return MultiByteToWideChar(CP_UTF8, 0, a_szSrc, -1, a_szDest, a_nDestSize);
#else  
		size_t result;
		iconv_t env;
		//int size = strlen(a_szSrc) + 1;
		env = iconv_open("WCHAR_T", "UTF-8");
		if (env == (iconv_t)-1) {
			return -1;
		}
		result = iconv(env, (char**)&a_szSrc, (size_t*)&size, (char**)&a_szDest, (size_t*)&a_nDestSize);
		if (result == (size_t)-1) {
			return -1;
		}
		iconv_close(env);
		return (int)result;
#endif  
	}

	NLECOMMON_API int WToUTF8(const wchar_t* a_szSrc, const int a_nSrcSize, char* a_szDest, int a_nDestSize)
	{
#ifdef WIN32  
		return WideCharToMultiByte(CP_UTF8, 0, a_szSrc, -1, a_szDest, a_nDestSize, NULL, NULL);
#else  
		size_t result;
		iconv_t env;
		env = iconv_open("UTF-8", "WCHAR_T");
		if (env == (iconv_t)-1) {
			return -1;
		}
		result = iconv(env, (char**)&a_szSrc, (size_t*)&a_nSrcSize, (char**)&a_szDest, (size_t*)&a_nDestSize);
		if (result == (size_t)-1) {
			return -1;
		}
		iconv_close(env);
		return (int)result;
#endif  
	}
	 
	NLECOMMON_API HRESULT WChar2CodeStr(const wchar_t *wszSrc, char *szDest, size_t nlen)
	{
		wchar_t *ch = (wchar_t*)wszSrc;
		int pos = 0;
		while (*ch)
		{
			WORD code = (WORD)*ch;
			pos += sprintf_s(szDest + pos, nlen - pos, "%04X", code);
			++ch;
		} 
		return S_OK;
	}

	NLECOMMON_API HRESULT CodeStr2WChar(const char* szSrc, wchar_t* wszDest)
	{
		char ch[5] = { 0 };  // 5 = UNICODE need 4 char + 1 end char
		char *pch = (char*)szSrc;
		int nIdx = 0;
		while (*pch)
		{
#ifdef WIN32
			strncpy_s(ch, pch, 4);
#else
			strncpy(ch, pch, 4);
#endif
			char *offset;
			WORD code = (WORD)strtol(ch, &offset, 16);
			wszDest[nIdx] = (wchar_t)code;
			pch += 4;
			++nIdx;
		}
		return S_OK;
	}


};

NLECOMMON_API HMODULE NLECommon::NLELoadDllByName(const wchar_t *dllName)
{
	wchar_t dllPath[1024] = { 0 };
	GetDllPath(dllPath, dllName);

#ifdef WIN32
	HMODULE hModule = LoadLibraryW(dllPath);
#else
	HMODULE hModule = MacLoadLibraryW(dllPath);
#endif

	if (NULL == hModule)
	{
		LOGGER_ERROR_(L"load library failed[%ls]\n", dllPath);
	}

	return hModule;
}

