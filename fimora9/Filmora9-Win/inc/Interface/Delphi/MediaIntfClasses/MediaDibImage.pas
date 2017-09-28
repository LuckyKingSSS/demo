unit MediaDibImage;

interface
uses
  Windows, Classes, MediaCommon;
const
  CLSID_CPCSText: TGUID = '{c3297339-6fc4-4b45-81cc-578b39bfac45}';
  IID_IPCText:    TGUID = '{E8E0B4CB-9E9A-46f6-AA7D-FA547A3EA1F4}';
const
  EFFECT_NAME_BUF_LEN = 64;
  TEXT_FONT_NAME_LEN  = 128;
  TEXT_BUF_MAX_LEN    = 1024;

type

  PARTTEXTINFO = ^TARTTEXTINFO;
  TARTTEXTINFO = record
    hInst: THandle;
    szArtID: array[0..64] of PWideChar;
    uBmpID: UINT;
    dwReserve: DWORD;
  end;
  
  PTEXTTRANSFORM = ^TTEXTTRANSFORM;
  TTEXTTRANSFORM = record
    ptLeftTop: TPoint;
    ptLeftBottom: TPoint;
    ptRightTop: TPoint;
    ptRightBottom: TPoint;
  end;
  // 文本数据结构定义
  
  PTRANSFORM_POINTS = ^TRANSFORM_POINTS;
  TRANSFORM_POINTS = record
    ptLeftTop: TPOINT;      // 左上角坐标
    ptRightTop: TPOINT;     // 右上角坐标
    ptRightBottom: TPOINT;  // 右下角坐标
    ptLeftBottom: TPOINT;   // 左下角坐标
  end;
  
  
  // 艺术字参数结构定义
  PARTTEXTPARAM = ^TARTTEXTPARAM;
  TARTTEXTPARAM = record
    strArtID: array[0..EFFECT_NAME_BUF_LEN - 1] of PWideChar;  // 艺术字类型ID
    nParam1: Integer;
    nParam2: Integer;
    nParam3: Integer;
    nParam4: Integer;
    nParam5: Integer;
    dParam6: Double;
    dParam7: Double; // 1-7号参数意义根据具体的艺术字类型定义
  end;


  IDibImage = interface(IUnknown)
    ['{2E1ADF80-6F97-47f5-A3AA-9EB80EE12D3F}']
    function GetWidth(): Integer; stdcall;
    function GetHeight(): Integer; stdcall;
    function GetBitCount(): Integer; stdcall;
    function GetImageSize(): Integer; stdcall;
    function GetPitch(): Integer; stdcall;
    function GetBitmapInfo(): PBITMAPINFO; stdcall;
    function GetBits(): PBYTE; stdcall;
    function GetHeaderSize(): Integer; stdcall;
  end;


  HDIBIMAGE = IDibImage;
  TDibImage = class
  private
    FHandle: HDIBIMAGE;
    FOwnedHandle: Boolean;
    procedure SetBitCount(Value : Integer);
    function GetBitCount() : Integer;
    function GetWidth() : Integer;
    function GetHeight() : Integer;
    function GetBitmapInfo() : PBITMAPINFO;
    function GetPitch() : Integer;
    function GetBits() : PBYTE;
    procedure SetHeight(Value: Integer);
    procedure SetWidth(Value: Integer);
    function GetBitmapInfoSize: Integer;
    function GetBitsSize: Integer;
    function GetEmpty: Boolean;
  public
    constructor Create(); overload;
    constructor Create(width : Integer; height : Integer; nBitCount : Integer); overload;
    constructor Create(hImage: HDIBIMAGE; AOwned: Boolean=True); overload;
    constructor Create(lpInfo: PBitmapInfoHeader; lpBits: PBYTE); overload;
    destructor Destroy; override;

    class function FromStream(AStream: TStream; ASize: Integer): TDibImage;
    class function FromFile(const AFileName: UnicodeString): TDibImage;
    class function FromFile32B(const AFileName: UnicodeString): TDibImage;
    class function FromResource(nID : UINT; hResModule : HMODULE = 0): TDibImage;

    function Clone: TDibImage;
    function GetHBitmap() : HBITMAP;
    function GetThumbnailImage(width : Integer; height : Integer) : HDIBIMAGE;

    function LoadFromStream(AStream: TStream; ASize: Integer): BOOL;
    function SaveToStream(AStream: TStream): BOOL;
    function LoadImage(pFileName : PWideChar) : BOOL; overload;
    function LoadImage(const AFileName : UnicodeString) : BOOL; overload;
    function LoadImageFromResource(nID : UINT; hResModule : HMODULE = 0) : BOOL;
    function LoadImage32B(pFileName : PWideChar) : BOOL; overload;
    function LoadImage32B(const AFileName : UnicodeString) : BOOL; overload;
    function Save(pFileName : PWideChar) : BOOL; overload;
    function Save(const AFileName : UnicodeString) : BOOL; overload;
    function SaveEx(const pFileName : PWideChar; const pClsName : PWideChar) : BOOL; overload;
    function SaveEx(const AFileName, AClsName : UnicodeString) : BOOL; overload;
    function SetContent(width : Integer; height : Integer; nBitCount : Integer) : BOOL;
    function ConvertFormat(nBitCount: Integer): BOOL;
    function BltDraw(hDC : HDC; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0) : BOOL;
    function StretchDraw(hDC : HDC; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0; cxSrc: Integer = 0; cySrc: Integer = 0) : BOOL;
    function AlphaBlend(hBackImage : HDIBIMAGE; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0; nOpacity : Integer = 100) : BOOL;
    function AlphaBlendEx(hBackImage : HDIBIMAGE; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0; nOpacity : Integer = 100; nOpacityBk : Integer = 100) : BOOL;
    function ImageResize(hImgDst : HDIBIMAGE; iin : IMAGE_INTERPOLATION = IIN_BI_LINER) : BOOL;
    function ImageResizeEx(hImgDst : HDIBIMAGE; xDst : Integer; yDst : Integer; cxDst : Integer; cyDst : Integer; xSrc : Integer; ySrc : Integer; cxSrc : Integer; cySrc : Integer; iin : IMAGE_INTERPOLATION = IIN_BI_LINER) : BOOL;
    function Rotate(hImgDst : HDIBIMAGE; iRotateAngle : Integer) : BOOL;
    function RotateSmooth(hImgDst : HDIBIMAGE; iRotateAngle : Integer) : BOOL;
    function FillColor32(argbColor : DWORD) : BOOL;
    function BltImage(hImgDest : HDIBIMAGE; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0) : BOOL;
    function BltMask(hImageMask : HDIBIMAGE; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0) : BOOL;
    function BltMaskEx(hImageMask : HDIBIMAGE; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0) : BOOL;
    function FillColor32Ex(argbColor: DWORD; x, y: Integer; cx: Integer = -1; cy: Integer = -1) : BOOL;
    function FillPatchColor32(cx, cy: Integer; argbColor, argbColor2: COLORREF) : BOOL;
    function FillPatchColor32Ex(cx, cy: Integer; argbColor, argbColor2: COLORREF; x_off, y_off: Integer) : BOOL;
    function ImageAlphaResize(hImgDst: HDIBIMAGE; xDst, yDst, cxDst, cyDst, xSrc, ySrc, cxSrc, cySrc: Integer; iin: IMAGE_INTERPOLATION = IIN_BI_LINER) : BOOL;
    function ImageAlphaResizeEx(hImgDst: HDIBIMAGE; xDst, yDst, cxDst, cyDst, xSrc, ySrc, cxSrc, cySrc: Integer;  iin: IMAGE_INTERPOLATION = IIN_BI_LINER) : BOOL;
    function ImageStretchMask(hImgDst: HDIBIMAGE; xDst, yDst, cxDst, cyDst, xSrc, ySrc, cxSrc, cySrc: Integer) : BOOL;
    function ImageStretchMaskEx(hImgDst: HDIBIMAGE; xDst, yDst, cxDst, cyDst, xSrc, ySrc, cxSrc, cySrc: Integer) : BOOL;
    function Transform(hImage: HDIBIMAGE; pTrPoints: PTRANSFORM_POINTS; xSrc: Integer = 0; ySrc: Integer = 0; cxSrc: Integer = -1; cySrc: Integer = -1; iin: IMAGE_INTERPOLATION = IIN_BI_LINER) : BOOL;
    function MaskImage(pRect: PRECT; dwArgbColor: COLORREF = $80000000) : BOOL;
    function MergeImage(hBackImage: HDIBIMAGE; x, y: Integer; cx: Integer = -1; cy: Integer = -1; xSrc: Integer = 0; ySrc: Integer = 0; nOpacity: Integer = 100; nOpacityBk: Integer = 100) : BOOL;

    property Handle: HDIBIMAGE read FHandle;
    property Width: Integer read GetWidth write SetWidth;
    property Height: Integer read GetHeight write SetHeight;
    property BitCount: Integer read GetBitCount write SetBitCount;
    property Pitch: Integer read GetPitch;
    property BitmapInfo: PBITMAPINFO read GetBitmapInfo;
    property Bits: PByte read GetBits;
    property BitmapInfoSize: Integer read GetBitmapInfoSize;
    property BitsSize: Integer read GetBitsSize;
    property Empty: Boolean read GetEmpty;
  end;

