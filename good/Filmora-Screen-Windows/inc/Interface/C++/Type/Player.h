#pragma once

#ifdef WIN32

	typedef HWND HSUBVIEW;
	typedef HWND HVIEW;
#else
	typedef void * HSUBVIEW;
	typedef void * HVIEW;
	
#endif


typedef void *HPLAYER;

class AudioDecoder;
class VideoDecoder;
typedef VideoDecoder *HVIDEODECODER;	// VideoDecoder句柄定义
typedef AudioDecoder *HAUDIODECODER;	// AudioDecoder句柄定义

#define WM_PLAYERMSG		(WM_USER + 2001) //播放器消息
#ifndef WIN32
#define WM_CLOSE                       WM_USER + 0x0010
#endif
//消息参数wParam
#define PM_Progress			1 //播放中，可用VPGetPlayedTime取得播放时间以计算进度
#define PM_Finish			2 //播放结束 （自动结束才会Post该消息）
#define PM_SeekFinish		3 //SEEK结束 可以准确获取进度

#ifndef IMAGE_INTERPOLATION_DEFINE
#define IMAGE_INTERPOLATION_DEFINE
// 图像插值算法
enum IMAGE_INTERPOLATION
{
	IIN_AUTO,			// 自动选择插值算法
	IIN_FAST,			// Nearest value (邻近插值)
	IIN_BI_LINER,		// Bi-Liner Interpolation (双线性插值)
	IIN_TRI_CONVOLUTE	// Tri-Convolute Interpolation (卷积插值)
};

#endif

#ifndef IMAGE_RESIZE_METHOD_DEFINE
#define IMAGE_RESIZE_METHOD_DEFINE

enum IMAGE_RESIZE_METHOD
{
	IRM_FULLSCREEN,		// stretch to full screen 
	IRM_ORIGINAL_SCALE, // keep scale, fit to size of output
	IRM_16_9,			// resize to 16:9, fit to size of output
	IRM_4_3,			// resize to 4:3, fit to size of output
	IRM_LETTERBOX,
	IRM_PAN_SCAN
};
#endif

#ifndef RESIZE_PARAM_DEFINE
#define RESIZE_PARAM_DEFINE

struct RESIZE_PARAM
{
	int					width;
	int					height;
	IMAGE_INTERPOLATION	uInterpolation;
	IMAGE_RESIZE_METHOD uStyle;
};
#endif

#ifndef DEINTERLACE_INTERPOLATION_DEF
#define DEINTERLACE_INTERPOLATION_DEF

enum DEINTERLACE_STYLE
{
	DIS_NONE,				
	DIS_ODD,				// 重复奇数行
	DIS_EVEN,				// 重复偶数行
	DIS_ODD_INTERPOLATE,	// 使用奇数行插值
	DIS_EVEN_INTERPOLATE,	// 使用偶数行插值
	DIS_AUTO,				// 自动处理（暂不支持）
};

#endif // DEINTERLACE_INTERPOLATION_DEF

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef WIN32
void __stdcall VPInit();

// 关闭GDI+
void __stdcall VPUninit();
#endif

//创建播放器窗口rcPos－窗口位置
HPLAYER __stdcall VPCreate(HVIEW hWndParent, LPRECT pRect); 

//销毁播放器窗口
void __stdcall VPDestroy(HPLAYER hPlayer);

//设定解码器
void __stdcall VPSetDecoder(HPLAYER hPlayer, HVIDEODECODER hVideo, HAUDIODECODER hAudio);

//根据VPSetDecoder所设定decoder，得到视频段长度
double __stdcall VPGetMovieLength(HPLAYER hPlayer);

//设置视频播放时间， dBegin——开始时间，dLength——持续时间，为0播放到结尾
void __stdcall VPSetPlayTime(HPLAYER hPlayer, double dBegin, double dLength);

//播放，dBegin——开始时间，dLength——持续时间，为0播放到结尾
void __stdcall VPPlay(HPLAYER hPlayer);

//停止播放
void __stdcall VPStop(HPLAYER hPlayer);

//暂停
void __stdcall VPPause(HPLAYER hPlayer);

//恢复
void __stdcall VPResume(HPLAYER hPlayer);

//定位到dTime,dTime为相对时间 以VPSetPlayTime设置的dBegin为基准 
void __stdcall VPSeek(HPLAYER hPlayer, double dTime, int accurate = TRUE);

//功能同VPSeek，会等待完成才返回 
void __stdcall VPSyncSeek(HPLAYER hPlayer, double dTime, int accurate = TRUE);

//单帧前进，须先调用VPPause
void __stdcall VPForward(HPLAYER hPlayer);

//功能同VPForward，会等待完成才返回 
void __stdcall VPSyncForward(HPLAYER hPlayer);

//定义回调窗口,消息为WM_PLAYERMSG,wParam参数见PM_Progress,PM_Finish定义
void __stdcall VPSetCallBack(HPLAYER hPlayer, HVIEW hCBWnd);

