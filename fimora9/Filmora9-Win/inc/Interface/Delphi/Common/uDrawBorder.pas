unit uDrawBorder;

interface

uses
  Windows;

function MakePalette(lpbi: PBITMAPINFO): HPALETTE;
function Blt(lpbi: PBITMAPINFO;  pBits: PBYTE; hDC: HDC;  x, y: Integer;  cx: Integer = -1;  cy : Integer = -1;  xSrc : Integer = 0;  ySrc :Integer = 0): Bool;
function DrawBorderToWnd (hDC: HDC;  pDstRect: PRECT; pBorderSize: PRECT;	 lpbi: PBITMAPINFO;  pBits :PBYTE = nil; pSrcRect: PRECT = nil): Bool;
function DrawBorderToWndFromResource (hDC: HDC;  pDstRect: PRECT; pBorderSize: PRECT;	 uBmpID: UINT;  hResModule :HMODULE = 0; pSrcRect: PRECT = nil): Bool;

type
  DibData = record
     lpbi: PBITMAPINFO;
     lpBits : PBYTE;
  end;
  DibDataList = record
    LeftTop     : DibData;
    Top         : DibData;
    RightTop    : DibData;
    Left        : DibData;
    Right       : DibData;
    LeftBottom  : DibData;
    Bottom      : DibData;
    RightBottom : DibData;
  end;
  PDibDataList = ^DibDataList;

function DrawImageListToWnd (hDC: HDC;  pDstRect: PRECT; pDibList: PDibDataList): Bool;

function DrawImageListToWndEx(hDC: HDC;  pDstRect: PRECT; pDibList: PDibDataList): Bool;


implementation

function  MakePalette(lpbi: PBITMAPINFO): HPALETTE;
const
  nRGBSize = 3;
var
  hPalette1: HPALETTE;
  nColors :Integer;
  pLogPal: PLOGPALETTE;
  i: Integer;
  pCardinal :Cardinal;
begin
 	hPalette1 := 0;
	if (lpbi.bmiHeader.biBitCount <= 8)  then
  begin
		nColors := 1 shl lpbi.bmiHeader.biBitCount;
    // pLogPal
    pCardinal :=  GlobalAlloc(
			GPTR, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColors);
    pLogPal := PLOGPALETTE(pCardinal);
		pLogPal.palVersion		:= $300;
		pLogPal.palNumEntries	:= nColors;

    for i := 0 to nColors-1 do
			CopyMemory(@pLogPal.palPalEntry[i], @lpbi.bmiColors[i], nRGBSize);

		hPalette1 := CreatePalette(pLogPal^);
		GlobalFree(pCardinal);
	end;
	result := hPalette1;
end;

function  Blt(lpbi: PBITMAPINFO;  pBits: PBYTE; hDC: HDC;  x, y: Integer;  cx: Integer = -1;  cy : Integer = -1;  xSrc : Integer = 0;  ySrc :Integer = 0): Bool;
var
  hOldPal: HPALETTE;
  hPalette1: HPALETTE;
  iRet: Integer;
  nHeight: Integer;
  BitsInfo: TBitmapInfo;
begin
 	if (lpbi = nil) then
  begin
		result := FALSE;
    Exit;
  end;

	if (pBits = nil) then
	//	pBits := PBYTE(@lpbi.bmiColors);
    pBits := pbyte(pchar(lpbi) + 40);

	 iRet := 0;
	if(pBits <> nil) then
	begin
		if(cx < 0)  then cx := lpbi.bmiHeader.biWidth;
		if(cy < 0)  then  cy := lpbi.bmiHeader.biHeight;

		 hOldPal := 0;
		 hPalette1 := 0;
		if(lpbi.bmiHeader.biBitCount <= 8)  then
		begin
			hPalette1 := MakePalette(lpbi);
			hOldPal := SelectPalette(hDC, hPalette1, TRUE);
			RealizePalette(hDC);
		end;

    BitsInfo := lpbi^;
		nHeight := lpbi.bmiHeader.biHeight;
		iRet := SetDIBitsToDevice(hDC,
			x, y, cx, cy, xSrc, nHeight - ySrc - cy,
			0, nHeight, pBits,
			BitsInfo, DIB_RGB_COLORS);

		if (hOldPal <> 0) then
      SelectPalette(hDC, hOldPal, FALSE);
		if (hPalette1 <> 0) then
      DeleteObject(hPalette1);
	end;
	result :=  iRet <> 0;