implementation

{ TDibImage }

function TDibImage.AlphaBlend(hBackImage: HDIBIMAGE; x, y, cx, cy, xSrc, ySrc,
  nOpacity: Integer): BOOL;
begin

end;

function TDibImage.AlphaBlendEx(hBackImage: HDIBIMAGE; x, y, cx, cy, xSrc, ySrc,
  nOpacity, nOpacityBk: Integer): BOOL;
begin

end;

function TDibImage.BltDraw(hDC: HDC; x, y, cx, cy, xSrc, ySrc: Integer): BOOL;
begin

end;

function TDibImage.BltImage(hImgDest: HDIBIMAGE; x, y, cx, cy, xSrc,
  ySrc: Integer): BOOL;
begin

end;

function TDibImage.BltMask(hImageMask: HDIBIMAGE; x, y, cx, cy, xSrc,
  ySrc: Integer): BOOL;
begin

end;

function TDibImage.BltMaskEx(hImageMask: HDIBIMAGE; x, y, cx, cy, xSrc,
  ySrc: Integer): BOOL;
begin

end;

function TDibImage.Clone: TDibImage;
begin

end;

function TDibImage.ConvertFormat(nBitCount: Integer): BOOL;
begin

end;

constructor TDibImage.Create;
begin
end;

constructor TDibImage.Create(lpInfo: PBitmapInfoHeader; lpBits: PBYTE);
begin

