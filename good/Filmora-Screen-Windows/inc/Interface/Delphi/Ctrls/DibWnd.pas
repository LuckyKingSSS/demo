unit DibWnd;
{
   通用贴图窗体类：
   用法：将欲贴图的窗体使用DibWnd派生，在FormCreate函数
       中调用SetBorderImage函数设置贴图文件，目前只支
       持24位BMP、JPG、PNG图像
   创建：杨长元 2006-12-25
   修订：
}

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, DibImage, uDrawBorder, ShellApi;

type
  UIImageList = record
    LeftTop     : WideString;
    Top         : WideString;
    RightTop    : WideString;
    Left        : WideString;
    Right       : WideString;
    LeftBottom  : WideString;
    Bottom      : WideString;
    RightBottom : WideString;
  end;
  PUIImageList = ^UIImageList;
  
  DibImageList = record
    LeftTop     : TDibImage;
    Top         : TDibImage;
    RightTop    : TDibImage;
    Left        : TDibImage;
    Right       : TDibImage;
    LeftBottom  : TDibImage;
    Bottom      : TDibImage;
    RightBottom : TDibImage;
  end;

  TFrmDib = class(TForm)
    procedure FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure FormResize(Sender: TObject);
  private
    { Private declarations }
    UIList : UIImageList;
    DDList : DibDataList;
    DIList : DibImageList;
    procedure Clear();
//    procedure WMEraseBkgnd(var Message: TWMEraseBkgnd); message WM_ERASEBKGND;
    procedure CalcRgn();

  public
    { Public declarations }
    procedure SetBorderImage(pImageList : PUIImageList);
  end;
  
function CreateRgnFromDib(lpbi: PBITMAPINFO; pBits: PBYTE; nPitch: Integer): HRgn;
function DIBWIDTHBYTES(cx: DWORD; bit: DWORD):DWORD;

function ModifyStyle(hWnd: HWND; dwRemove: DWORD; dwAdd: DWORD; nFlags: UINT  = 0) : BOOL;
function ModifyStyleEx(hWnd: HWND; dwRemove: DWORD; dwAdd: DWORD; nFlags: UINT  = 0) : BOOL;

var
  frmDib: TFrmDib;

implementation

{$R *.dfm}

procedure TFrmDib.Clear;
begin

  if (DIList.Top <> nil) then DIList.Top.Destroy;
  if (DIList.LeftTop <> nil) then DIList.LeftTop.Destroy;
  if (DIList.RightTop <> nil) then DIList.RightTop.Destroy;
  if (DIList.Left <> nil) then DIList.Left.Destroy;
  if (DIList.Right <> nil) then DIList.Right.Destroy;
  if (DIList.LeftBottom <> nil) then DIList.LeftBottom.Destroy;
  if (DIList.Bottom <> nil) then DIList.Bottom.Destroy;
  if (DIList.RightBottom <> nil) then DIList.RightBottom.Destroy;

  ZeroMemory(@DDList, sizeof(DDList));

end;

procedure TFrmDib.FormCreate(Sender: TObject);
begin
// RGN
  DoubleBuffered := True;
  ModifyStyle(Handle, 0, WS_CLIPCHILDREN or WS_CLIPSIBLINGS, 0);
end;

procedure TFrmDib.FormDestroy(Sender: TObject);
begin
  Clear();
end;

procedure TFrmDib.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if key = VK_F1 then
  begin
  //  ShellExecute(Handle, 'open', PAnsiChar('c:'), nil, nil, SW_SHOWNORMAL);
  end
end;

procedure TFrmDib.FormMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
    ReleaseCapture;
    Perform(WM_SYSCOMMAND, $F012, 0);
end;

procedure FillSolidRect(hDC: HDC; pRect: PRECT; clr: COLORREF);
begin
	SetBkColor(hDC, clr);
	ExtTextOut(hDC, 0, 0, ETO_OPAQUE, pRect, nil, 0, 0);
end;

procedure TFrmDib.FormPaint(Sender: TObject);
var
  dstRect: TRect;
