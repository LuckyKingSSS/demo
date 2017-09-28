/////////////////////////////////////////////
// Image.h
// 提供设备无关位图数据接口


#pragma once


typedef void* HDIBIMAGE;

#ifndef WIN32
#include "Linux32_Typedef.h"

typedef HANDLE HBITMAP;
#endif

#ifdef WIN32
void __stdcall ImageInit();

// 关闭GDI+
void __stdcall ImageUninit();
#endif

extern "C" HDIBIMAGE __stdcall DICreate();	// 创建设备无关位图句柄
extern "C" void __stdcall DIDestroy(HDIBIMAGE hImage);	// 创建设备无关位图句柄
extern "C" HDIBIMAGE __stdcall DICreateIndirect(LPBYTE lpInfo, LPBYTE lpBits = 0); // 根据Bitmap内存数据创建Image

// mac下只能加载：bmp、jpg、gif、png、tif、ico文件
extern "C" int __stdcall DILoadImage(HDIBIMAGE hImage, const wchar_t* pFileName); // 加载图像文件
#ifdef WIN32
int __stdcall DILoadImageFromResource(HDIBIMAGE hImage, UINT nID, HMODULE hResModule = 0); // 从资源中加载Bitmap位图数据
#endif
extern "C" int __stdcall DILoadImage32B(HDIBIMAGE hImage, const wchar_t* pFileName); // 加载图像文件并转换为32位数据
extern "C" int __stdcall DISave(HDIBIMAGE hImage, const wchar_t* pFileName); // 存储为Bitmap文件
extern "C" int __stdcall DISaveEx(HDIBIMAGE hImage, const wchar_t* pFileName, const wchar_t* pClsName = L"image/bmp"); // 存储为图像文件（支持多种格式）
// windows下pClsName 取以下值：
// L"image/bmp"
// L"image/emf"
// L"image/wmf"
// L"image/jpeg"
// L"image/png"
// L"image/gif"
// L"image/tiff"
// L"image/exif"
// L"image/icon"
// mac下pClsName 取以下值：
// L"image/bmp"
// L"image/jpeg"
// L"image/png"
// L"image/gif"
// L"image/tiff"
// L"image/pdf"

extern "C" int __stdcall DISetContent(HDIBIMAGE hImage, int width, int height, int nBitCount = 32); // 重新分配内存空间
extern "C" int __stdcall DISetContentEx(HDIBIMAGE hImage, int width, int height, void* addr,int nBitCount = 32); //确定内存空间 

extern "C" LPBITMAPINFO __stdcall DIGetBitmapInfo(HDIBIMAGE hImage); // 取得位图信息头指针
extern "C" LPBYTE __stdcall DIGetBits(HDIBIMAGE hImage); // 取得图像数据区内存指针
extern "C" int __stdcall DIGetPitch(HDIBIMAGE hImage); // 取得图像每行所占字节数
extern "C" int __stdcall DIGetBitCount(HDIBIMAGE hImage); // 取得图像位深度
extern "C" int __stdcall DIGetWidth(HDIBIMAGE hImage); // 取得图像宽度
extern "C" int __stdcall DIGetHeight(HDIBIMAGE hImage); // 取得图像高度
extern "C" int __stdcall DIGetBitmapInfoSize(HDIBIMAGE hImage); // 取得图像信息尺寸
extern "C" int __stdcall DIGetDataSize(HDIBIMAGE hImage); // 取得图像数据尺寸

extern "C" int __stdcall DISetBitmapInfo(HDIBIMAGE hImage, int newWidht, int newHeight, BYTE* pBase = NULL);

// 图像像素格式转换 目前只支持转为32位
int __stdcall DIConvertFormat(HDIBIMAGE hImage, int nBitCount); 
int __stdcall DICopyImage(HDIBIMAGE hImage, HDIBIMAGE hImageSrc); // 复制图像数据

#ifdef WIN32
int __stdcall DIBltDraw(HDIBIMAGE hImage, HDC hDC, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0); // 快速绘制图像 不支持缩放
#else
#endif

///////////////////////////////////////////////////////////////////////////////////////////
// 图像处理函数

// Alpha运算 （图像透明叠加）
int __stdcall DIAlphaBlend(HDIBIMAGE hBackImage, HDIBIMAGE hOverImage, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0, int nOpacity = 100);

// Alpha运算 （图像透明叠加）叠加后会更改hBackImage各像素的透明度（Alpha值） 支持透明图像的合并
int __stdcall DIAlphaBlendEx(HDIBIMAGE hBackImage, HDIBIMAGE hOverImage, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0, int nOpacity = 100, int nOpacityBk = 100);


// 图像缩放处理

#ifndef IMAGE_INTERPOLATION_DEFINE
#define IMAGE_INTERPOLATION_DEFINE
// 图像插值算法
enum IMAGE_INTERPOLATION
{
	IIN_AUTO,
	IIN_FAST,			// Nearest value (邻近插值)
	IIN_BI_LINER,		// Bi-Liner Interpolation (双线性插值)
	IIN_TRI_CONVOLUTE	// Tri-Convolute Interpolation (卷积插值)
};

