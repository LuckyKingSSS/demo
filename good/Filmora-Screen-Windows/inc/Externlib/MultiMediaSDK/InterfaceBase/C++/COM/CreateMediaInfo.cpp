#include "stdafx.h"
#include "FileInfoPlugins.h"
#include "GetMediaInfo.h"

MEDIA_FILE_INFO* __stdcall GetFileInfo(STREAM_READER reader);

IMediaInfo * __stdcall CreateMediaInfo(STREAM_READER reader)
{
	MEDIA_FILE_INFO* pMediaInfo = GetFileInfo(reader);
	if (pMediaInfo)
	{
		CMediaInfo *pNewMediaInfo = new CMediaInfo;
		pNewMediaInfo->Fill(pMediaInfo);
		MFIRelease(pMediaInfo);
		pNewMediaInfo->AddRef();
		return static_cast<IMediaInfo *> (pNewMediaInfo);
	}
	return NULL;
}