begin
  DstRect := Self.GetClientRect;
  DrawImageListToWnd(Canvas.Handle, @dstRect, @DDList);

  dstRect.Left := DDList.Left.lpbi.bmiHeader.biWidth;
  dstRect.Top := DDList.Top.lpbi.bmiHeader.biHeight;
  dstRect.Right := dstRect.Right - DDList.Right.lpbi.bmiHeader.biWidth;
  dstRect.Bottom := dstRect.Bottom - DDList.Bottom.lpbi.bmiHeader.biHeight;
  Canvas.FillRect(dstRect);

end;

procedure TFrmDib.FormResize(Sender: TObject);
begin
  CalcRgn();
  Invalidate();
end;

procedure TFrmDib.CalcRgn;
var
  DstRect: TRect;
  memdc: HDC;
  hbmp: HBITMAP;
  hOldbmp: HBITMAP;
  bmi: BITMAPINFO;
  pBits: PBYTE;
  Rgn: HRgn;
begin
  ZeroMemory(@bmi, sizeof(bmi));
  DstRect := Self.GetClientRect;
  bmi.bmiHeader.biSize := 40;
  bmi.bmiHeader.biPlanes := 1;
  bmi.bmiHeader.biBitCount := 24;
  bmi.bmiHeader.biWidth := DstRect.Right - DstRect.Left;
  bmi.bmiHeader.biHeight := DstRect.Bottom - DstRect.Top;
  bmi.bmiHeader.biSizeImage := DIBWIDTHBYTES(bmi.bmiHeader.biWidth, bmi.bmiHeader.biBitCount) * bmi.bmiHeader.biHeight;
  memdc := CreateCompatibleDC(0);
  hbmp := CreateDIBSection(memdc, bmi, DIB_RGB_COLORS, Pointer(pBits), 0, 0);
  hOldbmp := HBITMAP(SelectObject(memdc, hbmp));

  DrawImageListToWnd(memdc, @dstRect, @DDList);
  Rgn := CreateRgnFromDib(@bmi, pBits, DIBWIDTHBYTES(bmi.bmiHeader.biWidth, bmi.bmiHeader.biBitCount));

  SelectObject(memdc, hOldbmp);

  SetWindowRgn(self.Handle, Rgn, False);
  DeleteObject(Rgn);
  DeleteObject(hbmp);
  DeleteDC(memdc);

end;

procedure TFrmDib.SetBorderImage(pImageList: PUIImageList);
begin
  UIList := pImageList^;
  Clear();

  DIList.LeftTop             := TDibImage.Create();
  DIList.Top                 := TDibImage.Create();
  DIList.RightTop            := TDibImage.Create();
  DIList.Left                := TDibImage.Create();
  DIList.Right               := TDibImage.Create();
  DIList.LeftBottom          := TDibImage.Create();
  DIList.Bottom              := TDibImage.Create();
  DIList.RightBottom         := TDibImage.Create();

  DIList.LeftTop     .Load (PWideChar(UIList.LeftTop     ));
  DIList.Top         .Load (PWideChar(UIList.Top         ));
  DIList.RightTop    .Load (PWideChar(UIList.RightTop    ));
  DIList.Left        .Load (PWideChar(UIList.Left        ));
  DIList.Right       .Load (PWideChar(UIList.Right       ));
  DIList.LeftBottom  .Load (PWideChar(UIList.LeftBottom  ));
  DIList.Bottom      .Load (PWideChar(UIList.Bottom      ));
  DIList.RightBottom .Load (PWideChar(UIList.RightBottom ));

  DDList.LeftTop.lpbi        := DIList.LeftTop              .GetBitmapInfo();
  DDList.Top.lpbi            := DIList.Top                  .GetBitmapInfo();
  DDList.RightTop.lpbi       := DIList.RightTop             .GetBitmapInfo();
  DDList.Left.lpbi           := DIList.Left                 .GetBitmapInfo();
  DDList.Right.lpbi          := DIList.Right                .GetBitmapInfo();
  DDList.LeftBottom.lpbi     := DIList.LeftBottom           .GetBitmapInfo();
  DDList.Bottom.lpbi         := DIList.Bottom               .GetBitmapInfo();
  DDList.RightBottom.lpbi    := DIList.RightBottom          .GetBitmapInfo();

  DDList.LeftTop.lpBits      := DIList.LeftTop              .GetBits(  );
  DDList.Top.lpBits          := DIList.Top                  .GetBits(  );
  DDList.RightTop.lpBits     := DIList.RightTop             .GetBits(  );
  DDList.Left.lpBits         := DIList.Left                 .GetBits(  );
  DDList.Right.lpBits        := DIList.Right                .GetBits(  );
  DDList.LeftBottom.lpBits   := DIList.LeftBottom           .GetBits(  );
  DDList.Bottom.lpBits       := DIList.Bottom               .GetBits(  );
  DDList.RightBottom.lpBits  := DIList.RightBottom          .GetBits(  );

  CalcRgn();

  Invalidate();