//取得播放器子窗口句柄
HSUBVIEW __stdcall VPGetPlayerHwnd(HPLAYER hPlayer);

//取得播放时间,相对于VPSetPlayTime中dBegin,0～dLength
double __stdcall VPGetPlayedTime(HPLAYER hPlayer);

//取显示信息
IMAGE_RESIZE_METHOD __stdcall VPGetDisplayResizeMode(HPLAYER hPlayer);

//设置播放比例
int __stdcall VPSetDisplayResizeMode(HPLAYER hPlayer, IMAGE_RESIZE_METHOD irm);


// Crop区域更改消息
#define UM_CROP_CHANGE		(WM_USER + 4326)

// 设置播放器窗口的编辑模式 
void __stdcall VPSetCropEditStyle(HPLAYER hPlayer, UINT uCropStyle = 1);

// 设置Crop区域  图像坐标 非窗口坐标
void __stdcall VPSetCropRect(HPLAYER hPlayer, LPRECT pCropRect); 

// 获取Crop区域  图像坐标 非窗口坐标
void __stdcall VPGetCropRect(HPLAYER hPlayer, LPRECT pCropRect); 

//刷新显示
void __stdcall VPUpdate(HPLAYER hPlayer);

// 播放器是否处于活动状态，包括正在播放和Pause状态
// 0 - 不活动状态， 1- 正在播放， 2 - 暂停状态
int __stdcall VPIsActivate(HPLAYER hPlayer);

enum IMAGE_TYPE
{
	IMAGE_BMP,
	IMAGE_JPG,
	IMAGE_PNG
};

struct CaptureImageInfo
{
	RESIZE_PARAM		uResizeParam;
	DEINTERLACE_STYLE	uDeInterlace;
	IMAGE_TYPE			uImageType;
};

// 抓取视频图片
int __stdcall VPCaptureCurrentImage(HPLAYER hPlayer, const CaptureImageInfo *pCaptureImageInfo, const wchar_t *pszFile);

// Message Code:
//
#define VM_SAVE_IMAGE			0x0100 // wParam - LPBITMAPINFO  lParam - file name (char*)
#define VM_SAVE_YUV_2_BMP		0x0101 // wParam - const YUVImage *  lParam - file name (char*)

#define VM_TRACE_TEXT			0x0200 // wParam - COLORREF  lParam - Text (char*)

#define VM_FULL_SPEED			0x0800
#define VM_NORMAL_SPEED			0x0801
#define VM_GET_SPEED			0x0802 // 取得当前播放器每秒播放的帧数

DWORD __stdcall VPSendCommand(HPLAYER hPlayer, UINT uMsg, WPARAM wParam, LPARAM lParam);

#ifdef	__cplusplus
}
#endif



	#pragma warning(disable: 4996)
	#pragma warning(disable: 4995)

// 时间，帧，Timecode转换

__inline double Frame2Time(int nFrame, double dFrameRate)
{
	if (dFrameRate < 0.00001)
		return 0;
	return nFrame / dFrameRate;
}

__inline int Time2Frame(double dTime, double dFrameRate)
{
	return (int)(dTime * dFrameRate + 0.5);
}

__inline const char* Frame2TimeCode(int nFrame, double dFrameRate, char szBuf[16])
{
	int nFrameRate = (int)(dFrameRate + 0.5); 
	if (nFrameRate <= 0)
	{
		sprintf(szBuf, "%02x:%02x:%02x:%02x", (BYTE)-1, (BYTE)-1, (BYTE)-1, (BYTE)-1);
		return szBuf;
	}

	int h = nFrame / (nFrameRate * 3600);
	int tmp = nFrame - h * nFrameRate * 3600;
	int m = tmp / (nFrameRate * 60);
	tmp -= m * nFrameRate * 60;
	int s = tmp / nFrameRate;
	tmp -= s * nFrameRate;
	int f = tmp;

	sprintf(szBuf, "%02d:%02d:%02d:%02d", h, m, s, f);

	return szBuf;
}

__inline const char* Time2TimeCode(double dTime, double dFrameRate, char szBuf[16])
{
	if (dFrameRate < 0.001)
		dFrameRate = 100;
	if (dTime < 0)
		dTime = 0;

	int nFrame = Time2Frame(dTime, dFrameRate);
	return Frame2TimeCode(nFrame, dFrameRate, szBuf);
}

__inline int TimeCode2Frame(const char* pszTimeCode, double dFrameRate)
{
	int nFrameRate = (int)(dFrameRate + 0.5); 
	int h, m, s, f;
	sscanf(pszTimeCode, "%02d:%02d:%02d:%02d", &h, &m, &s, &f);

	return h * 3600 * nFrameRate + m * 60 * nFrameRate + s * nFrameRate + f;
}

__inline double TimeCode2Time(const char* pszTimeCode, double dFrameRate)
{
	int nFrame = TimeCode2Frame(pszTimeCode, dFrameRate);
	return Frame2Time(nFrame, dFrameRate);
}



