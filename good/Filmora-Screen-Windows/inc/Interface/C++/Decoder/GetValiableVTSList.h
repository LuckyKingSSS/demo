#pragma once
typedef void *HVTSAnalysis;
#ifdef __cplusplus
extern "C" 
{
#endif

HVTSAnalysis __stdcall CreateVTSAnalysis();
void		 __stdcall DestroyVTSAnalysis(HVTSAnalysis hVTSAnalysis);
HRESULT		 __stdcall SetAnalysisFold(HVTSAnalysis hVTSAnalysis, const wchar_t *pDVDFoldPath);
HRESULT		 __stdcall GetValiableVTSList(HVTSAnalysis hVTSAnalysis, int *pValiableVTSList, int *pVTSCnt);
HRESULT		__stdcall  GetValiableTitleListInVTS(HVTSAnalysis hVTSAnalysis, int nIFOID, int *pValiableTitleList, int *pTitleCnt);
HRESULT		 __stdcall GetValiableMenuList(HVTSAnalysis hVTSAnalysis, int *pValiableMenuList, int *pMenuCnt);
BOOL		 __stdcall IsTitleUndeletable(HVTSAnalysis hVTSAnalysis, int vts, int title);

#ifdef __cplusplus
}
#endif