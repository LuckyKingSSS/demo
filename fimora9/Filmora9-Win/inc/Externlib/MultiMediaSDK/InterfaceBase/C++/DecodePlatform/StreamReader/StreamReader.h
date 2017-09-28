// StreamReader.h 

#pragma once


typedef void* HSTREAMHANDLE;

typedef ULONG (__stdcall* STREAM_READ_API)(HSTREAMHANDLE hStream, unsigned char* pData, ULONG dwLength);
typedef int (__stdcall* STREAM_SEEK_API)(HSTREAMHANDLE hStream, __int64 i64Offset, int origin); // If successful, fseek and _fseeki64 returns 0. 
typedef __int64 (__stdcall* STREAM_GETPOS_API)(HSTREAMHANDLE hStream);
typedef __int64 (__stdcall* STREAM_GETLENGTH_API)(HSTREAMHANDLE hStream);
typedef BOOL (__stdcall* STREAM_ENABLEMULTITHREAD_API)(HSTREAMHANDLE hStream, BOOL bEnable);
typedef const wchar_t* (__stdcall* GETFILENAME_API)(HSTREAMHANDLE hStream);


struct STREAM_READER
{
	HSTREAMHANDLE					hStream;

	STREAM_READ_API					fnRead;
	STREAM_SEEK_API					fnSeek;
	STREAM_GETPOS_API				fnGetPos;
	STREAM_GETLENGTH_API			fnGetLength;
	STREAM_ENABLEMULTITHREAD_API	fnEnableMultiThread;
	GETFILENAME_API					fnGetFileName;

	DWORD							dwFourCC;
};

#ifdef __cplusplus

__inline BOOL SREnableThread(STREAM_READER* reader, BOOL bEnable)
{
	if (reader && reader->fnEnableMultiThread)
		return reader->fnEnableMultiThread(reader->hStream, bEnable);

	return FALSE;
}

#endif


enum STREAM_IO_TYPE
{
	SIT_FILE,
	//SIT_URL,
};


STREAM_READER* __stdcall WSIoStreamCreate(STREAM_IO_TYPE uType, const wchar_t* pPathName, BOOL bMultiThread = FALSE);
void __stdcall WSIoStreamDestroy(STREAM_READER* reader);

typedef STREAM_READER* (__stdcall* IOSTREAMCREATE_API)(STREAM_IO_TYPE uType, const wchar_t* pPathName, BOOL bMultiThread);
typedef void (__stdcall* IOSTREAMDESTROY_API)(STREAM_READER* reader);

