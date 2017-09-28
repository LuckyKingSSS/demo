unit DVDInfo;

{$WARNINGS OFF}

interface

USES
	FileInfo, Windows;

type
 HIFOINFO = Pointer;


//void __stdcall PDSSetCssSupport(int nCss);//set css support, 0--no support, other--support
	procedure PDSSetCssSupport(nCss: Integer); stdcall;

//HIFOINFO  __stdcall IFOOpen(const wchar_t * pPathFile); //open ifo file, return ifo handle
  function IFOOpen(pPathFile:PWideChar):HIFOINFO;stdcall;

//void  __stdcall IFOClose(HIFOINFO hIfoinfo); // close ifo handle
  procedure IFOClose(hIfoinfo: HIFOINFO ); stdcall;

//int  __stdcall IFOGetHaveCSSProtect(HIFOINFO hIfoinfo); // wether the disc have css protect -1 mean uncertain, 0 mean not, 1 mean have 
   function GetHaveCSSProtect(hIfoinfo: HIFOINFO): Integer; stdcall;

//int  __stdcall IFOGetTitleCount(HIFOINFO hIfoinfo);//get title count of ifo
  function IFOGetTitleCount(hIfoinfo: HIFOINFO): Integer; stdcall;

//int  __stdcall IFOGetChapterCount(HIFOINFO hIfoinfo, int nTitleIndex);//get chapter count
  function IFOGetChapterCount(hIfoinfo: HIFOINFO; nTitleIndex: Integer): Integer; stdcall;

//double  __stdcall IFOGetTitleBeginTime(HIFOINFO hIfoinfo, int nTitleIndex);//get the title begin time
  function IFOGetTitleBeginTime(hIfoinfo: HIFOINFO; nTitleIndex: Integer): Double; stdcall;

//double  __stdcall IFOGetTitleLength(HIFOINFO hIfoinfo, int nTitleIndex);//get the title length
  function IFOGetTitleLength(hIfoinfo: HIFOINFO; nTitleIndex: Integer): Double; stdcall;

//double  __stdcall IFOGetChapterBeginTime(HIFOINFO hIfoinfo, int nTitleIndex, int nChapterIndex);//get the chapter begintime
  function IFOGetChapterBeginTime(hIfoinfo: HIFOINFO; nTitleIndex: Integer; nChapterIndex: Integer): Double; stdcall;

//double  __stdcall IFOGetChapterLength(HIFOINFO hIfoinfo, int nTitleIndex, int nChapterIndex);//get the chapter length
  function IFOGetChapterLength(hIfoinfo: HIFOINFO; nTitleIndex: Integer; nChapterIndex: Integer): Double; stdcall;

//int  __stdcall IFOGetTitleAudioTrackCount(HIFOINFO hIfoinfo, int nTitleIndex);//get audio track count
  function IFOGetTitleAudioTrackCount(hIfoinfo: HIFOINFO; nTitleIndex: Integer):Integer; stdcall;

//const wchar_t *  __stdcall IFOGetAudioTrackDesc(HIFOINFO hIfoinfo, int nTitleIndex, int nAudioTrackidx);//get audio track description
  function IFOGetAudioTrackDesc(hIfoinfo: HIFOINFO; nTitleIndex: Integer; nAudioTrackidx: Integer):PWideChar; stdcall;

//int  __stdcall IFOGetTitleSubTitleCount(HIFOINFO hIfoinfo, int nTitleIndex);//get subtitle count
  function IFOGetTitleSubTitleCount(hIfoinfo: HIFOINFO; nTitleIndex: Integer):Integer; stdcall;

//const wchar_t *  __stdcall IFOGetSubTitleDesc(HIFOINFO hIfoinfo, int nTitleIndex, int nSubTitleidx);//get subtitle description
  function IFOGetSubTitleDesc(hIfoinfo: HIFOINFO; nTitleIndex: Integer; nSubTitleidx: Integer):PWideChar; stdcall;

//WORD __stdcall IFOGetAudioTrackLang(HIFOINFO hIfoinfo, int nTitleIndex, int nAudioTrackidx);
function IFOGetAudioTrackLang(hIfoinfo: HIFOINFO; nTitleIndex: Integer; nAudioTrackidx: Integer): WORD; stdcall;

//const wchar_t * __stdcall IFOGetAudioFormatDesc(HIFOINFO hIfoinfo, int nTitleIndex, int nAudioTrackidx);
function IFOGetAudioFormatDesc(hIfoinfo: HIFOINFO; nTitleIndex: Integer; nAudioTrackidx: Integer):PWideChar; stdcall;

