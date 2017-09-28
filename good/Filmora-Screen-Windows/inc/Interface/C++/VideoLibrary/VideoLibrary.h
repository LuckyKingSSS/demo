// VideoLibrary.h

#pragma once

#include "WSAudioInfo.h"
#include "WSVideoInfo.h"


typedef void* HVIDEOLIBRARY;

enum VL_SORT_TYPE
{
	VLST_FILENAME,			// 按文件名排序
	VLST_VIDEO_LENGTH,		// 按视频文件长度（时间）排序
};

// 创建一个VideoLibrary
HVIDEOLIBRARY __stdcall VLCreateVideoLibrary();

void __stdcall VLDestroyVideoLibrary(HVIDEOLIBRARY hVidLibrary);

// 打开xml文档，读取xml文件数据对VideoLibrary进行初始化。完成后即关闭该xml文档。
int __stdcall VLLoadXml(HVIDEOLIBRARY hVidLibrary, const wchar_t* pXmlPath);

// 将VideoLibrary保存至指定xml文档，有同名文件时覆盖原文件，无同名文件时创建新文件。
int __stdcall VLSaveXml(HVIDEOLIBRARY hVidLibrary, const wchar_t* pXmlPath);

// 取VideoLibrary中视频总数，返回-1为操作失败
int __stdcall VLGetVideoCount(HVIDEOLIBRARY hVidLibrary);

// 向VideoLibrary指定位置nIndex插入一个视频元素，若nIndex为-1则添加至末尾，返回值为该视频元素在的位置ID，失败返回 -1
int  __stdcall VLInsertVideoItem(HVIDEOLIBRARY hVidLibrary, const wchar_t* pFilePath, int nIndex);

// 从VideoLibrary中移除ID为nIndex的VideoItem，nIndex为-1时则移除所有VideoItem
int __stdcall VLRemoveVideoItem(HVIDEOLIBRARY hVidLibrary, int nIndex);

// 将ID号从nSrcIndex起的nCount个VideoItem移至nDstIndex，以移动前ID为准
int __stdcall VLMoveVideoItem(HVIDEOLIBRARY hVidLibrary, int nDstIndex, int nSrcIndex, int nCount);

// 按eSortType指定key类型进行排序，bAscend为TRUE时为升序，FALSE为反序
int __stdcall VLSort(HVIDEOLIBRARY hVidLibrary, enum VL_SORT_TYPE eSortType, int bAscend);


// 返回ID为nIndex的视频文件的路径（全路径）nFilePathLength是szFilePath的数组元数个数，不是数组空间的大小
int __stdcall VLGetFilePath(HVIDEOLIBRARY hVidLibrary, int nIndex, wchar_t* szFilePath, int nFilePathLength);

int __stdcall VLGetVideoInfo(HVIDEOLIBRARY hVidLibrary, int nIndex, WSVideoInfo* pInfo);
int __stdcall VLSetVideoInfo(HVIDEOLIBRARY hVidLibrary, int nIndex, const WSVideoInfo* pInfo);

int __stdcall VLGetAudioInfo(HVIDEOLIBRARY hVidLibrary, int nIndex, WSAudioInfo* pInfo);
int __stdcall VLSetAudioInfo(HVIDEOLIBRARY hVidLibrary, int nIndex, const WSAudioInfo* pInfo);

// szFileName为缩略图文件名（不含路径）nFileNameLength是szFileName的数组元素个数，不是数组空间的大小
int __stdcall VLGetThumbnailName(HVIDEOLIBRARY hVidLibrary, int nIndex, wchar_t* szFileName, int nFileNameLength);
int __stdcall VLSetThumbnailName(HVIDEOLIBRARY hVidLibrary, int nIndex, const wchar_t* szFileName);

int __stdcall VLGetVideoChangeTime(HVIDEOLIBRARY hVidLibrary, int nIndex, FILETIME* pFileTime);
int __stdcall VLSetVideoChangeTime(HVIDEOLIBRARY hVidLibrary, int nIndex, FILETIME* pFileTime);