end;

constructor TDibImage.Create(hImage: HDIBIMAGE; AOwned: Boolean);
begin

end;

constructor TDibImage.Create(width, height, nBitCount: Integer);
begin

end;

destructor TDibImage.Destroy;
begin

  inherited;
end;

function TDibImage.FillColor32(argbColor: DWORD): BOOL;
begin

end;

function TDibImage.FillColor32Ex(argbColor: DWORD; x, y, cx, cy: Integer): BOOL;
begin

end;

function TDibImage.FillPatchColor32(cx, cy: Integer; argbColor,
  argbColor2: COLORREF): BOOL;
begin

end;

function TDibImage.FillPatchColor32Ex(cx, cy: Integer; argbColor,
  argbColor2: COLORREF; x_off, y_off: Integer): BOOL;
begin

end;

class function TDibImage.FromFile(const AFileName: UnicodeString): TDibImage;
begin

end;

class function TDibImage.FromFile32B(const AFileName: UnicodeString): TDibImage;
begin
  Result:=TDibImage.Create;
  Result.LoadImage32B(AFileName);
end;

class function TDibImage.FromResource(nID: UINT;
  hResModule: HMODULE): TDibImage;
begin

end;

class function TDibImage.FromStream(AStream: TStream;
  ASize: Integer): TDibImage;
begin

end;

function TDibImage.GetBitCount: Integer;
begin

end;

function TDibImage.GetBitmapInfo: PBITMAPINFO;
begin
  Result := nil;
  if Assigned(FHandle) then
    Result := FHandle.GetBitmapInfo();
end;

function TDibImage.GetBitmapInfoSize: Integer;
var
  lpbi: PBITMAPINFO;
  nColor: Integer;
begin
  Result := 0;
  lpbi := nil;
  if Assigned(FHandle) then
    lpbi := FHandle.GetBitmapInfo();
  if Assigned(lpbi) then
  begin
    Result := SizeOf(TBitmapInfoHeader);
    if lpbi.bmiHeader.biBitCount <= 8 then
    begin
      nColor := lpbi.bmiHeader.biClrUsed;
      if nColor = 0 then
        nColor := 1 shl lpbi.bmiHeader.biBitCount;
      Result := Result + SizeOf(RGBQUAD) * nColor;
    end;  
  end;
end;

function TDibImage.GetBits: PBYTE;
begin
  Result := nil;
  if Assigned(FHandle) then
    Result := FHandle.GetBits();