end;


// 画位图边框
// pDstRect - 目标矩形
// pBorderSize - 边框大小 right的值表示右边框尺寸 bottom的值表示下边框尺寸
// lpbi - 位图数据信息头指针
// pBits - 位图数据数据块指针 为0表示位图数据和信息头为一整体
// pSrcRect - 位图数据源矩形位置，为0表示使用最边上部分
// uBmpID - 位图资源ID
// hResModule - 位图资源所在的模块句柄
function DrawBorderToWnd (hDC: HDC;  pDstRect: PRECT; pBorderSize: PRECT;	 lpbi: PBITMAPINFO;  pBits :PBYTE; pSrcRect: PRECT): Bool;
var
  dstRect, srcRect: TRECT;
	 srcWIDTH: Integer;
	 srcHEIGHT: Integer;
   rc: TRECT;
   x, y, width, height: Integer;
	 destX: Integer;
	 destY: Integer;
	 srcX: Integer;
	 srcY: Integer;
	 wd: Integer;
	 hi: Integer;
	 count: Integer;
   i:Integer;
begin
  dstRect := pDstRect^;

	if (pSrcRect <> nil) then
		srcRect := pSrcRect^;

	if (pSrcRect = nil) then
	begin
		srcWIDTH  := lpbi.bmiHeader.biWidth;
		srcHEIGHT := lpbi.bmiHeader.biHeight;
    rc.Left := 0;
    rc.Top := 0;
    rc.Right := srcWIDTH;
    rc.Bottom := srcHEIGHT;
		srcRect := rc;
	end
	else
	begin
		srcWIDTH  := srcRect.Right - srcRect.Left;
		srcHEIGHT := srcRect.Bottom - srcRect.Top;
	end;

	// draw the top border
	x      := dstRect.left;
	y      := dstRect.top;
	width  := dstRect.Right - dstRect.Left;
	height := pBorderSize.top;

	begin
		destX := x;
		destY := y;
		srcX  := srcRect.left;
		srcY  := srcRect.top;
		wd := srcWIDTH - (pBorderSize.left + pBorderSize.right);
    count := Trunc((width - (pBorderSize.left + pBorderSize.right)) / wd );

		Blt(lpbi, pBits, hDC, destX, destY,
			pBorderSize.left, height, srcX, srcY);

		destX := destX + pBorderSize.left;
		srcX  := srcX + pBorderSize.left;
    for i := 0 to count-1 do
		begin
			Blt(lpbi, pBits, hDC, destX, destY, wd, height, srcX, srcY);
			destX := destX + wd;
		end;

		wd      := dstRect.right - destX;
		srcX    := srcRect.right - wd;
		Blt(lpbi, pBits, hDC, destX, destY, wd, height, srcX, srcY);

	end;

 	// draw the left border
	x        := dstRect.left;
	y        := dstRect.top;
	width    := pBorderSize.left;
	height   := dstRect.Bottom - dstRect.Top;

	begin
		y := y + pBorderSize.top;
		destX := x;
		destY := y;
		srcX  := srcRect.left;
		srcY  := srcRect.top + pBorderSize.top;
		wd := width;
		hi := srcHEIGHT - (pBorderSize.top + pBorderSize.bottom);
		count := Trunc((height - (pBorderSize.top + pBorderSize.bottom))  / hi);

    for i := 0 to count - 1 do
		begin
			Blt(lpbi, pBits, hDC, destX, destY, wd, hi, srcX, srcY);
			destY := destY + hi;
		end;

		hi      := dstRect.bottom - destY;
		srcY    := srcRect.bottom - hi;
		Blt(lpbi, pBits, hDC, destX, destY, width, hi, srcX, srcY);

	end;

 	// draw the right border
	x        := dstRect.right - pBorderSize.right;
	y        := dstRect.top;
	width    := pBorderSize.right;
	height   := dstRect.Bottom - dstRect.Top;

	begin
		y := y + pBorderSize.top;
		destX := x;
		destY := y;
		srcX  := srcRect.right - pBorderSize.right;
		srcY  := srcRect.top + pBorderSize.top;
		wd := width;
		hi := srcHEIGHT - (pBorderSize.top + pBorderSize.bottom);
		count := Trunc((height - (pBorderSize.top + pBorderSize.bottom))  / hi);

    for i := 0 to count - 1 do
		begin
			Blt(lpbi, pBits, hDC, destX, destY, wd, hi, srcX, srcY);
			destY := destY+ hi;
		end;

		hi      := dstRect.bottom - destY;
		srcY    := srcRect.bottom - hi;
		Blt(lpbi, pBits, hDC, destX, destY, width, hi, srcX, srcY);
	end;

	// draw the bottom border
	x        := dstRect.left;
	y        := dstRect.bottom - pBorderSize.bottom;
	width    := dstRect.Right - dstRect.Left;
	height   := pBorderSize.bottom;

	begin
		destX := x;
		destY := y;
		srcX  := srcRect.left;
		srcY  := srcRect.bottom - pBorderSize.bottom;
		wd := srcWIDTH - (pBorderSize.left + pBorderSize.right);
		hi := height;
		count := Trunc((width - (pBorderSize.left + pBorderSize.right))  / wd);

		Blt(lpbi, pBits, hDC, destX, destY,
			pBorderSize.left, height, srcX, srcY);

		destX := destX + pBorderSize.left;
		srcX  := srcX + pBorderSize.left;
    for i := 0 to count - 1 do
		begin
			Blt(lpbi, pBits, hDC, destX, destY, wd, hi, srcX, srcY);
			destX := destX + wd;
		end;

		wd      := dstRect.right - destX;
		srcX    := srcRect.right - wd;
		Blt(lpbi, pBits, hDC, destX, destY, wd, height, srcX, srcY);
	end;

	result := TRUE;
