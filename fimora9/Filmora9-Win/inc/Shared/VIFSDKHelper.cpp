#include "stdafx.h"
#include "NLEType.h"
#include "VIFSDKHelper.h"
#include "VIFSDK.h"
#include "NLECommon.h"
#include "INLERepository.h"
#include "NLEKey.h"
#include "CFactory.h"

//using namespace VIFSDK;

CVIFSDKHelper* CVIFSDKHelper::m_pSDKHelperInstance = NULL;

CVIFSDKHelper::CVIFSDKHelper(LPCWSTR pDir, LPCWSTR pLogDir)
{
	char pszDir[MAX_PATH], pszLogDir[MAX_PATH];
	NLECommon::WChar2Char(pDir, pszDir, MAX_PATH);
	NLECommon::WChar2Char(pLogDir, pszLogDir, MAX_PATH);
	VIFSDK::VIRESULT vr = VIFSDK::InitializeEngine(pszDir, pszLogDir);
	_ASSERT(vr == 0);
}


CVIFSDKHelper::~CVIFSDKHelper()
{
	VIFSDK::UninitializeEngine();
}

CVIFSDKHelper* CVIFSDKHelper::GetVIFSDKHelper(LPCWSTR pDir)
{
	if (NULL == m_pSDKHelperInstance)
	{
		NLEComPtr<INLERepository> pRepository;

	
		HRESULT hr = NLECommon::GetInterface(CLSID_NLERepository, NULL, 0, IID_INLERepository, (LPVOID *)&pRepository);
		_ASSERT(SUCCEEDED(hr));

        pRepository->Initialize(NULL);

		NLEComPtr<INLEProperties> pFilterInfo = pRepository->GetVideoFilterInfo();
		_ASSERT(pFilterInfo);

		WCHAR pszEffectEnginDir[MAX_PATH];
		hr = pFilterInfo->GetString(NLEKey::Repository::kEffectEngineDir, pszEffectEnginDir, MAX_PATH);
		_ASSERT(SUCCEEDED(hr));

		m_pSDKHelperInstance = new CVIFSDKHelper(pszEffectEnginDir, pRepository->GetLoggerPath());
	}
	return m_pSDKHelperInstance;
}
