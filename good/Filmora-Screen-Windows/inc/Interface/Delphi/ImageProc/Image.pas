unit
	Image;
    
interface

{$MINENUMSIZE 4}

uses
    Windows;

type
	HDIBIMAGE = Pointer;	// 设备无关位图句柄定义

// void __stdcall ImageInit();
procedure ImageInit(); stdcall;

// 关闭GDI+
// void __stdcall ImageUninit();
procedure ImageUninit(); stdcall;

//
//HDIBIMAGE __stdcall DICreate();	// 创建设备无关位图句柄
function DICreate() : HDIBIMAGE; stdcall;

//void __stdcall DIDestroy(HDIBIMAGE hImage);	// 创建设备无关位图句柄
procedure DIDestroy(hImage : HDIBIMAGE); stdcall;

//
//BOOL __stdcall DILoadImage(HDIBIMAGE hImage, const wchar_t* pFileName); // 加载图像文件
function DILoadImage(hImage : HDIBIMAGE; const pFileName : PWideChar) : BOOL; stdcall;

//BOOL __stdcall DILoadImageFromResource(HDIBIMAGE hImage, UINT nID, HMODULE hResModule = 0); // 从资源中加载Bitmap位图数据
function DILoadImageFromResource(hImage : HDIBIMAGE; nID : UINT; hResModule : HMODULE = 0) : BOOL; stdcall;

//BOOL __stdcall DILoadImage32B(HDIBIMAGE hImage, const wchar_t* pFileName); // 加载图像文件并转换为32位数据
function DILoadImage32B(hImage : HDIBIMAGE; const pFileName : PWideChar) : BOOL; stdcall;

//BOOL __stdcall DISave(HDIBIMAGE hImage, const wchar_t* pFileName); // 存储为Bitmap文件
function DISave(hImage : HDIBIMAGE; const pFileName : PWideChar) : BOOL; stdcall;

//BOOL __stdcall DISaveEx(HDIBIMAGE hImage, const wchar_t* pFileName, const wchar_t* pClsName = L"image/bmp"); // 存储为图像文件（支持多种格式）
function DISaveEx(hImage : HDIBIMAGE; const pFileName : PWideChar; const pClsName : PWideChar) : BOOL; stdcall;
// pClsName 取以下值：
// L"image/bmp"
// L"image/emf"
// L"image/wmf"
// L"image/jpeg"
// L"image/png"
// L"image/gif"
// L"image/tiff"
// L"image/exif"
// L"image/icon"


//BOOL __stdcall DISetContent(HDIBIMAGE hImage, int width, int height, int nBitCount = 32); // 重新分配内存空间
function DISetContent(hImage : HDIBIMAGE; width : Integer; height : Integer; nBitCount : Integer) : BOOL; stdcall;

//LPBITMAPINFO __stdcall DIGetBitmapInfo(HDIBIMAGE hImage); // 取得位图信息头指针
function DIGetBitmapInfo(hImage : HDIBIMAGE) : PBITMAPINFO; stdcall;

//LPBYTE __stdcall DIGetBits(HDIBIMAGE hImage); // 取得图像数据区内存指针
function DIGetBits(hImage : HDIBIMAGE) : PBYTE; stdcall;

//int __stdcall DIGetPitch(HDIBIMAGE hImage); // 取得图像每行所占字节数
function DIGetPitch(hImage : HDIBIMAGE) : Integer; stdcall;

//int __stdcall DIGetBitCount(HDIBIMAGE hImage); // 取得图像位深度
function DIGetBitCount(hImage : HDIBIMAGE) : Integer; stdcall;

//int __stdcall DIGetWidth(HDIBIMAGE hImage); // 取得图像宽度
function DIGetWidth(hImage : HDIBIMAGE) : Integer; stdcall;

//int __stdcall DIGetHeight(HDIBIMAGE hImage); // 取得图像高度
function DIGetHeight(hImage : HDIBIMAGE) : Integer; stdcall;


// BOOL __stdcall DIConvertFormat(HDIBIMAGE hImage, int nBitCount); // 转换像素格式 暂不支持
function DIConvertFormat(hImage : HDIBIMAGE; nBitCount : Integer) : BOOL; stdcall;