end;

//procedure TFrmDib.WMEraseBkgnd(var Message: TWMEraseBkgnd);
//begin
//
//end;

function CreateRgnFromDib(lpbi: PBITMAPINFO; pBits: PBYTE; nPitch: Integer): HRgn;
var
  Rgn: HRgn;
  RgnTemp: HRgn;
  i, j: Integer;
  sp: PBYTE;
  width, height: Integer;
  pt : array[0..1] of TPoint;
  r, g, b: BYTE;
begin
  width := lpbi.bmiHeader.biWidth;
  height := lpbi.bmiHeader.biHeight;

  Rgn := CreateRectRgn(0, 0, 0, 0);

  sp := PBYTE(PCHAR(pBits) + (height - 1) * nPitch);
  for I := 0 to height - 1 do
  begin
    pt[0].X := -1;
    pt[0].Y := -1;
    pt[1].X := -1;
    pt[1].Y := -1;
    for j := 0 to width - 1 do
    begin
      r := PBYTE(PCHAR(sp) + j * 3 + 0)^;
      g := PBYTE(PCHAR(sp) + j * 3 + 1)^;
      b := PBYTE(PCHAR(sp) + j * 3 + 2)^;

      if -1 = pt[0].X then
      begin
        if (255 <> r) or (255 <> b) or (0 <> g) then
        begin
          pt[0].X := j;
          pt[0].Y := i;
        end;
      end
      else
      begin
        if (255 = r) AND (255 = b) AND (0 = g) then
        begin
          pt[1].X := j;
          pt[1].Y := i;
          break;
        end;
      end;
    end;

    if -1 <> pt[0].X then
    begin
      if -1 = pt[1].X then
      begin
          pt[1].X := width;
          pt[1].Y := i;
      end;
        RgnTemp := CreateRectRgn(pt[0].x, pt[0].y, pt[1].x, pt[1].y + 1);
        CombineRgn(Rgn, rgnTemp, Rgn, RGN_XOR);
        DeleteObject(RgnTemp);
    end;
    
    sp := PBYTE(PCHAR(sp) - nPitch);
  end;

  result := Rgn;
end;

function DIBWIDTHBYTES(cx: DWORD; bit: DWORD):DWORD;
begin
  Result := ((cx * bit + 31) and not 31) shr 3;
end;


function ModifyStyleImpl(hWnd: HWND; nStyleOffset: Integer; dwRemove: DWORD; dwAdd: DWORD; nFlags: UINT  = 0) : BOOL;
var
  dwStyle: DWORD;
  dwNewStyle: DWORD;
begin
    result := FALSE;
		dwStyle := GetWindowLong(hWnd, nStyleOffset);
		dwNewStyle := (dwStyle and not dwRemove) or dwAdd;
		if (dwStyle = dwNewStyle) then	exit;

		SetWindowLong(hWnd, nStyleOffset, dwNewStyle);
		if (nFlags <> 0) then
    begin
			SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE or SWP_NOMOVE or SWP_NOZORDER or SWP_NOACTIVATE or nFlags);
    end;
    result := TRUE;
end;

function ModifyStyle(hWnd: HWND; dwRemove: DWORD; dwAdd: DWORD; nFlags: UINT  = 0) : BOOL;
begin
		ModifyStyleImpl(hWnd, GWL_STYLE, dwRemove, dwAdd, nFlags);
end;

function ModifyStyleEx(hWnd: HWND; dwRemove: DWORD; dwAdd: DWORD; nFlags: UINT  = 0) : BOOL;
begin
		ModifyStyleImpl(hWnd, GWL_EXSTYLE, dwRemove, dwAdd, nFlags);
end;



end.