end;

function DrawBorderToWndFromResource (hDC: HDC;  pDstRect: PRECT; pBorderSize: PRECT;	 uBmpID: UINT;  hResModule :HMODULE; pSrcRect: PRECT): Bool;
var
  hSrc: HRSRC;
  hResData: HGLOBAL;
  lpResData: Pointer;
  lpbi : PBITMAPINFO;
begin
  hResData := 0;
	if(hResModule = 0) then     hResModule := GetModuleHandle(nil);

  hSrc := FindResource(hResModule, LPCTSTR(MAKEINTRESOURCE(uBmpID)), RT_BITMAP);
	if(hSrc <> 0) then
    hResData := LoadResource(hResModule, hSrc);
  if(hResData <> 0) then
  begin
    lpResData := LockResource(hResData);
    lpbi := PBITMAPINFO(lpResData);
    DrawBorderToWnd(hDC, pDstRect, pBorderSize, lpbi, nil, nil);
  end;
  Result := True;
end;


function DrawImageListToWnd (hDC: HDC;  pDstRect: PRECT; pDibList: PDibDataList): Bool;
var
   dstRect: TRECT;
   x, y, width, height: Integer;
	 destX: Integer;
	 destY: Integer;
	 wd: Integer;
	 ht: Integer;
	 count: Integer;
   i:Integer;
