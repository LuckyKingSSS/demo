unit
	MD5;
    //
    
interface

uses
    Windows;


//BOOL __stdcall GetDataMD5(const BYTE* pBuf, UINT nLength, char szBuf[64]);
function GetDataMD5(const pBuf: PBYTE; nLength : UINT; szBuf : PChar): BOOL; stdcall;

//BOOL __stdcall GetFileMD5(const char* pFileName, char szBuf[64]);
function GetFileMD5(const pFileName: PChar; szBuf : PChar): BOOL; stdcall;

//const char* __stdcall GetFileMD5Ex(const char* pFileName);
function GetFileMD5Ex(const pFileName: PChar): PChar; stdcall;

// 启动校验和检测
// 返回成功表示已经启动检测
// 请确保pFileName指针一直有效
//BOOL __stdcall StartCheckSum(const wchar_t* pFileName); 
function StartCheckSum(const pFileName: PWideChar): BOOL; stdcall;

//BOOL __stdcall StartCheckMD5(const wchar_t* pFileName, const char* pMD5); 
function StartCheckMD5(const pFileName: PWideChar; const pMD5: PChar): BOOL; stdcall;

implementation

const
    DLLNAME = 'WS_Log.dll';

function GetDataMD5					; external DLLNAME Name 'GetDataMD5';
function GetFileMD5					; external DLLNAME Name 'GetFileMD5';
function GetFileMD5Ex				; external DLLNAME Name 'GetFileMD5Ex';
function StartCheckSum				; external DLLNAME Index 110;
function StartCheckMD5				; external DLLNAME Index 111;

end.