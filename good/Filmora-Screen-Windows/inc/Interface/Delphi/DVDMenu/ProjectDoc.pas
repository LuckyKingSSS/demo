
unit
	ProjectDoc;

{$MINENUMSIZE 4}

interface

uses
    Windows, DVDMenuItem, EncParam, TClip;

type
  TClipInfo = record
    uType         :	ClipType;		// Clip类型
    dBeginTime    :	double;			// 在Timeline上的起始时间
    dLength       :	double;			// 在Timeline上持续时间长度
    dTrimFrom     :	double;

    szFileName    : array[0..512 - 1] of WideChar;	//
    szTransID     :	array[0..64 - 1] of WideChar;	// 尾部转场ID

    dAudioFadeIn  :	double;			// 音频淡入（秒）
    dAudioFadeOut :	double;			// 音频淡出

    nVolume       :	integer;		// 音量,0~200, 100保持原音量
    bAudioMute    :	BOOL;
    nBrightness   :	integer;		// 亮度，-100～100，0保持原亮度
    nContrast     :	integer;		// 对比度，-100～100，0保持原对比度
    nSaturation   :	integer;		// 饱和度,-100～100，0保持原饱和度
    bUseCropRect  :	BOOL;
    rcCrop        :	TRECT;

    uResizeStyle  :	IMAGE_RESIZE_METHOD;
    dwTransLength : DWORD;
	end;
type
  PTClipInfo = ^TClipInfo;
type
  HPROJDOC = pointer;

type
  DvdBurnerParam = record
	szTempPath		: array[0..512-1] of WideChar;
	szDVDFolder		: array[0..512-1] of WideChar;
	szIsoFileName	: array[0..512-1] of WideChar;
	szVolumeLabel	: array[0..64-1] of WideChar;
	bBurnToIso		: BOOL;
	bSaveDVDFolder	: BOOL;
	nBurnDriver		: integer;
	nBurnEngine		: integer;
	dwNorm			: DWORD;     // 电视制式                            
	dwAspect		: DWORD;   // 比例     
	dwDiskType		: DWORD;	// 磁盘类型
	dwQuality		: DWORD;
	end;
	
type PDvdBurnerParam = ^DvdBurnerParam;

//初始化TClipInfo结构体
//void __stdcall PDInitClipInfo(TClipInfo* pClipInfo);
procedure PDInitClipInfo(pClipInfo: PTClipInfo); stdcall;

//HPROJDOC __stdcall PDCreateProjDoc();
function PDCreateProjDoc(): HPROJDOC ; stdcall;

//void __stdcall PDDestroyProjDoc(HPROJDOC hProjDoc);
procedure PDDestroyProjDoc(hProjDoc: HPROJDOC); stdcall;

//添加一个Slideshow
//int __stdcall PDAddSlideshow(HPROJDOC hProjDoc, const EncodeParam* pEncParam);
function PDAddSlideshow(hProjDoc: HPROJDOC; pEncParam: PEncodeParam): integer; stdcall;

//设置ClipInfo
//int __stdcall PDAddClip(HPROJDOC hProjDoc, int nSlideIndex, const TClipInfo* pClipInfo);
function PDAddClip(hProjDoc: HPROJDOC; nSlideIndex: integer; pClipInfo: PTClipInfo): integer; stdcall;

//获取Slideshow个数
//int __stdcall PDGetSlideshowCount(HPROJDOC hProjDoc);
function PDGetSlideshowCount(hProjDoc: HPROJDOC): integer; stdcall;

//获取Slideshow信息
//BOOL __stdcall PDGetSlideshowInfo(HPROJDOC hProjDoc, int nSlideIndex, EncodeParam* pEncParam);
function PDGetSlideshowInfo(hProjDoc: HPROJDOC; nSlideIndex: integer; pEncParam: PENCODEPARAM): BOOL; stdcall;

//获取Slideshow包含的Clip个数
//int __stdcall PDGetClipCount(HPROJDOC hProjDoc, int nSlideIndex);
function PDGetClipCount(hProjDoc: hProjDoc; nSlideIndex: Integer): integer; stdcall;

