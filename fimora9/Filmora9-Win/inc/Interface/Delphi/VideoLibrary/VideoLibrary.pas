// VideoLibrary.pas
unit
  VideoLibrary;

interface

uses
  windows, WSVideoInfo, WSAudioInfo;

type
  VL_SORT_TYPE =
  (
    VLST_FILENAME,      // 按文件名排序
	  VLST_VIDEO_LENGTH   // 按视频文件长度（时间）排序
  );

type
  HVIDEOLIBRARY = Pointer;

// 创建一个VideoLibrary
// HVIDEOLIBRARY __stdcall VLCreateVideoLibrary();
function VLCreateVideoLibrary() : HVIDEOLIBRARY; stdcall;

// void __stdcall VLDestroyVideoLibrary(HVIDEOLIBRARY hVidLibrary);
procedure VLDestroyVideoLibrary(hVidLibrary : HVIDEOLIBRARY); stdcall;

// 打开xml文档，读取xml文件数据对VideoLibrary进行初始化。完成后即关闭该xml文档。
// BOOL __stdcall VLLoadXml(HVIDEOLIBRARY hVidLibrary, const wchar_t* pXmlPath);
function VLLoadXml(hVidLibrary : HVIDEOLIBRARY; const pXmlPath : PWideChar) : BOOL; stdcall;

// 将VideoLibrary保存至指定xml文档，有同名文件时覆盖原文件，无同名文件时创建新文件。
// BOOL __stdcall VLSaveXml(HVIDEOLIBRARY hVidLibrary, const wchar_t* pXmlPath);
function VLSaveXml(hVidLibrary : HVIDEOLIBRARY; const pXmlPath : PWideChar) : BOOL; stdcall;

// 取VideoLibrary中视频总数，返回-1为操作失败
// int __stdcall VLGetVideoCount(HVIDEOLIBRARY hVidLibrary);
function VLGetVideoCount(hVidLibrary : HVIDEOLIBRARY) : Integer; stdcall;

// 向VideoLibrary指定位置nIndex插入一个视频元素，若nIndex为-1则添加至末尾，返回值为该视频元素在的位置ID，失败返回 -1
// int  __stdcall VLInsertVideoItem(HVIDEOLIBRARY hVidLibrary, const wchar_t* pFilePath, int nIndex);
function VLInsertVideoItem(hVidLibrary : HVIDEOLIBRARY; const pFilePath : PWideChar; nIndex: Integer) : Integer; stdcall;

// 从VideoLibrary中移除ID为nIndex的VideoItem，nIndex为-1时则移除所有VideoItem
// BOOL __stdcall VLRemoveVideoItem(HVIDEOLIBRARY hVidLibrary, int nIndex);
function VLRemoveVideoItem(hVidLibrary : HVIDEOLIBRARY; nIndex: Integer) : BOOL; stdcall;

// 将ID号从nSrcIndex起的nCount个VideoItem移至nDstIndex，以移动前ID为准
// BOOL __stdcall VLMoveVideoItem(HVIDEOLIBRARY hVidLibrary, int nDstIndex, int nSrcIndex, int nCount);
function VLMoveVideoItem(hVidLibrary : HVIDEOLIBRARY; nDstIndex : Integer; nSrcIndex : Integer; nCount : Integer) : BOOL; stdcall;

// 按eSortType指定key类型进行排序，bAscend为TRUE时为升序，FALSE为反序
function VLSort(hVidLibrary : HVIDEOLIBRARY; eSortType : VL_SORT_TYPE; bAscend : BOOL) : BOOL; stdcall;

// 返回ID为nIndex的视频文件的路径（全路径）
// int __stdcall VLGetFilePath(HVIDEOLIBRARY hVidLibrary, int nIndex, wchar_t* szFilePath, int nFilePathLength);
function VLGetFilePath(hVidLibrary : HVIDEOLIBRARY; nIndex: Integer; szFilePath : PWideChar; nFilePathLength : Integer) : Integer; stdcall;