begin
  dstRect := pDstRect^;

	// draw the top border
	x      := dstRect.left;
	y      := dstRect.top;
	width  := dstRect.Right - dstRect.Left;
	height := dstRect.Bottom - dstRect.Top;

	begin
 		destX := x;
		destY := y;

    wd := pDibList.LeftTop.lpbi.bmiHeader.biWidth;
    ht := pDibList.LeftTop.lpbi.bmiHeader.biHeight;
		Blt(pDibList.LeftTop.lpbi, pDibList.LeftTop.lpBits, hDC, destX, destY,
			wd, ht, 0, 0);

    wd := pDibList.Top.lpbi.bmiHeader.biWidth;
    count := Trunc((width - (pDibList.Left.lpbi.bmiHeader.biWidth + pDibList.Right.lpbi.bmiHeader.biWidth)) / wd + 0.5);
		destX := destX + pDibList.LeftTop.lpbi.bmiHeader.biWidth;
    for i := 0 to count-1 do
		begin
			Blt(pDibList.Top.lpbi, pDibList.Top.lpBits, hDC, destX, destY, wd, ht, 0, 0);
			destX := destX + wd;
		end;

    wd := width - pDibList.RightTop.lpbi.bmiHeader.biWidth - destX;
    if wd > 0  then
			Blt(pDibList.Top.lpbi, pDibList.Top.lpBits, hDC, destX, destY, wd, ht, 0, 0);
    destX := destX + wd;

    wd := pDibList.RightTop.lpbi.bmiHeader.biWidth;
    ht := pDibList.RightTop.lpbi.bmiHeader.biHeight;
		Blt(pDibList.RightTop.lpbi, pDibList.RightTop.lpBits, hDC, destX + x, destY, wd, ht, 0, 0);

	end;

 	// draw the left border
	x        := dstRect.left;
	y        := dstRect.top;

    wd := pDibList.Left.lpbi.bmiHeader.biWidth;
    ht := pDibList.Left.lpbi.bmiHeader.biHeight;

	begin
		y := y + pDibList.LeftTop.lpbi.bmiHeader.biHeight;
		destX := x;
		destY := y;
		count := Trunc((height - (pDibList.Top.lpbi.bmiHeader.biHeight + pDibList.Bottom.lpbi.bmiHeader.biHeight))  / ht + 0.5);

    for i := 0 to count - 1 do
		begin
			Blt(pDibList.Left.lpbi, pDibList.Left.lpBits, hDC, destX, destY, wd, ht, 0, 0);
			destY := destY + ht;
		end;

    ht := height - pDibList.LeftBottom.lpbi.bmiHeader.biHeight - destY;
    if wd > 0  then
			Blt(pDibList.Left.lpbi, pDibList.Left.lpBits, hDC, destX, destY, wd, ht, 0, 0);
    destY := destY + ht;

		ht      := pDibList.LeftBottom.lpbi.bmiHeader.biHeight;
		Blt(pDibList.LeftBottom.lpbi, pDibList.LeftBottom.lpBits, hDC, destX, destY + dstRect.top, wd, ht, 0, 0);

	end;

 	// draw the right border
    wd := pDibList.Right.lpbi.bmiHeader.biWidth;
    ht := pDibList.Right.lpbi.bmiHeader.biHeight;
	x        := dstRect.right - wd;
	y        := dstRect.top;

	begin
		y := y + pDibList.RightTop.lpbi.bmiHeader.biHeight;
		destX := x;
		destY := y;
		count := Trunc((height - (pDibList.RightTop.lpbi.bmiHeader.biHeight + pDibList.RightBottom.lpbi.bmiHeader.biHeight))  / ht + 0.5);

    for i := 0 to count - 1 do
		begin
			Blt(pDibList.Right.lpbi, pDibList.Right.lpBits, hDC, destX, destY, wd, ht, 0, 0);
			destY := destY+ ht;
		end;

    ht := height - pDibList.RightBottom.lpbi.bmiHeader.biHeight - destY;
    if wd > 0  then
			Blt(pDibList.Right.lpbi, pDibList.Right.lpBits, hDC, destX, destY, wd, ht, 0, 0);
    destY := destY + ht;

		ht      := pDibList.RightBottom.lpbi.bmiHeader.biHeight;
		Blt(pDibList.RightBottom.lpbi, pDibList.RightBottom.lpBits, hDC, destX, destY + dstRect.top, wd, ht, 0, 0);
	end;

	// draw the bottom border
	x        := dstRect.left;
	y        := dstRect.bottom - pDibList.Bottom.lpbi.bmiHeader.biHeight;
    wd := pDibList.Bottom.lpbi.bmiHeader.biWidth;
    ht := pDibList.Bottom.lpbi.bmiHeader.biHeight;

	begin
		destX := x;
		destY := y;
		count := Trunc((width - (pDibList.LeftBottom.lpbi.bmiHeader.biWidth + pDibList.RightBottom.lpbi.bmiHeader.biWidth))  / wd + 0.5);

