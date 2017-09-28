// FileInfoMgr.h 

#pragma once

#include "FileInfo.h"
#include "FileInfoPlugins.h"

#ifdef __cplusplus
extern "C" 
{
#endif

IMediaInfo * __stdcall AVCreateMediaInfo(const wchar_t* pFileName);
MEDIA_FILE_INFO* __stdcall AVGetMediaFileInfo(const wchar_t* pFileName);
void __stdcall AVReleaseMediaFileInfo(MEDIA_FILE_INFO* pMediaInfo);

int __stdcall AVGetFileInfoSupportCount();
const FileInfoPluginsItem* __stdcall AVGetFileInfoSupportItem(int nIndex);

#ifdef __cplusplus
}
#endif

