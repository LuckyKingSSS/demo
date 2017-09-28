unit
	ImageRender;
    
interface

{$MINENUMSIZE 4}

uses
    Windows, Image;


// 图像滤镜ID
type
IMAGE_FILTER_ID =
(
	IFI_NONE,		
	IFI_ZOOM_BLUR,				// Zoom模糊 nRadius - 模糊深度
	IFI_GAUSS_BLUR,				// 高斯模糊 nRadius - 模糊半径 
	IFI_SHADOW,					// 添加阴影 nRadius - 不透明度  nParam1 - 阴影颜色（COLORREF） nParam2 - 平滑程度 nParam3 - X方向偏移 nParam4 - Y方向偏移

	IFI_INVERT			= 300,	// 图像反色
	IFI_GRAY_SCALE,				// 灰度图像
	IFI_THRESHOLD,				// 阀值 nRadius (0 - 255)
	IFI_FLIP,					// 垂直翻转
	IFI_FLIP_HORZ,				// 水平翻转
	IFI_EMBOSS,					// 浮雕 nRadius (0 - 100)
	IFI_SPLASH,					// 斑点 nRadius (0 - 255)
	IFI_MOSAIC,					// 马赛克 nRadius (0 - 255)
	IFI_OIL_PAINT,				// 油画 nRadius (0 - 10)
	IFI_3DGRID,					// 3D网格 nParam1 - 网格尺寸（1 - ） nParam2 - 网格深度（1 - ） 
	IFI_WHIRL_PINCH,			// 旋转挤压 nParam1 - 旋转（0 - 360） nParam2 - 挤压（0 - 100） 
	IFI_GRADIENT_RADIAL,		// 放射型渐变 暂不使用
	IFI_GAMMA,					// gamma调节 nRadius (0 - 255)
	IFI_ROTATE90,				// 顺时针旋转90度/rotate 90'
	IFI_RIBBON,					// 带形 nParam1 : 振幅[0..100]，此值为一百分比  nParam2 : 频率>=0，每10为一个pi
	IFI_HALFTONE_M3,			// 半影调 无参数
	IFI_BRIGHTNESS,				// 调节亮度 使用nParam1参数（0 - 200） 
	IFI_CONTRAST,				// 调节对比度 使用nParam1参数（0 - 200） 
	IFI_COLORTONE,				// 单色调 nParam1 - R分量 nParam2 - G分量 nParam3 - B分量
	IFI_HUE_SATURATION,			// 色调饱和度/hue saturation  nParam1 - 色调(-180, 180) nParam2 - 饱和度(-100, 100) 
	IFI_CREATEHALO,				// 制作光晕
	IFI_OLD_PHOTO,				// 老照片 nRadius (0 - 10) 暂时未使用任何参数

	IFI_STRETCH = 500,			// nQuality 取值见IMAGE_INTERPOLATION定义 nParam1 - 缩放后图像的宽度 nParam2 - 缩放后图像的高度
	IFI_ROTATE,					// 旋转 nAngle - (0 - 360)
	
	IFI_COUNT
);								
	
// Image Filter Parameter structure
type
  IMAGE_FILTER_PARAM = record
   
	uFilterID : IMAGE_FILTER_ID;	// 滤镜ID
	nRadius : Integer;				// 半径
	nAngle : Integer;				// 角度
	nQuality : Integer;				// 品质
	nParam1 : Integer;
	nParam2 : Integer;
	nParam3 : Integer;
	nParam4 : Integer;

end;




type
    FILTERPARAM = record
    uFilterID : IMAGE_FILTER_ID;
    IDCaption :  Array[0..63] of WideChar;  //显示的标题
    bRadius : BOOL;  RadiusText:  Array[0..31] of WideChar; RadiusMax, RadiusMin: integer;
    bAngle : BOOL;   AngleText:   Array[0..31] of WideChar;  AngleMax, AngleMin: integer;
    bQuality: BOOL;  QualityText: Array[0..31] of WideChar; QualityMax, QualityMin: integer;
    bParam1: BOOL;   Param1Text:  Array[0..31] of WideChar; Param1Max, Param1Min :integer;
    bParam2: BOOL;   Param2Text:  Array[0..31] of WideChar; Param2Max, Param2Min :integer;
    bParam3: BOOL;   Param3Text:  Array[0..31] of WideChar; Param3Max, Param3Min :integer;
    bParam4: BOOL;   Param4Text:  Array[0..31] of WideChar; Param4Max, Param4Min :integer;
 end;
   LPFILTERPARAM = ^FILTERPARAM;
  LPIMAGE_FILTER_PARAM = ^IMAGE_FILTER_PARAM;

 	LPVOID = Pointer;	//


// 图像滤镜回掉函数定义
//typedef BOOL (__stdcall * IMAGE_FILTER_CALLBACK)(void* pUserObj, UINT uMesg, WPARAM wParam, LPARAM lParam);
type
  IMAGE_FILTER_CALLBACK = function(pUserObj: Pointer; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): DWORD; stdcall; 

// 图像滤镜处理 暂不支持回掉
//BOOL __stdcall IRImageFitler(HDIBIMAGE hImageDst, HDIBIMAGE hImageSrc, LPIMAGE_FILTER_PARAM pParam, IMAGE_FILTER_CALLBACK fnCallback = 0, void* pUserObj = 0);
function IRImageFitler(hImageDst : HDIBIMAGE; hImageSrc : HDIBIMAGE; pParam : LPIMAGE_FILTER_PARAM; fnCallback : IMAGE_FILTER_CALLBACK = nil; pUserObj : LPVOID = nil) : BOOL; stdcall;


//BOOL __stdcall IRGetFilterParam(IMAGE_FILTER_ID uFilterID, LPFILTERPARAM pParam); // 取得Filter参数
function IRGetFilterParam(uFilterID : IMAGE_FILTER_ID; pParam : LPFILTERPARAM) : BOOL; stdcall;

//int __stdcall IRGetFilterCount()  //取得Ｆｉｌｔｅｒ的个数；
function IRGetFilterCount(): integer;  stdcall;

//BOOL __stdcall IRGetFilterItem(int Index , LPFILTERPARAM pParam) //取得第　index项
function IRGetFilterItem(Index: integer; pParam: LPFILTERPARAM): BOOL; stdcall;

implementation

const
    DLLNAME = 'WS_ImageProc.dll';

function IRImageFitler					; external DLLNAME Name 'IRImageFitler';
function IRGetFilterParam				; external DLLNAME Name 'IRGetFilterParam';
function IRGetFilterCount				; external DLLNAME Name 'IRGetFilterCount';
function IRGetFilterItem				; external DLLNAME Name 'IRGetFilterItem';

end.

