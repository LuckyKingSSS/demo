{*******************************************************}
{       Wondershare DVD 信息 API单元  　                }
{       创建人:　 凌曙光　　 　　　　　　　   　　　　  }
{       创建日期: 2008/06/30                            }
{       修订人:   凌曙光                                }
{       修订日期:                                       }
{*******************************************************}

unit DVDInfo;

interface

uses Windows, SysUtils, MediaCommon, MediaDecode, WideStrings, WideStrUtils;

const
  CLSID_CCreateISOInfo: TGUID = '{A13F85D0-420C-4cf0-BD56-B6C1F101A99F}';
  IID_IIFOInfo: TGUID = '{5D71E358-66C1-4248-BB77-EB39F910E1FC}';
  IID_ISOInfo: TGUID = '{490F7DE8-FC4E-42af-9570-1433DB5FA847}';
  IID_ICreateISOInfo: TGUID = '{B66AB703-15E7-4e39-94E9-CEA64BBD5534}';

type

  IIFOInfo = interface(IUnknown)
    ['{5D71E358-66C1-4248-BB77-EB39F910E1FC}']
    function GetHaveCSSProtect(): Integer; stdcall;
    function GetTitleCount(): Integer; stdcall;
    function GetChapterCount(nTitleIndex: Integer): Integer; stdcall;
    function GetTitleBeginTime(nTitleIndex: Integer): Double; stdcall;
    function GetTitleLength(nTitleIndex: Integer): Double; stdcall;
    function GetChapterBeginTime(nTitleIndex: Integer; nChapterIndex: Integer): Double; stdcall;
    function GetChapterLength(nTitleIndex: Integer; nChapterIndex: Integer): Double; stdcall;
    function GetTitleAudioTrackCount(nTitleInex: Integer): Integer; stdcall;
    function GetAudioTrackDesc(nTitleIndex: Integer; nAudioTrackidx: Integer; var ppDescription: UnicodeString): HRESULT; stdcall;
    function GetTitleSubTitleCount(nTitleIndex: Integer): Integer; stdcall;
    function GetSubTitleDesc(nTitleIndex: Integer; nSubTitleidx: Integer; var ppDescription: UnicodeString): HRESULT; stdcall;
    function GetAudioTrackLang(nTitleIndex: Integer; nAudioTrackidx: Integer): Word; stdcall;
    function GetAudioFormatDesc(nTitleIndex: Integer; nAudioTrackidex: Integer; var ppDescription: UnicodeString): HRESULT; stdcall;
    function GetAudioTrackID(nTitleIndex: Integer; nAudioTrackidx: Integer): Integer; stdcall;
    function GetAudioChannel(nTitleIndex: Integer; nAudioTrackidx: Integer): Integer; stdcall;
    function GetSubLangCode(nTitleIndex: Integer; nSubTitleidx: Integer): Word; stdcall;
    function GetSubTitleTrackID(nTitleIndex: Integer; nSubTitleidx: Integer): Integer; stdcall;
    function GetTitleAvailable(nTitleIndex: Integer): Integer; stdcall;
  end;

  IISOInfo = interface(IUnknown)
    ['{490F7DE8-FC4E-42af-9570-1433DB5FA847}']
    function GetIFOCount(): Integer; stdcall;
    function GetISOVolumeName(var ppVolumeName: UnicodeString): HRESULT; stdcall;
  end;

  ICreateISOInfo = interface(IUnknown)
    ['{B66AB703-15E7-4e39-94E9-CEA64BBD5534}']
    function CreateISOInfo(pISOInfo: UnicodeString): Pointer; stdcall;
  end;
  
  HIFOINFO = IIFOInfo;

  TDVDIFOInfo = class
  private
    FIFOInfo: HIFOINFO;
    FIFOMediaInfo: IMediaInfo;
  public
    constructor Create(const AFileName: UnicodeString);
    destructor Destroy; override;

    class function GetISOIFOCount(const pPathFile: UnicodeString): Integer;
    class function GetISOVolumeName(const pPath: UnicodeString; out pVolumeName: UnicodeString): BOOL;

    function TitleAvailable(nTitleIndex: Integer): Boolean;
    function GetFileInfo(): IMediaInfo;
    function GetHaveCSSProtect(): Integer;

    function GetTitleProgramId(AMediaInfo: IMediaInfo; nTitleIndex: Integer): UINT;
    function GetTitleAudioTrackList(nTitleIndex: Integer; Strs: TWideStrings): Integer;
    function GetSubTitleList(nTitleIndex: Integer; Strs: TWideStrings): Integer;

    function GetTitleCount(): Integer;
    function GetChapterCount(nTitleIndex: Integer): Integer;
    function GetTitleBeginTime(nTitleIndex: Integer): Double;
    function GetTitleLength(nTitleIndex: Integer): Double;
    function GetChapterBeginTime(nTitleIndex: Integer; nChapterIndex: Integer): Double;
    function GetChapterLength(nTitleIndex: Integer; nChapterIndex: Integer): Double;
    function GetTitleAudioTrackCount(nTitleIndex: Integer): Integer;
    function GetAudioTrackDesc(nTitleIndex: Integer; nAudioTrackidx: Integer): PWideChar;
    function GetTitleSubTitleCount(nTitleIndex: Integer): Integer;
    function GetSubTitleDesc(nTitleIndex: Integer; nSubTitleidx: Integer): PWideChar;
    function GetAudioTrackLang(nTitleIndex: Integer; nAudioTrackidx: Integer): WORD;
    function GetAudioFormatDesc(nTitleIndex: Integer; nAudioTrackidx: Integer): PWideChar;
    function GetAudioTrackID(nTitleIndex: Integer; nAudioTrackidx: Integer): Integer;
    function GetAudioChannel(nTitleIndex: Integer; nAudioTrackidx: Integer): Integer;
    function GetSubLangCode(nTitleIndex: Integer; nSubTitleidx: Integer): WORD;
    function GetSubTitleTrackID(nTitleIndex: Integer; nSubTitleidx: Integer): Integer;

    property Handle: HIFOINFO read FIFOInfo;
  end;