end;

function TDibImage.GetBitsSize: Integer;
begin

end;

function TDibImage.GetEmpty: Boolean;
begin

end;

function TDibImage.GetHBitmap: HBITMAP;
begin

end;

function TDibImage.GetHeight: Integer;
begin

end;

function TDibImage.GetPitch: Integer;
begin

end;

function TDibImage.GetThumbnailImage(width, height: Integer): HDIBIMAGE;
begin

end;

function TDibImage.GetWidth: Integer;
begin

end;

function TDibImage.ImageAlphaResize(hImgDst: HDIBIMAGE; xDst, yDst, cxDst,
  cyDst, xSrc, ySrc, cxSrc, cySrc: Integer; iin: IMAGE_INTERPOLATION): BOOL;
begin

end;

function TDibImage.ImageAlphaResizeEx(hImgDst: HDIBIMAGE; xDst, yDst, cxDst,
  cyDst, xSrc, ySrc, cxSrc, cySrc: Integer; iin: IMAGE_INTERPOLATION): BOOL;
begin

end;

function TDibImage.ImageResize(hImgDst: HDIBIMAGE;
  iin: IMAGE_INTERPOLATION): BOOL;
begin

end;

function TDibImage.ImageResizeEx(hImgDst: HDIBIMAGE; xDst, yDst, cxDst, cyDst,
  xSrc, ySrc, cxSrc, cySrc: Integer; iin: IMAGE_INTERPOLATION): BOOL;
begin

end;

function TDibImage.ImageStretchMask(hImgDst: HDIBIMAGE; xDst, yDst, cxDst,
  cyDst, xSrc, ySrc, cxSrc, cySrc: Integer): BOOL;
begin

end;

function TDibImage.ImageStretchMaskEx(hImgDst: HDIBIMAGE; xDst, yDst, cxDst,
  cyDst, xSrc, ySrc, cxSrc, cySrc: Integer): BOOL;
begin

end;

function TDibImage.LoadFromStream(AStream: TStream; ASize: Integer): BOOL;
begin

end;

function TDibImage.LoadImage(pFileName: PWideChar): BOOL;
begin

end;

function TDibImage.LoadImage(const AFileName: UnicodeString): BOOL;
begin

end;

function TDibImage.LoadImage32B(pFileName: PWideChar): BOOL;
begin

end;

function TDibImage.LoadImage32B(const AFileName: UnicodeString): BOOL;
begin
  
end;

function TDibImage.LoadImageFromResource(nID: UINT; hResModule: HMODULE): BOOL;
begin

end;

function TDibImage.MaskImage(pRect: PRECT; dwArgbColor: COLORREF): BOOL;
begin

end;

function TDibImage.MergeImage(hBackImage: HDIBIMAGE; x, y, cx, cy, xSrc, ySrc,
  nOpacity, nOpacityBk: Integer): BOOL;
begin

end;

function TDibImage.Rotate(hImgDst: HDIBIMAGE; iRotateAngle: Integer): BOOL;
begin

end;

function TDibImage.RotateSmooth(hImgDst: HDIBIMAGE;
  iRotateAngle: Integer): BOOL;
begin

end;

function TDibImage.Save(pFileName: PWideChar): BOOL;
begin

end;

function TDibImage.Save(const AFileName: UnicodeString): BOOL;
begin

end;

function TDibImage.SaveEx(const pFileName, pClsName: PWideChar): BOOL;
begin

end;

function TDibImage.SaveEx(const AFileName, AClsName: UnicodeString): BOOL;
begin

end;

function TDibImage.SaveToStream(AStream: TStream): BOOL;
begin

end;

procedure TDibImage.SetBitCount(Value: Integer);
begin

end;

function TDibImage.SetContent(width, height, nBitCount: Integer): BOOL;
begin

end;

procedure TDibImage.SetHeight(Value: Integer);
begin

end;

procedure TDibImage.SetWidth(Value: Integer);
begin

end;

function TDibImage.StretchDraw(hDC: HDC; x, y, cx, cy, xSrc, ySrc, cxSrc,
  cySrc: Integer): BOOL;
begin

end;

function TDibImage.Transform(hImage: HDIBIMAGE; pTrPoints: PTRANSFORM_POINTS;
  xSrc, ySrc, cxSrc, cySrc: Integer; iin: IMAGE_INTERPOLATION): BOOL;
begin

end;

end.
