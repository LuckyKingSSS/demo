// ExtraData.h 

#pragma once


struct EXTRA_DATA
{
    BYTE*	pData[8];
    int		nSize[8];
};


#ifdef __cplusplus
extern "C" 
{
#endif

EXTRA_DATA* __stdcall MallocExtraData();
void __stdcall ReleaseExtraData(EXTRA_DATA* pExtraData);

BYTE* __stdcall WSMalloc(int size);
void __stdcall WSFree(BYTE* p);


#ifdef __cplusplus
}
#endif
