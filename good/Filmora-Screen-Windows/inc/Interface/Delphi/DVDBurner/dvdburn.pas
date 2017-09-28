unit dvdburner;

interface

uses Windows, Messages;

const
  dvdburner = 'dvdburner.dll';
const
  WM_BURNER_UPDATE = WM_USER + 300;  // wParam 为BURN_PROGRESS等，lParam为下面定义的值

// wParam
  BURN_PROGRESS = 0;// lParam为进度百分比，0-100
  BURN_FINISH = 1;// 成功结束
  BURN_ERROR = 2;// 出错
  BURN_STATUS = 3;// 文本状态信息。lParam为wchar_t指针
  BURN_START = 4;   // 开始

  BURN_FIND_BEGIN = 5;   // 5  find begin
  BURN_FIND_ONE = 6;   // 6 find one  lParam为DISC_INFO结构
  BURN_FIND_END = 7;   // 7 find ends
  BURN_STATUS_FROMISODATA_START = 8;  //开始获取DISC的信息
  BURN_STATUS_FROMISODATA_STOP = 9;  //完成DISC的信息

// status
  BURN_STATUS_IDLE = 0;
  BURN_STATUS_FIND = 1; // find device
  BURN_STATUS_TOUDFISO = 2;// to iso
  BURN_STATUS_TOISO9006 = 3;// to iso
  BURN_STATUS_TODVD = 4;  // to dvd
  BURN_STATUS_BLANK = 5;  // to dvd
  BURN_STATUS_TOVIDEOISO = 6;  // to video iso
  BURN_STATUS_TOVIDEODVD = 7;  // to video dvd
  BURN_STATUS_FROMISODATA = 8;  // FROM ISO data

//type
//  DISC_INFO = record
//    string szVolumeName; // G:
//    string szProductId;  // DVD_ROM
//    BOOL bDvdReadSupported;
//    BOOL bDvdWriteSupported;
//    BOOL bCdReadSupported;
//    BOOL bCdWriteSupported;
//    int nIndex;
//  end;

  DVD_INFO = record
    bInsertDVD: Boolean;
    szDVDType: array [0..256] of Char;;
    bNewDVD: Boolean;
    
    bCanWrite: Boolean;
    bCanErase: Boolean;
    bEmpty: Boolean;
    nCurrentSize: int64;
  end;

type
  HQDBURNER = type pointer;


// 申请句柄
function AllocBurnCtl(nBurnerId: Integer): HQDBURNER; stdcall; external dvdburner;
procedure FreeBurnCtl(pBurner: HQDBURNER); stdcall; external dvdburner;

function __stdcall InitBurnCtl(pCtl: HQDBURNER); stdcall; 

// 查找DVD光驱
function qBeginFindDisc(pCtl: HQDBURNER; hNotifyWnd: HWnd): Integer; stdcall; external dvdburner;
function qGetDiscCount(pCtl: HQDBURNER): Integer; stdcall; external dvdburner;


// 刻录ISO文件
function qBurnIsoToDvd(pCtl: HQDBURNER; nDiscIndex: Integer; szIsoFile: PWideChar): Integer; stdcall; external dvdburner;

// 获取当前的状态，0为空闲
function qGetStatus(pCtl: HQDBURNER): Integer; stdcall; external dvdburner;

// 设置回调句柄
procedure qSetWndHandle(pBurner: HQDBURNER;  hNotifyWnd: HWnd); stdcall; external dvdburner;

// 设置卷标
procedure qSetVolumeName(pBurner: HQDBURNER;  szVolumeName: PWideChar); stdcall; external dvdburner;

function qBlankDisc(pCtl: HQDBURNER; nDiscIndex: Integer): Integer; stdcall;  //擦除DVD
function qGetDiscStatus(pCtl: HQDBURNER; nDriverIndex: Integer): Integer; stdcall; //获取某个光驱中光盘的信息。
implementation

end.