//int __stdcall IFOGetAudioTrackID(HIFOINFO hIfoinfo, int nTitleIndex, int nAudioTrackidx);
function IFOGetAudioTrackID(hIfoinfo: HIFOINFO; nTitleIndex: Integer; nAudioTrackidx: Integer):Integer; stdcall;

//int __stdcall IFOGetAudioChannel(HIFOINFO hIfoinfo, int nTitleIndex, int nAudioTrackidx);
function IFOGetAudioChannel(hIfoinfo: HIFOINFO; nTitleIndex: Integer; nAudioTrackidx: Integer):Integer; stdcall;

//WORD __stdcall IFOGetSubLangCode(HIFOINFO hIfoinfo, int nTitleIndex, int nSubTitleidx);
function IFOGetSubLangCode(hIfoinfo: HIFOINFO; nTitleIndex: Integer; nSubTitleidx: Integer):WORD; stdcall;

//int __stdcall IFOGetSubTitleTrackID(HIFOINFO hIfoinfo, int nTitleIndex, int nSubTitleidx);
function IFOGetSubTitleTrackID(hIfoinfo: HIFOINFO; nTitleIndex: Integer; nSubTitleidx: Integer):Integer; stdcall;

//BOOL __stdcall IFOTitleAvailable(HIFOINFO hIfoinfo, int nTitleIndex)
  function IFOTitleAvailable(hIfoinfo: HIFOINFO; nTitleIndex: Integer):boolean; stdcall;
  
//MEDIA_FILE_INFO* __stdcall IFOGetFileInfo(HIFOINFO hIfoinfo);
function IFOGetFileInfo(hIfoinfo: HIFOINFO): PMEDIA_FILE_INFO; stdcall;

//int __stdcall ISOGetIFOCount(const wchar_t *pPath);  //获得ISO中IFO的数目
function ISOGetIFOCount(pPath: PWideChar): Integer; stdcall;

//BOOL _stdcall ISOGetISOVolumeName(const wchar_t *pPath, wchar_t *pVolumeName); //获取ISO 文件的名称
function ISOGetISOVolumeName(pPath, pVolumeName: PWideChar): BOOL; stdcall;

implementation

const
  DLLNAME = '.\DecPlugins\fdpDVD.dll';

procedure PDSSetCssSupport          ; external DLLNAME index 101;
function  IFOOpen						        ; external DLLNAME Name 'IFOOpen';
procedure IFOClose                  ; external DLLNAME Name 'IFOClose';
function  IFOGetTitleCount			    ; external DLLNAME Name 'IFOGetTitleCount';
function  IFOGetChapterCount        ; external DLLNAME Name 'IFOGetChapterCount';
function  IFOGetTitleBeginTime      ; external DLLNAME Name 'IFOGetTitleBeginTime';
function  IFOGetTitleLength         ; external DLLNAME Name 'IFOGetTitleLength';
function  IFOGetChapterBeginTime    ; external DLLNAME Name 'IFOGetChapterBeginTime';
function  IFOGetChapterLength       ; external DLLNAME Name 'IFOGetChapterLength';
function  IFOGetTitleAudioTrackCount; external DLLName Name 'IFOGetTitleAudioTrackCount';
function  IFOGetAudioTrackDesc      ; external DLLName Name 'IFOGetAudioTrackDesc';
function  IFOGetTitleSubTitleCount  ; External DLLName Name 'IFOGetTitleSubTitleCount';
function  IFOGetSubTitleDesc        ; External DLLName Name 'IFOGetSubTitleDesc';
function 	IFOGetAudioTrackLang			; External DLLName Name 'IFOGetAudioTrackLang';
function 	IFOGetAudioFormatDesc			; External DLLName Name 'IFOGetAudioFormatDesc';
function 	IFOGetAudioTrackID				; External DLLName Name 'IFOGetAudioTrackID';
function  IFOGetAudioChannel				; External DLLName Name	'IFOGetAudioChannel';
function 	IFOGetSubLangCode					; External DLLName Name	'IFOGetSubLangCode';
function  IFOGetSubTitleTrackID			; External DLLName Name 'IFOGetSubTitleTrackID';
function  IFOTitleAvailable         ; External DLLName Name 'IFOTitleAvailable';
function  IFOGetFileInfo		        ; External DLLName Name 'IFOGetFileInfo';
function  ISOGetIFOCount		        ; External DLLName Name 'ISOGetIFOCount';
function ISOGetISOVolumeName		; External DLLName Name 'ISOGetISOVolumeName';

end.
