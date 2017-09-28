unit
	DibImage;
    
interface

{$MINENUMSIZE 4}

uses
    Windows;

type
	HDIBIMAGE = Pointer;	// 设备无关位图句柄定义

  TDibImage = class
     constructor Create();
     destructor Destroy();

     function Width() : Integer;
     function Height() : Integer;
     function Pitch() : Integer;
     function BitCount() : Integer;
     function ImageSize() : Integer;
     function GetBitmapInfo() : PBITMAPINFO;
     function GetBits() : PBYTE;
     function Load(const lpszPathName : PWideChar) : BOOL;
     function Save(const lpszPathName : PWideChar) : BOOL;

	   function HeaderSize(lpbi : PBITMAPINFO = nil) : Integer;

     function SetContent(cx, cy, nBitCount : Integer) : BOOL;
     procedure Clear();

     function Blt(hDC: HDC;  x, y: Integer;  cx: Integer = -1;  cy : Integer = -1;  xSrc : Integer = 0;  ySrc :Integer = 0): BOOL;

  private
	  m_nPitch : Integer;

		m_lpBits : PBYTE;
		m_lpBmpInfo : PBITMAPINFO;

  end;

//var

implementation


{ TDibImage }

constructor TDibImage.Create;
begin

end;

destructor TDibImage.Destroy;
begin
  Clear;
end;

function TDibImage.BitCount: Integer;
begin
		if nil <> m_lpBmpInfo then
			Result := m_lpBmpInfo.bmiHeader.biBitCount;
end;

function TDibImage.GetBitmapInfo: PBITMAPINFO;
begin
  Result := m_lpBmpInfo;
end;

function TDibImage.GetBits: PBYTE;
begin
  Result := m_lpBits;
end;

function TDibImage.Width: Integer;
begin
		if nil <> m_lpBmpInfo then
			Result := m_lpBmpInfo.bmiHeader.biWidth;
end;

function TDibImage.Height: Integer;
begin
		if nil <> m_lpBmpInfo then
			Result := m_lpBmpInfo.bmiHeader.biHeight;
end;

function TDibImage.ImageSize: Integer;
begin
		if nil <> m_lpBmpInfo then
			Result := m_lpBmpInfo.bmiHeader.biSizeImage;
end;

function TDibImage.HeaderSize(lpbi: PBITMAPINFO): Integer;
var
  nSize : Integer;
  nColor : Integer;
begin
	nSize := 0;
	if lpbi = nil then
		lpbi := m_lpBmpInfo;

	if lpbi <> nil then
  begin

		nSize := sizeof(BITMAPINFOHEADER);
		if(lpbi.bmiHeader.biBitCount <= 8) then
		begin
			nColor := lpbi.bmiHeader.biClrUsed;
			if (nColor = 0) then
				nColor := 1 shl lpbi.bmiHeader.biBitCount;
			nSize := nSize + sizeof(RGBQUAD) * nColor;
		end;
	end;

  Result := nSize;
  
end;

//#define DIBPITCH(cx, bit)  (((bit * cx + 31) & ~31) >> 3)

function DIBPITCH(cx, bit : Integer) : Integer;
begin
  Result := ((bit * cx + 31) and (not 31)) shr 3;
end;

function TDibImage.Save(const lpszPathName: PWideChar): BOOL;
var
  bRet : Boolean;
  hFile : THandle;
  fd : BITMAPFILEHEADER;
  dwWrited : DWORD;
begin

	bRet := FALSE;
  ZeroMemory(@fd, SizeOf(fd));
  dwWrited := 0;

	if m_lpBits <> nil then
	begin
		hFile := CreateFileW(	lpszPathName,
									GENERIC_WRITE,
									FILE_SHARE_READ,
									nil,
									CREATE_ALWAYS,
									FILE_ATTRIBUTE_NORMAL,
									0);

	  if(hFile <> INVALID_HANDLE_VALUE) then
		begin
			fd.bfType := $4d42;
      fd.bfSize := sizeof(BITMAPFILEHEADER) + HeaderSize() + ImageSize();
      fd.bfReserved1 := 0;
      fd.bfReserved2 := 0;
      fd.bfOffBits := sizeof(BITMAPFILEHEADER) + HeaderSize();


			dwWrited := 0;
			WriteFile(hFile, fd, sizeof(BITMAPFILEHEADER), dwWrited, nil);
			WriteFile(hFile, m_lpBmpInfo^, HeaderSize(), dwWrited, nil);
			WriteFile(hFile, GetBits()^, ImageSize(), dwWrited, nil);

			CloseHandle(hFile);

			bRet := TRUE;
		end;
	end;

	Result := bRet;

end;

function TDibImage.Load(const lpszPathName: PWideChar): BOOL;
var
  bRet : BOOL;
  hFile : THandle;
  fd : BITMAPFILEHEADER;
  id : BITMAPINFOHEADER;
  dwRead : DWORD;
  nColor : Integer;