//		Blt(pDibList.LeftBottom.lpbi, pDibList.RightBottom.lpBits, hDC, destX, destY,
//			pBorderSize.left, height, srcX, srcY);

		destX := destX + pDibList.LeftBottom.lpbi.bmiHeader.biWidth;
    for i := 0 to count - 1 do
		begin
			Blt(pDibList.Bottom.lpbi, pDibList.Bottom.lpBits, hDC, destX, destY, wd, ht, 0, 0);
			destX := destX + wd;
		end;

    wd := width - pDibList.RightBottom.lpbi.bmiHeader.biWidth - destX;
    if wd > 0  then
			Blt(pDibList.Bottom.lpbi, pDibList.Bottom.lpBits, hDC, destX, destY, wd, ht, 0, 0);
    destX := destX + wd;

 //		wd      := dstRect.right - destX;
//		srcX    := srcRect.right - wd;
//		Blt(lpbi, pBits, hDC, destX, destY, wd, height, srcX, srcY);
	end;

	result := TRUE;
end;

function DrawImageListToWndEx(hDC: HDC;  pDstRect: PRECT; pDibList: PDibDataList): Bool;
var
   dstRect: TRECT;
   x, y, width, height: Integer;
	 destX: Integer;
	 destY: Integer;
	 wd: Integer;
	 ht: Integer;
	 count: Integer;
   i:Integer;
begin
  dstRect := pDstRect^;

	// draw the top border
	x      := dstRect.left;
	y      := dstRect.top;
	width  := dstRect.Right - dstRect.Left;
	height := dstRect.Bottom - dstRect.Top;

	begin
 		destX := x;
		destY := y;

    wd := pDibList.Top.lpbi.bmiHeader.biWidth;
    ht := pDibList.Top.lpbi.bmiHeader.biHeight;

		Blt(pDibList.Top.lpbi, pDibList.Top.lpBits, hDC, destX, destY,
			wd, ht, 0, 0);

//		Blt(pDibList.LeftTop.lpbi, pDibList.LeftTop.lpBits, hDC, destX, destY,
//			wd, ht, 0, 0);
//
//    wd := pDibList.Top.lpbi.bmiHeader.biWidth;
//    count := Trunc((width - (pDibList.Left.lpbi.bmiHeader.biWidth + pDibList.Right.lpbi.bmiHeader.biWidth)) / wd + 1);
//		destX := destX + pDibList.LeftTop.lpbi.bmiHeader.biWidth;
//    for i := 0 to count-1 do
//		begin
//			Blt(pDibList.Top.lpbi, pDibList.Top.lpBits, hDC, destX, destY, wd, ht, 0, 0);
//			destX := destX + wd;
//		end;
//
//    wd := width - pDibList.RightTop.lpbi.bmiHeader.biWidth - destX;
//    if wd > 0  then
//			Blt(pDibList.Top.lpbi, pDibList.Top.lpBits, hDC, destX, destY, wd, ht, 0, 0);
//    destX := destX + wd;
//
//    wd := pDibList.RightTop.lpbi.bmiHeader.biWidth;
//    ht := pDibList.RightTop.lpbi.bmiHeader.biHeight;
//		Blt(pDibList.RightTop.lpbi, pDibList.RightTop.lpBits, hDC, destX + x, destY, wd, ht, 0, 0);

	end;

 	// draw the left border
	x        := dstRect.left;
	y        := dstRect.top;

    wd := pDibList.Left.lpbi.bmiHeader.biWidth;
    ht := pDibList.Left.lpbi.bmiHeader.biHeight;

	begin
		y := y + pDibList.Top.lpbi.bmiHeader.biHeight;
		destX := x;
		destY := y;
		count := Trunc((height - (pDibList.Top.lpbi.bmiHeader.biHeight + pDibList.Bottom.lpbi.bmiHeader.biHeight))  / ht + 1);

    for i := 0 to count - 1 do
		begin
			Blt(pDibList.Left.lpbi, pDibList.Left.lpBits, hDC, destX, destY, wd, ht, 0, 0);
			destY := destY + ht;
		end;

    ht := height - pDibList.Bottom.lpbi.bmiHeader.biHeight - destY;
    if wd > 0  then
			Blt(pDibList.Left.lpbi, pDibList.Left.lpBits, hDC, destX, destY, wd, ht, 0, 0);
    destY := destY + ht;

		ht      := pDibList.Bottom.lpbi.bmiHeader.biHeight;
		//Blt(pDibList.LeftBottom.lpbi, pDibList.LeftBottom.lpBits, hDC, destX, destY + dstRect.top, wd, ht, 0, 0);

	end;

 	// draw the right border