implementation

{ TDVDIFOInfo }
var
  gCreateISOInfo: ICreateISOInfo = nil;
  
constructor TDVDIFOInfo.Create(const AFileName: UnicodeString);
begin
  inherited Create;
  if not Assigned(gDecMgr) then
  begin
    WSCoCreateInstance(CLSID_CDecMgr, nil, 0, IID_IDecMgr, gDecMgr);
    gDecMgr.GetMediaInfo(AFileName, MEDIA_FILE, AUTO_MODEL, FIFOMediaInfo);
    if Assigned(FIFOMediaInfo) then
    begin
      FIFOMediaInfo.QueryInterface(IID_IIFOInfo, FIFOInfo);
    end;  
  end;  

end;

destructor TDVDIFOInfo.Destroy;
begin
  if Assigned(FIFOInfo) then
  begin
    FIFOInfo._Release;
  end;
  if Assigned(FIFOMediaInfo) then
    FIFOMediaInfo._Release;
  inherited;
end;

function TDVDIFOInfo.GetAudioChannel(nTitleIndex, nAudioTrackidx: Integer): Integer;
begin
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetAudioChannel(nTitleIndex, nAudioTrackidx);
  end;
end;

function TDVDIFOInfo.GetAudioFormatDesc(nTitleIndex, nAudioTrackidx: Integer): PWideChar;
var
  pAudioFormatDesc: UnicodeString;
begin
  if Assigned(FIFOInfo) then
  begin
    FIFOInfo.GetAudioFormatDesc(nTitleIndex, nAudioTrackidx, pAudioFormatDesc);
    Result := PWideChar(pAudioFormatDesc);
  end;  
end;

function TDVDIFOInfo.GetAudioTrackDesc(nTitleIndex, nAudioTrackidx: Integer): PWideChar;
var
  pAudioTrackDesc: UnicodeString;
begin
  if Assigned(FIFOInfo) then
  begin
    FIFOInfo.GetAudioTrackDesc(nTitleIndex, nAudioTrackidx, pAudioTrackDesc);
    Result := PWideChar(pAudioTrackDesc);
  end;  
end;

function TDVDIFOInfo.GetAudioTrackID(nTitleIndex, nAudioTrackidx: Integer): Integer;
begin
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetAudioTrackID(nTitleIndex, nAudioTrackidx);
  end;  
end;

function TDVDIFOInfo.GetAudioTrackLang(nTitleIndex, nAudioTrackidx: Integer): WORD;
begin
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetAudioTrackLang(nTitleIndex, nAudioTrackidx);
  end;
end;

function TDVDIFOInfo.GetChapterBeginTime(nTitleIndex, nChapterIndex: Integer): Double;
begin
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetChapterBeginTime(nTitleIndex, nChapterIndex);
  end;  
end;

function TDVDIFOInfo.GetChapterCount(nTitleIndex: Integer): Integer;
begin
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetChapterCount(nTitleIndex);
  end;  
end;

function TDVDIFOInfo.GetChapterLength(nTitleIndex, nChapterIndex: Integer): Double;
begin
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetChapterLength(nTitleIndex, nChapterIndex);
  end;
end;

function TDVDIFOInfo.GetSubLangCode(nTitleIndex, nSubTitleidx: Integer): WORD;
begin
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetSubLangCode(nTitleIndex, nSubTitleidx);
  end;  
end;

function TDVDIFOInfo.GetSubTitleDesc(nTitleIndex, nSubTitleidx: Integer): PWideChar;
var
  pSubTitleDesc: UnicodeString;
begin
  if Assigned(FIFOInfo) then
  begin
    FIFOInfo.GetSubTitleDesc(nTitleIndex, nSubTitleidx, pSubTitleDesc);
    Result := PWideChar(pSubTitleDesc);
  end;  
end;

function TDVDIFOInfo.GetSubTitleList(nTitleIndex: Integer;
  Strs: TWideStrings): Integer;
var
  i: Integer;