#endif

// 只适合32位图像缩放 将hImgSrc缩放到hImgDst上

int __stdcall DIImageResize(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, IMAGE_INTERPOLATION iin = IIN_BI_LINER);
int __stdcall DIImageResizeEx(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, int xDst, int yDst, int cxDst, int cyDst, int xSrc, int ySrc, int cxSrc, int cySrc, IMAGE_INTERPOLATION iin = IIN_BI_LINER); 




// 图像旋转 将根据旋转角度重新为hImgDst分配内存空间
int __stdcall DIRotate(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, int iRotateAngle);
int __stdcall DIRotateSmooth(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, int iRotateAngle);

int __stdcall DIFillColor32(HDIBIMAGE hImage, DWORD argbColor); // 填充颜色 只适用于32位位图

HBITMAP __stdcall DICreateHBitmap(HDIBIMAGE hImage); // 使用hImage转换为HBITMAP供一般Windows应用 返回的句柄必须使用DeleteObject销毁



// 位数据传送 
int __stdcall DIBltImage(HDIBIMAGE hImage, HDIBIMAGE hImageSrc, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0);

struct TRANSFORM_POINTS
{
	POINT ptLeftTop;		// 左上角坐标
	POINT ptRightTop;		// 右上角坐标
	POINT ptRightBottom;	// 右下角坐标
	POINT ptLeftBottom;		// 左下角坐标
};

typedef TRANSFORM_POINTS* LPTRANSFORM_POINTS;

#ifdef USE_OLD_IMAGE_OPERATION

// 传送Mask数据（Alpha数据） 只适用于32位位图 
int __stdcall DIBltMask(HDIBIMAGE hImage, HDIBIMAGE hImageMask, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0);

// 传送Mask数据（Alpha数据）并将全透明部分清0 只适用于32位位图 
int __stdcall DIBltMaskEx(HDIBIMAGE hImage, HDIBIMAGE hImageMask, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0);

int __stdcall DIFillColor32Ex(HDIBIMAGE hImage, DWORD argbColor, int x, int y, int cx = -1, int cy = -1); // 区域填充颜色 只适用于32位位图
int __stdcall DIFillPatchColor32(HDIBIMAGE hImage, int cx, int cy, DWORD argbColor, DWORD argbColor2); // 填充拼贴颜色 只适用于32位位图
int __stdcall DIFillPatchColor32Ex(HDIBIMAGE hImage, int cx, int cy, DWORD argbColor, DWORD argbColor2, int x_off, int y_off); // 填充拼贴颜色 只适用于32位位图 支持偏移

HDIBIMAGE __stdcall DICreateThumbnail(HDIBIMAGE hImage, int width, int height); // 为hImage创建缩略图 返回的缩略图必须使用DIDestroy销毁 目前只适用于32位位图


// 支持Alpha运算的缩放操作 目前只支持IIN_BI_LINER方式 
int __stdcall DIImageAlphaResize(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, int xDst, int yDst, int cxDst, int cyDst, int xSrc, int ySrc, int cxSrc, int cySrc, IMAGE_INTERPOLATION iin = IIN_BI_LINER); 
// 同DIImageAlphaResize函数 坐标允许不在图像范围内 
int __stdcall DIImageAlphaResizeEx(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, int xDst, int yDst, int cxDst, int cyDst, int xSrc, int ySrc, int cxSrc, int cySrc, IMAGE_INTERPOLATION iin = IIN_BI_LINER); 
 
// 支持缩放的Mask传送 目前只支持8位灰度Mask 黑色为透明 
int __stdcall DIImageStretchMask(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, int xDst, int yDst, int cxDst, int cyDst, int xSrc, int ySrc, int cxSrc, int cySrc);
// 同DIImageStretchMask函数 坐标允许不在图像范围内 
int __stdcall DIImageStretchMaskEx(HDIBIMAGE hImgDst, HDIBIMAGE hImgSrc, int xDst, int yDst, int cxDst, int cyDst, int xSrc, int ySrc, int cxSrc, int cySrc);





// 图像变换 将pImageSrc各点映射到pImage中trPoints四个点所围成的区域内 
int __stdcall DITransform(HDIBIMAGE hImage, HDIBIMAGE hImageSrc, LPTRANSFORM_POINTS pTrPoints, int xSrc = 0, int ySrc = 0, int cxSrc = -1, int cySrc = -1, IMAGE_INTERPOLATION iin = IIN_BI_LINER);

// 在图像四周蒙上一层颜色
int __stdcall DIMaskImage(HDIBIMAGE hImage, LPRECT pRect, DWORD dwArgbColor = 0x80000000); // pRect - 不填充蒙板的区域

#endif

// 图像合并Alpha运算 （图像透明叠加）支持图层不透明度
int __stdcall DIMergeImage(HDIBIMAGE hBackImage, HDIBIMAGE hOverImage, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0, int nOpacity = 100, int nOpacityBk = 100);