//BOOL __stdcall DICopyImage(HDIBIMAGE hImage, HDIBIMAGE hImageSrc); // 复制图像数据
function DICopyImage(hImage : HDIBIMAGE; hImageSrc : HDIBIMAGE) : BOOL; stdcall;

//BOOL __stdcall DIBltDraw(HDIBIMAGE hImage, HDC hDC, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0); // 快速绘制图像 不支持缩放
function DIBltDraw(hImage : HDIBIMAGE; hDC : HDC; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0) : BOOL; stdcall;


///////////////////////////////////////////////////////////////////////////////////////////
// 图像处理函数

// Alpha运算 （图像透明叠加）
//BOOL __stdcall DIAlphaBlend(HDIBIMAGE hBackImage, HDIBIMAGE hOverImage, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0, int nOpacity = 100);
function DIAlphaBlend(hBackImage : HDIBIMAGE; hOverImage : HDIBIMAGE; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0; nOpacity : Integer = 100) : BOOL; stdcall;

// Alpha运算 （图像透明叠加）叠加后会更改hBackImage各像素的透明度（Alpha值） 支持透明图像的合并
//BOOL __stdcall DIAlphaBlendEx(HDIBIMAGE hBackImage, HDIBIMAGE hOverImage, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0, int nOpacity = 100, int nOpacityBk = 100);
function DIAlphaBlendEx(hBackImage : HDIBIMAGE; hOverImage : HDIBIMAGE; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0; nOpacity : Integer = 100; nOpacityBk : Integer = 100) : BOOL; stdcall;


// 图像缩放处理

// 图像插值算法
type
IMAGE_INTERPOLATION =
(
    IIN_FAST,			// Nearest value (邻近插值)
    IIN_BI_LINER,		// Bi-Liner Interpolation (双线性插值)
    IIN_TRI_CONVOLUTE	// Tri-Convolute Interpolation (卷积插值)
);


// 只支持32位图像缩放 将hImgSrc缩放到hImgDst上
//BOOL __stdcall DIImageResize(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, IMAGE_INTERPOLATION iin = IIN_BI_LINER);
function DIImageResize(hImgDst : HDIBIMAGE; hImgSrc : HDIBIMAGE; iin : IMAGE_INTERPOLATION = IIN_BI_LINER) : BOOL; stdcall;

//BOOL __stdcall DIImageResizeEx(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, int xDst, int yDst, int cxDst, int cyDst, int xSrc, int ySrc, int cxSrc, int cySrc, IMAGE_INTERPOLATION iin = IIN_BI_LINER); 
function DIImageResizeEx(hImgDst : HDIBIMAGE; hImgSrc : HDIBIMAGE; xDst : Integer; yDst : Integer; cxDst : Integer; cyDst : Integer; xSrc : Integer; ySrc : Integer; cxSrc : Integer; cySrc : Integer; iin : IMAGE_INTERPOLATION = IIN_BI_LINER) : BOOL; stdcall;


// 图像旋转 将根据旋转角度重新为hImgDst分配内存空间
// 只支持32位图像
//BOOL __stdcall DIRotate(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, int iRotateAngle);
function DIRotate(hImgDst : HDIBIMAGE; hImgSrc : HDIBIMAGE; iRotateAngle : Integer) : BOOL; stdcall;

//BOOL __stdcall DIRotateSmooth(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, int iRotateAngle);
function DIRotateSmooth(hImgDst : HDIBIMAGE; hImgSrc : HDIBIMAGE; iRotateAngle : Integer) : BOOL; stdcall;

//BOOL __stdcall DIFillColor32(HDIBIMAGE hImage, ARGB argbColor); // 填充颜色 只适用于32位位图
function DIFillColor32(hImage : HDIBIMAGE; argbColor : DWORD) : BOOL; stdcall;


// 位数据传送
//BOOL __stdcall DIBltImage(HDIBIMAGE hImage, HDIBIMAGE hImageSrc, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0);
function DIBltImage(hImage : HDIBIMAGE; hImageSrc : HDIBIMAGE; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0) : BOOL; stdcall;