begin
  Result:=0;
  Strs.Clear;
  for I := 0 to GetTitleSubTitleCount(nTitleIndex) - 1 do
  begin
    Strs.AddObject(UnicodeString(GetSubTitleDesc(nTitleIndex, I)), TObject(GetSubTitleTrackID(nTitleIndex, I)));
    Inc(Result);
  end;
end;

function TDVDIFOInfo.GetSubTitleTrackID(nTitleIndex, nSubTitleidx: Integer): Integer;
begin
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetSubTitleTrackID(nTitleIndex, nSubTitleidx);
  end;  
end;

function TDVDIFOInfo.GetTitleAudioTrackCount(nTitleIndex: Integer): Integer;
begin
  if Assigned(FIFOInfo) then
  begin
    FIFOInfo.GetTitleAudioTrackCount(nTitleIndex);
  end;
end;

function TDVDIFOInfo.GetTitleAudioTrackList(nTitleIndex: Integer;
  Strs: TWideStrings): Integer;
var
  i: Integer;
begin
  Result:=0;
  Strs.Clear;
  for I := 0 to GetTitleAudioTrackCount(nTitleIndex) - 1 do
  begin
    Strs.AddObject(UnicodeString(GetAudioTrackDesc(nTitleIndex, I)), TObject(GetAudioTrackID(nTitleIndex, I)));
    Inc(Result);
  end;
end;

function TDVDIFOInfo.GetTitleBeginTime(nTitleIndex: Integer): Double;
begin
  Result := 0;
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetTitleBeginTime(nTitleIndex);
  end;  
end;

function TDVDIFOInfo.GetTitleCount: Integer;
begin
  Result := 0;
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetTitleCount;
  end;  
end;

function TDVDIFOInfo.GetTitleLength(nTitleIndex: Integer): Double;
begin
  Result := 0;
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetTitleLength(nTitleIndex);
  end;  
end;

function TDVDIFOInfo.GetTitleProgramId(AMediaInfo: IMediaInfo;
  nTitleIndex: Integer): UINT;
var
  pProgramInfos: IProgramInfos;
  pProgramInfo: IProgramInfo;
begin
  Result:=0;
  pProgramInfos := nil;
  pProgramInfo := nil;
  if Assigned(AMediaInfo) then
  begin
    AMediaInfo.GetIPrograms(pProgramInfos);
    if Assigned(pProgramInfos) then
    begin
      if nTitleIndex < pProgramInfos.GetCount then
      begin
        pProgramInfos.GetItem(nTitleIndex, pProgramInfo);
        if Assigned(pProgramInfo) then
        begin
          Result := pProgramInfo.GetID;
          pProgramInfo._Release;
        end;  
      end;  
      pProgramInfos._Release;
    end;  
  end;
end;

function TDVDIFOInfo.GetTitleSubTitleCount(nTitleIndex: Integer): Integer;
begin
  Result := 0;
  if Assigned(FIFOInfo) then
    Result := FIFOInfo.GetTitleSubTitleCount(nTitleIndex);
end;

class function TDVDIFOInfo.GetISOVolumeName(const pPath: UnicodeString;
  out pVolumeName: UnicodeString): BOOL;
var
  pISOInfo: IISOInfo;
begin
  if not Assigned(gCreateISOInfo) then
    WSCoCreateInstance(CLSID_CCreateISOInfo, nil, 0, IID_ICreateISOInfo, gCreateISOInfo);
  pISOInfo := nil;
  if Assigned(gCreateISOInfo) then
  begin
    pISOInfo := IISOInfo(gCreateISOInfo.CreateISOInfo(pPath));
    if Assigned(pISOInfo) then
    begin
      pISOInfo.GetISOVolumeName(pVolumeName);
      pISOInfo._Release;
    end;
  end;  
end;

function TDVDIFOInfo.GetFileInfo: IMediaInfo;
begin
  Result := nil;
  if Assigned(FIFOMediaInfo) then
  begin
    FIFOMediaInfo._AddRef;
    Result := FIFOMediaInfo;
  end;  
end;

function TDVDIFOInfo.GetHaveCSSProtect: Integer;
begin
  Result := 0;
  if Assigned(FIFOInfo) then
  begin
    Result := FIFOInfo.GetHaveCSSProtect;
  end;  
end;

class function TDVDIFOInfo.GetISOIFOCount(const pPathFile: UnicodeString): Integer;
var
  pISOInfo: IISOInfo;
begin
  pISOInfo := nil;
  Result := 0;
  if not Assigned(gCreateISOInfo) then
    WSCoCreateInstance(CLSID_CCreateISOInfo, nil, 0, IID_ICreateISOInfo, gCreateISOInfo);
  if Assigned(gCreateISOInfo) then
  begin
    pISOInfo := IISOInfo(gCreateISOInfo.CreateISOInfo(pPathFile));
    if Assigned(pISOInfo) then
    begin
      Result := pISOInfo.GetIFOCount;
      pISOInfo._Release;
    end;
  end;  
end;

function TDVDIFOInfo.TitleAvailable(nTitleIndex: Integer): Boolean;
begin
  Result := False;
  if Assigned(FIFOInfo) then
  begin
    Result := Boolean(FIFOInfo.GetTitleAvailable(nTitleIndex));
  end;  
end;

end.