//
// BOOL __stdcall VLGetVideoInfo(HVIDEOLIBRARY hVidLibrary, int nIndex, VideoInfo* pInfo);
// BOOL __stdcall VLSetVideoInfo(HVIDEOLIBRARY hVidLibrary, int nIndex, const VideoInfo* pInfo);
function VLGetVideoInfo(hVidLibrary : HVIDEOLIBRARY; nIndex: Integer; pInfo : PVideoInfo) : BOOL; stdcall;
function VLSetVideoInfo(hVidLibrary : HVIDEOLIBRARY; nIndex: Integer; const pInfo : PVideoInfo) : BOOL; stdcall;

// BOOL __stdcall VLGetAudioInfo(HVIDEOLIBRARY hVidLibrary, int nIndex, AudioInfo* pInfo);
// BOOL __stdcall VLSetAudioInfo(HVIDEOLIBRARY hVidLibrary, int nIndex, const AudioInfo* pInfo);
function VLGetAudioInfo(hVidLibrary : HVIDEOLIBRARY; nIndex: Integer; pInfo : PAudioInfo) : BOOL; stdcall;
function VLSetAudioInfo(hVidLibrary : HVIDEOLIBRARY; nIndex: Integer; const pInfo : PAudioInfo) : BOOL; stdcall;

// szFileName为缩略图文件名（不含路径）
// int __stdcall VLGetThumbnailName(HVIDEOLIBRARY hVidLibrary, int nIndex, wchar_t* szFileName, int nFileNameLength);
// BOOL __stdcall VLSetThumbnailName(HVIDEOLIBRARY hVidLibrary, int nIndex, const wchar_t* szFileName);
function VLGetThumbnailName(hVidLibrary : HVIDEOLIBRARY; nIndex: Integer; szFileName : PWideChar; nFileNameLength : Integer) : Integer; stdcall;
function VLSetThumbnailName(hVidLibrary : HVIDEOLIBRARY; nIndex: Integer; const pFileName : PWideChar) : BOOL; stdcall;

// 判断文件是否被修改
//BOOL __stdcall VLGetVideoChangeTime(HVIDEOLIBRARY hVidLibrary, int nIndex, FILETIME* pFileTime);
//BOOL __stdcall VLSetVideoChangeTime(HVIDEOLIBRARY hVidLibrary, int nIndex, FILETIME* pFileTime);

function VLGetVideoChangeTime(hVidLibrary : HVIDEOLIBRARY; nIndex : integer; pTime : pFILETIME) : BOOL; stdcall;
function VLSetVideoChangeTime(hVidLibrary : HVIDEOLIBRARY; nIndex : integer; pTime : pFILETIME) : BOOL; stdcall;

implementation

const
    DLLNAME = 'WS_VideoLibrary.dll';

function  VLCreateVideoLibrary    ; external DLLNAME Name 'VLCreateVideoLibrary';
procedure VLDestroyVideoLibrary   ; external DLLNAME Name 'VLDestroyVideoLibrary';
function  VLLoadXml               ; external DLLNAME Name 'VLLoadXml';
function  VLSaveXml               ; external DLLNAME Name 'VLSaveXml';
function  VLGetVideoCount         ; external DLLNAME Name 'VLGetVideoCount';
function  VLInsertVideoItem       ; external DLLNAME Name 'VLInsertVideoItem';
function  VLRemoveVideoItem       ; external DLLNAME Name 'VLRemoveVideoItem';
function  VLMoveVideoItem         ; external DLLNAME Name 'VLMoveVideoItem';
function  VLSort                  ; external DLLNAME Name 'VLSort';

function  VLGetFilePath           ; external DLLNAME Name 'VLGetFilePath';
function  VLGetVideoInfo          ; external DLLNAME Name 'VLGetVideoInfo';
function  VLSetVideoInfo          ; external DLLNAME Name 'VLSetVideoInfo';
function  VLGetAudioInfo          ; external DLLNAME Name 'VLGetAudioInfo';
function  VLSetAudioInfo          ; external DLLNAME Name 'VLSetAudioInfo';
function  VLGetThumbnailName      ; external DLLNAME Name 'VLGetThumbnailName';
function  VLSetThumbnailName      ; external DLLNAME Name 'VLSetThumbnailName';
function  VLGetVideoChangeTime    ; external DLLNAME Name 'VLGetVideoChangeTime';
function  VLSetVideoChangeTime    ; external DLLNAME Name 'VLSetVideoChangeTime';

end.
