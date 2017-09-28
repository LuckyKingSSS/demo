// FileInfoPlugins.h 

#pragma once

#ifndef EXPORT_API
#ifdef __cplusplus
	#define EXPORT_API extern "C"
#else
	#define EXPORT_API
#endif
#endif
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include "FileInfo.h"
#include "StreamReader.h"
#include "DecMgr.h"

typedef MEDIA_FILE_INFO* (__stdcall* GET_MEDIA_FILEINFO_API)(STREAM_READER reader);
typedef IMediaInfo* (__stdcall* Create_MEDIA_INFO_API)(STREAM_READER reader);

struct FileInfoPluginsItem
{
	wchar_t		szID[128];
	DWORD		dwFourCC;
	wchar_t		szExtName[128]; // File Extention Name，例如：L".mp4;.mov;.3gp"

	int			nPriority;

	GET_MEDIA_FILEINFO_API	fnGetMediaFileInfo;
	Create_MEDIA_INFO_API   fnCreateMediaInfo;
};


// 插件接口
// 取得文件信息读取插件详细信息，返回FileInfoPluginsItem列表指针，当dwFourCC为0则表示结束，FourCC必须大写
EXPORT_API const FileInfoPluginsItem* __stdcall GetFileInfoSupport(); // Demux插件必须导出该函数

typedef const FileInfoPluginsItem* (__stdcall* GET_FILEINFO_SUPPORT_API)();