// 传送Mask数据（Alpha数据） 只适用于32位位图
//BOOL __stdcall DIBltMask(HDIBIMAGE hImage, HDIBIMAGE hImageMask, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0);
function DIBltMask(hBackImage : HDIBIMAGE; hImageMask : HDIBIMAGE; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0) : BOOL; stdcall;

// 传送Mask数据（Alpha数据）并将全透明部分清0 只适用于32位位图
//BOOL __stdcall DIBltMaskEx(HDIBIMAGE hImage, HDIBIMAGE hImageMask, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0);
function DIBltMaskEx(hBackImage : HDIBIMAGE; hImageMask : HDIBIMAGE; x : Integer; y : Integer; cx : Integer = -1; cy : Integer = -1; xSrc : Integer = 0; ySrc : Integer = 0) : BOOL; stdcall;


//HDIBIMAGE __stdcall DICreateThumbnail(HDIBIMAGE hImage, int width, int height); // 为hImage创建缩略图 返回的缩略图必须使用DIDestroy销毁 目前只适用于32位位图
function DICreateThumbnail(hImage : HDIBIMAGE; width : Integer; height : Integer) : HDIBIMAGE; stdcall;

//HBITMAP __stdcall DICreateHBitmap(HDIBIMAGE hImage); // 使用hImage转换为HBITMAP供一般Windows应用 返回的句柄必须使用DeleteObject销毁
function DICreateHBitmap(hImage : HDIBIMAGE) : HBITMAP; stdcall;


implementation

const
    DLLNAME = 'WS_ImageProc.dll';

procedure ImageInit          ;external DLLNAME name 'ImageInit';
procedure ImageUninit        ;external DLLNAME name 'ImageUninit';

function DICreate						; external DLLNAME Name 'DICreate';
procedure DIDestroy						; external DLLNAME Name 'DIDestroy';

function DILoadImage					; external DLLNAME Name 'DILoadImage';
function DILoadImageFromResource		; external DLLNAME Name 'DILoadImageFromResource';
function DILoadImage32B					; external DLLNAME Name 'DILoadImage32B';
function DISave							; external DLLNAME Name 'DISave';
function DISaveEx						; external DLLNAME Name 'DISaveEx';

function DISetContent					; external DLLNAME Name 'DISetContent';
function DIGetBitmapInfo				; external DLLNAME Name 'DIGetBitmapInfo';
function DIGetBits						; external DLLNAME Name 'DIGetBits';
function DIGetPitch						; external DLLNAME Name 'DIGetPitch';
function DIGetBitCount					; external DLLNAME Name 'DIGetBitCount';
function DIGetWidth						; external DLLNAME Name 'DIGetWidth';
function DIGetHeight					; external DLLNAME Name 'DIGetHeight';
function DIConvertFormat				; external DLLNAME Name 'DIConvertFormat';
function DICopyImage					; external DLLNAME Name 'DICopyImage';
function DIBltDraw						; external DLLNAME Name 'DIBltDraw';
function DIAlphaBlend					; external DLLNAME Name 'DIAlphaBlend';
function DIAlphaBlendEx					; external DLLNAME Name 'DIAlphaBlendEx';
function DIImageResize					; external DLLNAME Name 'DIImageResize';
function DIImageResizeEx				; external DLLNAME Name 'DIImageResizeEx';
function DIRotate						; external DLLNAME Name 'DIRotate';
function DIRotateSmooth					; external DLLNAME Name 'DIRotateSmooth';
function DIFillColor32					; external DLLNAME Name 'DIFillColor32';
function DIBltImage						; external DLLNAME Name 'DIBltImage';
function DIBltMask						; external DLLNAME Name 'DIBltMask';
function DIBltMaskEx					; external DLLNAME Name 'DIBltMaskEx';
function DICreateThumbnail				; external DLLNAME Name 'DICreateThumbnail';
function DICreateHBitmap				; external DLLNAME Name 'DICreateHBitmap';


end.

