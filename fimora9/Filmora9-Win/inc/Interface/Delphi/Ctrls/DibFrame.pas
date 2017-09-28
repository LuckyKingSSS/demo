unit DibFrame;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms, 
  Dialogs, DibWnd, uDrawBorder, DibImage;

type
  TTDibFrame = class(TFrame)
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
    procedure WMOnPaint(var Message: TWMPaint); message WM_PAINT;

  public
    { Public declarations }
    procedure SetBorderImage(pImageList : PUIImageList);
  end;

implementation

{$R *.dfm}

procedure TTDibFrame.Clear;
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

procedure TTDibFrame.FormDestroy(Sender: TObject);
begin
  Clear();
end;

procedure FillSolidRect(DC : HDC; lpRect : PRECT; clr : COLORREF );
begin
	Windows.SetBkColor(DC, clr);
	Windows.ExtTextOut(DC, 0, 0, ETO_OPAQUE, lpRect, nil, 0, nil);

end;

procedure TTDibFrame.FormPaint(Sender: TObject);
var
  dstRect: TRect;
  DC : HDC;
begin
  DstRect := Self.GetClientRect;
  DC := Windows.GetDC(Self.Handle);
  DrawImageListToWnd(DC, @dstRect, @DDList);

  dstRect.Left := DDList.Left.lpbi.bmiHeader.biWidth;
  dstRect.Top := DDList.Top.lpbi.bmiHeader.biHeight;
  dstRect.Right := dstRect.Right - DDList.Right.lpbi.bmiHeader.biWidth;
  dstRect.Bottom := dstRect.Bottom - DDList.Bottom.lpbi.bmiHeader.biHeight;
//  Canvas.FillRect(dstRect);

  dstRect.Right := 200;
  dstRect.Bottom := 200;
  FillSolidRect(DC, @dstRect, RGB(255, 0, 0));

  Windows.ReleaseDC(Self.Handle, DC);

end;

procedure TTDibFrame.FormResize(Sender: TObject);
begin
  Invalidate();
end;


procedure TTDibFrame.SetBorderImage(pImageList: PUIImageList);
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

  Invalidate();

end;

procedure TTDibFrame.WMOnPaint(var Message: TWMPaint);
var
  dstRect: TRect;
  DC: HDC;
  PS: TPaintStruct;
begin
  DstRect := Self.GetClientRect;

  DC := BeginPaint(Handle, PS);

  DrawImageListToWnd(DC, @dstRect, @DDList);

  dstRect.Left := DDList.Left.lpbi.bmiHeader.biWidth;
  dstRect.Top := DDList.Top.lpbi.bmiHeader.biHeight;
  dstRect.Right := dstRect.Right - DDList.Right.lpbi.bmiHeader.biWidth;
  dstRect.Bottom := dstRect.Bottom - DDList.Bottom.lpbi.bmiHeader.biHeight;
//  Canvas.FillRect(dstRect);

  FillSolidRect(DC, @dstRect, RGB(255, 255, 255));

  EndPaint(Handle, PS);

end;

end.
