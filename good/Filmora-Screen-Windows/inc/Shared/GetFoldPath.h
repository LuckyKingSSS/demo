/* ### WS@H Project:NLE ### */
#pragma once

#ifndef WIN32
//typedef void* HDIBIMAGE;

enum MacDirectoryType{
	MacUserHome,
	MacTempDir,
	MacApplicationLibrary,
	MacApplicationCache,
	MacApplicationSupport,
    MacAppResourcePath,
    MacDocumentDirectPath,
};

enum MacStoreFileType{
	MacStoreSucessType,
	MacStoreNoReadorWriteFileType,
	MacStoreFailedType
};


int GetFoldPath(int nFlag, wchar_t* szPath, int nPathBufferSize);
int CopyDocmentMedias(const wchar_t* szSrcFilePath, const wchar_t* szDstFilePath);
void setMacAppstore(bool appstore);
int isAuthoredPath(const wchar_t* szSrcFilePath);
MacStoreFileType storeFileURL(const wchar_t* szSrcFilePath);
void getSongPathStoreBookmark(const wchar_t* szFilePath, wchar_t* szSongPath);
bool NLEIsFileExist(const wchar_t* pwszFileName);
wchar_t* NLEGetFileName(const wchar_t* pwszFilePath);
void GetSystemVersion(int* majorVersion, int* minorVersion, int* bugFixVersion);
void GetScreenSize(int* width, int* height);
bool CreateDirectory(const wchar_t* szPath,int nFlag);
void copyStringToClippoard(const wchar_t* wszVal);
wchar_t* stringFromClippoard();
bool NLECopyFile(const wchar_t* szSrcFilePath, const wchar_t* szDstFilePath);
bool NLEDeleteFile(const wchar_t* szFilePath);
//bool NLEImageLoadFile(HDIBIMAGE pImage, const wchar_t* pFileName);
#endif
