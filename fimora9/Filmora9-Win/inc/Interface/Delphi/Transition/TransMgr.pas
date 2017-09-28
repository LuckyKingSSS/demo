unit TransMgr;

interface

uses
  Windows, TransParam;

//BOOL __stdcall TMInitTransMgr();
function TMInitTransMgr() : BOOL; stdcall;

//void __stdcall TMUninitTransMgr();
procedure TMUninitTransMgr(); stdcall;

//int	__stdcall TMGetTransTypeCount();
function TMGetTransTypeCount() : Integer; stdcall;

//const char* __stdcall TMGetTransTypeName(int nIndex);
function TMGetTransTypeName(nIndex : Integer) : PChar; stdcall;

//int	__stdcall TMGetTransCount(int nTypeIndex);
function TMGetTransCount(nTypeIndex : Integer) : Integer; stdcall;

//const char* __stdcall TMGetTransID(int nTypeIndex, int nIndex);
function TMGetTransID(nTypeIndex : Integer; nIndex : Integer) : PChar; stdcall;

//const char* __stdcall TMGetRandomID();
function TMGetRandomID(): PChar; stdcall;

//TRANSITION_API __stdcall TMGetTransProc(int nTypeIndex, int nIndex);
function TMGetTransProc(nTypeIndex : Integer; nIndex : Integer) : TRANSITION_API; stdcall;

//void __stdcall TMTransitionProc(YV12Image* imgOut, const YV12Image* imgFrom, const YV12Image* imgTo, const TransParam* trParam);
procedure TMTransitionProc(imgOut : PYV12Image; const imgFrom : PYV12Image; const imgTo : PYV12Image; const trParam : PTTransParam); stdcall;

implementation

const
    DLLNAME = 'TransMgr.dll';

function  TMInitTransMgr              ; external DLLNAME Name 'TMInitTransMgr';
procedure TMUninitTransMgr            ; external DLLNAME Name 'TMUninitTransMgr';
function  TMGetTransTypeCount         ; external DLLNAME Name 'TMGetTransTypeCount';
function  TMGetTransTypeName          ; external DLLNAME Name 'TMGetTransTypeName';
function  TMGetTransCount             ; external DLLNAME Name 'TMGetTransCount';
function  TMGetTransID                ; external DLLNAME Name 'TMGetTransID';
function  TMGetRandomID               ; external DLLNAME Name 'TMGetRandomID';
function  TMGetTransProc              ; external DLLNAME Name 'TMGetTransProc';
procedure TMTransitionProc            ; external DLLNAME Name 'TMTransitionProc';
                                     
end.                                 
                                     
                                     
                                     
                                     
                                     
                                     
                                     