begin
	bRet := FALSE;
  ZeroMemory(@fd, SizeOf(fd));
  ZeroMemory(@id, SizeOf(id));
  dwRead := 0;
  nColor := 0;
	hFile := CreateFileW(	lpszPathName,
								GENERIC_READ,
								FILE_SHARE_READ,
								nil,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL or FILE_FLAG_SEQUENTIAL_SCAN,
								0);

	if(hFile <> INVALID_HANDLE_VALUE) then
  begin
		dwRead := 0;

		ReadFile(hFile, fd, sizeof(BITMAPFILEHEADER), dwRead, nil);
		if(fd.bfType = $4d42) then
    begin

			ReadFile(hFile, id, sizeof(BITMAPINFOHEADER), dwRead, nil);
			if(id.biSizeImage = 0) then
				id.biSizeImage := DIBPITCH(id.biWidth, id.biBitCount) * id.biHeight;
			if(SetContent(id.biWidth, id.biHeight, id.biBitCount)) then
      begin
 				CopyMemory(Pointer(m_lpBmpInfo), Pointer(@id), sizeof(BITMAPINFOHEADER));
				if(id.biBitCount <= 8) then
				begin
					nColor := id.biClrUsed;
					if(nColor = 0) then
						nColor := 1 shl id.biBitCount;
					ReadFile(hFile, m_lpBmpInfo.bmiColors, sizeof(RGBQUAD) * nColor, dwRead, nil);
				end;

				ReadFile(hFile, GetBits()^, ImageSize(), dwRead, nil);

				bRet := TRUE;
			end;
		end;

		CloseHandle(hFile);
	end;

  Result := bRet;

end;

function TDibImage.Pitch: Integer;
begin
	if 0 <> m_nPitch then
  begin
    if nil <> m_lpBmpInfo then
      m_nPitch := DIBPITCH(m_lpBmpInfo.bmiHeader.biWidth, m_lpBmpInfo.bmiHeader.biBitCount);
  end;

  Result := m_nPitch;
end;

procedure TDibImage.Clear;
begin

		FreeMem(Pointer(m_lpBits));
		FreeMem(Pointer(m_lpBmpInfo));

    m_nPitch := 0;
    m_lpBits := 0;
    m_lpBmpInfo := 0;

end;

function TDibImage.SetContent(cx, cy, nBitCount: Integer): BOOL;
var
  nHeaderSize : Integer;
  dwMemSize : Integer;
begin
  Result := TRUE;
	if (cx = Width()) and (cy = Height()) and (nBitCount = BitCount()) then
		Exit;

	Clear();

	nHeaderSize := sizeof(BITMAPINFOHEADER);
	if(nBitCount <= 8) then
    nHeaderSize := nHeaderSize + (1 shl nBitCount) * sizeof(RGBQUAD);

	m_nPitch := DIBPITCH(cx, nBitCount);
	dwMemSize := m_nPitch * cy;


  GetMem(m_lpBmpInfo, nHeaderSize);  // Dispose(m_lpBmpInfo);
  ZeroMemory(m_lpBmpInfo, nHeaderSize);
	m_lpBmpInfo.bmiHeader.biSize		:= sizeof(BITMAPINFOHEADER);
	m_lpBmpInfo.bmiHeader.biWidth		:= cx;
	m_lpBmpInfo.bmiHeader.biHeight		:= cy;
	m_lpBmpInfo.bmiHeader.biPlanes		:= 1;
	m_lpBmpInfo.bmiHeader.biBitCount	:= nBitCount;
	m_lpBmpInfo.bmiHeader.biSizeImage	:= dwMemSize;

  GetMem(m_lpBits, dwMemSize + 1024);
  ZeroMemory(m_lpBits, dwMemSize + 1024);

end;

function TDibImage.Blt(hDC: HDC; x, y, cx, cy, xSrc, ySrc: Integer): BOOL;
var
  iRet: Integer;
  nHeight: Integer;
  BitsInfo: TBitmapInfo;
  lpbi: PBITMAPINFO;
  pBits: PBYTE;
begin
  lpbi := m_lpBmpInfo;
  pBits := m_lpBits;

 	if (lpbi = nil) then
  begin
		result := FALSE;
    Exit;
  end;

	if (pBits = nil) then
    pBits := pbyte(pchar(lpbi) + 40);

  iRet := 0;
	if(pBits <> nil) then
	begin
		if(cx < 0)  then cx := lpbi.bmiHeader.biWidth;
		if(cy < 0)  then  cy := lpbi.bmiHeader.biHeight;
{
		 hOldPal := 0;
		 hPalette1 := 0;
		if(lpbi.bmiHeader.biBitCount <= 8)  then
		begin
			hPalette1 := MakePalette(lpbi);
			hOldPal := SelectPalette(hDC, hPalette1, TRUE);
			RealizePalette(hDC);
		end;
 }

		nHeight := lpbi.bmiHeader.biHeight;
		iRet := SetDIBitsToDevice(hDC,
			x, y, cx, cy, xSrc, nHeight - ySrc - cy,
			0, nHeight, pBits,
			lpbi^, DIB_RGB_COLORS);
{
		if (hOldPal <> 0) then
      SelectPalette(hDC, hOldPal, FALSE);
		if (hPalette1 <> 0) then
      DeleteObject(hPalette1);
      }
	end;
	result :=  iRet <> 0;

end;

end.


