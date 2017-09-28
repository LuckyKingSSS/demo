unit
  DVDBurner;

interface

uses
  Windows, Messages, DVDBurnerParam;

//BOOL __stdcall BNInit();
//function BNInit(): BOOL; stdcall;

//void __stdcall BNUninit();
//procedure BNUninit(); stdcall;

//void __stdcall BNCheckNeroVersion();
procedure BNCheckNeroVersion(); stdcall;

//BOOL __stdcall BNNeroIsValid();
function BNNeroIsValid(): BOOL; stdcall;

//void __stdcall BNCheckBurnDriver();
procedure BNCheckBurnDriver(); stdcall;

//int __stdcall BNGetBurnDriverCount();
function BNGetBurnDriverCount(): Integer; stdcall;

//const wchar_t* __stdcall BNGetBurnDriverName(int nIndex);
function BNGetBurnDriverName(nIndex : Integer): PWideChar; stdcall;

//BOOL __stdcall BNGetDiskInfo(int nDriverIndex);
function BNGetDiskInfo(nDriverIndex : Integer) :BOOL; stdcall;

//BOOL __stdcall BNGetDiskInfoData(CDiskInfo* pDiskInfo);
function BNGetDiskInfoData(pDiskInfo : PCDiskInfo) :BOOL; stdcall;

//BOOL __stdcall BNPrepareBurn(int nDriverIndex);
function BNPrepareBurn(nDriverIndex : Integer) :BOOL; stdcall;

//BOOL __stdcall BNEraseDisk(int nDriverIndex);
function BNEraseDisk(nDriverIndex : Integer) :BOOL; stdcall;

//void __stdcall BNRegisterMsgHwnd(HWND hMsgWnd);
procedure BNRegisterMsgHwnd(hMsgWnd : HWND); stdcall;

//BOOL BNBurnIsoFile(const wchar_t* szISOName, const wchar_t* szDVDFolder, const wchar_t* szVolumeLabel);
function BNBurnIsoFile(const szISOName : PWideChar; const szDVDFolder : PWideChar; const szVolumeLabel : PWideChar) : BOOL; stdcall;

//BOOL __stdcall BNBurnDVD(BURNER_ID uBurnerID, int nDriverIndex, const wchar_t* szDVDFolder, const wchar_t* szVolumeLabel);
function BNBurnDVD(uBurnerID : BURNER_ID; nDriverIndex : Integer; const szDVDFolder : PWideChar; const szVolumeLabel : PWideChar): BOOL; stdcall;

//BOOL __stdcall BNBurnISO2DVD(BURNER_ID uBurnerID, int nDriverIndex, const wchar_t* szISOName);
function BNBurnISO2DVD(uBurnerID : BURNER_ID; nDriverIndex : Integer; const szISOName : PWideChar): BOOL; stdcall;

//void __stdcall BNExitBurn();
procedure BNExitBurn(); stdcall;

const
    DLLNAME = 'WS_DVDBurner.dll';

implementation

//function  BNInit                ; external DLLNAME Name 'BNInit';
//procedure BNUninit              ; external DLLNAME Name 'BNUninit';

procedure BNCheckNeroVersion    ; external DLLNAME Name 'BNCheckNeroVersion';
function  BNNeroIsValid         ; external DLLNAME Name 'BNNeroIsValid';

procedure BNCheckBurnDriver     ; external DLLNAME Name 'BNCheckBurnDriver';
function  BNGetBurnDriverCount  ; external DLLNAME Name 'BNGetBurnDriverCount';
function  BNGetBurnDriverName   ; external DLLNAME Name 'BNGetBurnDriverName';
function  BNGetDiskInfo         ; external DLLNAME Name 'BNGetDiskInfo';
function  BNGetDiskInfoData     ; external DLLNAME Name 'BNGetDiskInfoData';
function  BNPrepareBurn         ; external DLLNAME Name 'BNPrepareBurn';
function  BNEraseDisk           ; external DLLNAME Name 'BNEraseDisk';

procedure BNRegisterMsgHwnd     ; external DLLNAME Name 'BNRegisterMsgHwnd';
function  BNBurnIsoFile         ; external DLLNAME Name 'BNBurnIsoFile';
function  BNBurnDVD             ; external DLLNAME Name 'BNBurnDVD';
function  BNBurnISO2DVD         ; external DLLNAME Name 'BNBurnISO2DVD';
procedure BNExitBurn            ; external DLLNAME Name 'BNExitBurn';


end.