//    wd := pDibList.Right.lpbi.bmiHeader.biWidth;
//    ht := pDibList.Right.lpbi.bmiHeader.biHeight;
//	x        := dstRect.right - wd;
//	y        := dstRect.top;
//
//	begin
//		y := y + pDibList.RightTop.lpbi.bmiHeader.biHeight;
//		destX := x;
//		destY := y;
//		count := Trunc((height - (pDibList.RightTop.lpbi.bmiHeader.biHeight + pDibList.RightBottom.lpbi.bmiHeader.biHeight))  / ht + 1);
//
//    for i := 0 to count - 1 do
//		begin
//			Blt(pDibList.Right.lpbi, pDibList.Right.lpBits, hDC, destX, destY, wd, ht, 0, 0);
//			destY := destY+ ht;
//		end;
//
//    ht := height - pDibList.RightBottom.lpbi.bmiHeader.biHeight - destY;
//    if wd > 0  then
//			Blt(pDibList.Right.lpbi, pDibList.Right.lpBits, hDC, destX, destY, wd, ht, 0, 0);
//    destY := destY + ht;
//
//		ht      := pDibList.RightBottom.lpbi.bmiHeader.biHeight;
//		Blt(pDibList.RightBottom.lpbi, pDibList.RightBottom.lpBits, hDC, destX, destY + dstRect.top, wd, ht, 0, 0);
//	end;

	// draw the bottom border
	x        := dstRect.left;
	y        := dstRect.bottom - pDibList.Bottom.lpbi.bmiHeader.biHeight;
    wd := pDibList.Bottom.lpbi.bmiHeader.biWidth;
    ht := pDibList.Bottom.lpbi.bmiHeader.biHeight;

	begin
		destX := x;
		destY := y;
		//count := Trunc((width - (pDibList.LeftBottom.lpbi.bmiHeader.biWidth + pDibList.RightBottom.lpbi.bmiHeader.biWidth))  / wd + 1);

		Blt(pDibList.Bottom.lpbi, pDibList.Bottom.lpBits, hDC, destX, destY,
			wd, ht, 0, 0);

//		destX := destX + pDibList.LeftBottom.lpbi.bmiHeader.biWidth;
//    for i := 0 to count - 1 do
//		begin
//			Blt(pDibList.Bottom.lpbi, pDibList.Bottom.lpBits, hDC, destX, destY, wd, ht, 0, 0);
//			destX := destX + wd;
//		end;
//
//    wd := width - pDibList.RightBottom.lpbi.bmiHeader.biWidth - destX;
//    if wd > 0  then
//			Blt(pDibList.Bottom.lpbi, pDibList.Bottom.lpBits, hDC, destX, destY, wd, ht, 0, 0);
//    destX := destX + wd;

 //		wd      := dstRect.right - destX;
//		srcX    := srcRect.right - wd;
//		Blt(lpbi, pBits, hDC, destX, destY, wd, height, srcX, srcY);
	end;

	result := TRUE;
end;

end.