//获取ClipInfo
//BOOL __stdcall PDGetClipInfo(HPROJDOC hProjDoc, int nSlideIndex, int nClipIndex, TClipInfo* pClipInfo);
function PDGetClipInfo(hProjDoc: HPROJDOC; nSlideIndex: integer; nClipIndex: integer; pClipInfo: PTClipInfo): BOOL; stdcall;

//---
//BOOL __stdcall PDSaveToFile(HPROJDOC hProjDoc, LPCWSTR pszFilePath);
function PDSaveToFile(hProjDoc: HPROJDOC; pszFile: PWideChar): BOOL; stdcall;

//BOOL __stdcall PDLoadFromFile(HPROJDOC hProjDoc, LPCWSTR pszFilePath);
function PDLoadFromFile(hProjDoc: HPROJDOC; pszFilePath: PWideChar): BOOL; stdcall;

//BOOL __stdcall PDSetMenuXml(HPROJDOC hProjDoc, LPCWSTR pszXml);
function PDSetMenuXml(hProjDoc: HPROJDOC; const pszXml: PWideChar): BOOL; stdcall;

//int __stdcall PDGetMenuXml(HPROJDOC hProjDoc, LPWSTR pszXml, int nLen);
function PDGetMenuXml(hProjDoc: HPROJDOC; pszXml: PWideChar; nLen: integer): integer; stdcall;

//BOOL __stdcall PDSetProjSettingXml(HPROJDOC hProjDoc, LPCWSTR pszXml);
function PDSetProjSettingXml(hProjDoc: HPROJDOC; pszXml: PWideChar): BOOL; stdcall;

//int __stdcall PDGetProjSettingXml(HPROJDOC hProjDoc, LPWSTR pszXml, int nLen);
function PDGetProjSettingXml(hProjDoc: HPROJDOC; pszXml: PWideChar; nLen: integer): integer; stdcall;

//BOOL __stdcall PDSetDvdBurnerParam(HPROJDOC hProjDoc, const DvdBurnParam* pBurnerParam);
function PDSetDvdBurnerParam(hPD: HPROJDOC; const pBurnerParam: PDvdBurnerParam): BOOL; stdcall;

//BOOL __stdcall PDGetDvdBurnerParam(HPROJDOC hProjDoc, DvdBurnParam* pBurnerParam);
function PDGetDvdBurnerParam(hPD: HPROJDOC; pBurnerParam: PDvdBurnerParam): BOOL; stdcall;

const
    DLLNAME = 'WS_ProjectDoc.dll';

implementation

procedure	PDInitClipInfo			; external DLLNAME Name 'PDInitClipInfo';
function	PDCreateProjDoc			; external DLLNAME Name 'PDCreateProjDoc';
procedure	PDDestroyProjDoc		; external DLLNAME Name 'PDDestroyProjDoc';
function	PDAddSlideshow			; external DLLNAME Name 'PDAddSlideshow';
function	PDAddClip				    ; external DLLNAME Name 'PDAddClip';
function	PDGetSlideshowCount	; external DLLNAME Name 'PDGetSlideshowCount';
function	PDGetSlideshowInfo	; external DLLNAME Name 'PDGetSlideshowInfo';
function	PDGetClipCount			; external DLLNAME Name 'PDGetClipCount';
function	PDGetClipInfo			  ; external DLLNAME Name 'PDGetClipInfo';
function	PDSaveToFile			  ; external DLLNAME Name 'PDSaveToFile';
function	PDLoadFromFile			; external DLLNAME Name 'PDLoadFromFile';
function	PDSetMenuXml			; external DLLNAME Name 'PDSetMenuXml';
function	PDGetMenuXml			; external DLLNAME Name 'PDGetMenuXml';
function	PDSetProjSettingXml		; external DLLNAME Name 'PDSetProjSettingXml';
function	PDGetProjSettingXml		; external DLLNAME Name 'PDGetProjSettingXml';
function	PDSetDvdBurnerParam		; external DLLNAME Name 'PDSetDvdBurnerParam';
function	PDGetDvdBurnerParam		; external DLLNAME Name 'PDGetDvdBurnerParam';
end.