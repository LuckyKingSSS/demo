
#pragma once

#define USE_PLAYER_EX 1

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLERender.h" 
#include "INLEFrame.h"



#ifndef HSUBVIEW_DEF
#define HSUBVIEW_DEF
typedef HWND HSUBVIEW;
typedef HWND HVIEW;

#endif  

#ifndef CALLBACK_PARAM
#define CALLBACK_PARAM
#ifdef WIN32
typedef WPARAM WPARAM_PLAYER;
typedef LPARAM LPARAM_PLAYER;
#else
//mac x64
typedef NLEulonglong WPARAM_PLAYER;
typedef NLEulonglong  LPARAM_PLAYER;
#endif

#endif

#define WM_PLAYERMSG		(WM_USER + 2001) //播放器消息

//消息参数wParam
#define PM_Progress			1 //播放中，可用VPGetPlayedTime取得播放时间以计算进度
#define PM_Finish			2 //播放结束 （自动结束才会Post该消息）
#define PM_FrameSpeed       3 
#define PM_Buffering		4 //缓冲
#define PM_Update			5 //update
#define PM_Pause			6 //pause


#define WM_AudioVolume		50 //音量的消息

#ifndef USE_PLAYER_EX

#else

//CALL BACK MSG
enum PLAYER_MSG
{
	PLAYER_MSG_LOAD = 100,	// LOAD消息；消息值：时间戳
	PLAYER_MSG_SEEK,	//暂停状态下的SEEK消息；消息值：时间戳
	PLAYER_MSG_SEEK_PLAYING,//在播放过程中，进行异步SEEK消息；消息值：时间戳

	PLAYER_MSG_PAUSE,	// PAUSE消息；消息值：时间戳
	PLAYER_MSG_PLAYING,	// PAUSE消息；消息值：时间戳
	PLAYER_MSG_UPDATE,  // UPDATE消息；消息值：时间戳
	PLAYER_MSG_STOP,	// STOP消息；消息值：时间戳
	PLAYER_MSG_FINISH,	// FINISH消息；消息值：时间戳

	PLAYER_MSG_FORWARD,  // FORWARD消息；消息值：时间戳
	PLAYER_MSG_BACKWARD, // BACKWARD消息；消息值：时间戳
	PLAYER_MSG_PROGRESS, // PROGRESS消息；消息值：时间戳
	PLAYER_MSG_AUDIOVOLUME, // AUDIOVOLUME消息；消息值：音量值，范围[0,100]
	PLAYER_MSG_PROGRESSFORMARKUP //
};

//PLAYER STATE:播放器当前状态

enum PLAYER_STATE_FLAG
{
	PS_PLAYING = 0x01, //
	PS_PAUSE = 0x02,
	PS_STOP = 0x04,
	PS_LOADING = 0x08,
};
#endif


enum IMAGE_TYPE
{
	IMAGE_BMP,
	IMAGE_JPG,
	IMAGE_PNG
};

struct CaptureImageInfo
{
	int					width;
	int					height;
	IMAGE_TYPE			uImageType;
};

/*!
* \class IPlayerView
* \brief PlayerView接口
*/

// {8E7CE0D9-BF91-4e4f-AF9A-E25C6D9A2C12}
DEFINE_GUID(IID_IPlayerView,0x8e7ce0d9, 0xbf91, 0x4e4f, 0xaf, 0x9a, 0xe2, 0x5c, 0x6d, 0x9a, 0x2c, 0x12);

class IPlayerView: public IUnknown
{
public:
	/*! \fn STDMETHODIMP SetVideoInput(int bSource, IMediaStream* pMediaStream)
	* \brief 设置视频输入流
	* \param bReadStream [in] 设置的源类型，取值如下:
	* \-	 1 仅读取渲染后的视频帧
	* \-	 0 驱动流向前(seek/forwad等)和渲染
	* \param pMediaStream [in] 设置的视频流
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetVideoInput(int bOnlyReadStream, INLERender* pMediaStream) = 0;
	/*! \fn STDMETHODIMP SetParentWND(HVIEW parent)
	* \brief 设置视频显示窗口附着的父窗口
	* \param parent [in] 设置视频显示窗口附着的父窗口
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetParentWND(HVIEW parent) = 0;
	/*! \fn virtual STDMETHODIMP_(HVIEW) GetParentWND(void)
	* \brief 得到视频显示窗口附着的父窗口
	* \retval  视频显示窗口附着的父窗口
	*/
	virtual STDMETHODIMP_(HVIEW) GetParentWND(void) = 0;
	/*! \fn STDMETHODIMP SetParentWND(HVIEW parent)
	* \brief 设置视频显示区域
	* \param rect [in] 设置的视频流显示区域
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetRect(const RECT & rect) = 0;
	/*! \fn virtual STDMETHODIMP_(RECT) GetRect(void)
	* \brief 得到视频显示区域
	* \retval  视频显示区域
	*/
	virtual STDMETHODIMP_(RECT) GetRect(void) = 0;
	/*! \fn virtual STDMETHODIMP_(int)	GetIndex()
	* \brief 得到playview的索引
	* \retval  playview的索引
	*/
	virtual STDMETHODIMP_(int)	GetIndex() = 0;
	/*! \fn virtual STDMETHODIMP_(HSUBVIEW) GetWND(void)
	* \brief 得到视频显示窗口
	* \retval  视频显示窗口
	*/
	virtual STDMETHODIMP_(HSUBVIEW) GetWND(void) = 0;

    virtual STDMETHODIMP_(NLElonglong)	CurrentVideoPTS() = 0;
};

/*!
* \class IPlayerViewMgr
* \brief PlayerView管理器接口
*/

// {08E9EED2-63E7-4895-9C46-8577AD7745CC}
DEFINE_GUID(IID_IPlayerViewMgr,	0x8e9eed2, 0x63e7, 0x4895, 0x9c, 0x46, 0x85, 0x77, 0xad, 0x77, 0x45, 0xcc);

class IPlayerViewMgr : public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP_(IPlayerView *) CreatePlayView(HVIEW parentWND, const RECT & rect)
	* \brief 创建一个新的playview
	* \param parentWND [in] 视频显示窗口附着的父窗口
	* \param rect [in] 视频流显示区域
	* \retval  playview接口
	*/
	virtual STDMETHODIMP_(IPlayerView *) CreatePlayView(HVIEW parentWND, const RECT & rect) = 0;
	/*! \fn virtual STDMETHODIMP_(int) GetCount(void)
	* \brief 获得playview的数目
	* \retval  playview的数目
	*/
	virtual STDMETHODIMP_(int) GetCount(void) = 0;
	/*! \fn virtual STDMETHODIMP Remove(int nIndex)
	* \brief 移除指定索引值的playview
	* \param nIndex [in] 待移除playview的索引
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Remove(int nIndex) = 0;
	/*! \fn virtual STDMETHODIMP GetItem(int nIndex, IPlayerView** ppPlayerView)
	* \brief 得到指定索引值的playview接口
	* \param nIndex [in] playview的指定索引
	* \param ppPlayerView [out] playview的接口
	* \retval  返回状态
	*/
	virtual STDMETHODIMP GetItem(int nIndex, IPlayerView** ppPlayerView) = 0;
};


/*!
* \class IMediaPlayer
* \brief 播放器接口
*/

struct CaptureImageInfo;
// {05A20FDC-2032-4c70-9134-AE340D8FC36F}
DEFINE_GUID(IID_INLEPlayer, 0x5a20fdc, 0x2032, 0x4c70, 0x91, 0x34, 0xae, 0x34, 0xd, 0x8f, 0xc3, 0x6f);
//定义组件对象MediaPlayer
// {6487969C-8280-4371-B037-6FDD91321A3E}
DEFINE_GUID(CLSID_CMediaPlayer, 0x6487969c, 0x8280, 0x4371, 0xb0, 0x37, 0x6f, 0xdd, 0x91, 0x32, 0x1a, 0x3e);
#ifndef USE_PLAYER_EX
class INLEPlayer : public IUnknown
{
public:
	typedef HRESULT(__stdcall *MULTI_CALLBACK_PLAYER)(void* pUserObj, UINT uMsg, WPARAM_PLAYER wParam, LPARAM_PLAYER lParam);
	/*! \fn virtual STDMETHODIMP_(double) GetMovieLength(void)
	* \brief 得到影片长度
	* \retval  返回影片长度
	*/
	virtual STDMETHODIMP_(double) GetMovieLength(void) = 0;
	/*! \fn virtual STDMETHODIMP  SetPlayTime(double dBegin, double dLength)
	* \brief 设置播放时间
	* \param dBegin [in] 播放开始时间
	* \param dLength [in] 播放长度
	* \retval  返回状态
	*/
	virtual STDMETHODIMP SetPlayTime(double dBegin, double dLength) = 0;
	/*! \fn virtual SetCallBack(MULTI_CALLBACK pCallBack, void *pUserObject)
	* \brief 设置回调函数
	* \param pCallBack   [in]  回调函数
	* \param pUserObject [in]  接受回调的对象
	* \retval  返回状态
	*/
	virtual STDMETHODIMP SetCallBack(MULTI_CALLBACK_PLAYER pCallBack, void *pUserObject) = 0;
	/*! \fn virtual STDMETHODIMP GetPlayerViewMgr(IPlayerViewMgr** ppIViewMgr)
	* \brief 得到playview管理器接口
	* \param ppIViewMgr   [out] playview管理器接口
	* \retval  返回状态
	*/
	virtual STDMETHODIMP GetPlayerViewMgr(IPlayerViewMgr** ppIViewMgr) = 0;
	/*! \fn virtual STDMETHODIMP_(int) IsActivate(void)
	* \brief 播放器是否处于活动状态
	* \retval  返回状态
	*/
	virtual STDMETHODIMP_(int) IsActivate(void) = 0;
	/*! \fn virtual STDMETHODIMP Stop(void)
	* \brief 播放器停止播放
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Stop(void) = 0;
	/*! \fn virtual STDMETHODIMP Play(void)
	* \brief 播放器开始播放
	* \retval  返回状态
	*/

	virtual STDMETHODIMP Play(void) = 0;
	/*! \fn virtual STDMETHODIMP Pause(void)
	* \brief 播放器暂停
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Pause(void) = 0;
	/*! \fn virtual STDMETHODIMP Resume(void)
	* \brief 播放器恢复播放
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Resume(void) = 0;
	/*! \fn virtual STDMETHODIMP Seek(double dTime, int accurate)
	* \brief seek到指定时间
	* \param dTime      [in] 指定时间点
	* \param accurate   [in] 精确seek标志
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Seek(double dTime, int accurate) = 0;
	/*! \fn virtual STDMETHODIMP SyncSeek(double dTime, int accurate)
	* \brief 同步方式seek到指定时间
	* \param dTime      [in] 指定时间点
	* \param accurate   [in] 精确seek标志
	* \retval  返回状态
	*/
	virtual STDMETHODIMP SyncSeek(double dTime, int accurate) = 0;
	/*! \fn virtual STDMETHODIMP Forward(void)
	* \brief 前进一帧数据
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Forward(void) = 0;
	/*! \fn virtual STDMETHODIMP SyncForward(void)
	* \brief 同步方式前进一帧数据
	* \retval  返回状态
	*/
	virtual STDMETHODIMP SyncForward(void) = 0;
	/*! \fn virtual STDMETHODIMP Update(int bForce)
	* \brief 强制刷新视频窗口
	* \param bForce [in] 强制刷新窗口标志
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Update(int bForce) = 0;
	/*! \fn virtual STDMETHODIMP CaptureCurrentImage(const CaptureImageInfo* pImageInfo, const wchar_t* pszFile)
	* \brief 截取当前视频图像保存为指定文件
	* \retval  返回状态
	*/
	virtual STDMETHODIMP CaptureCurrentImage(const CaptureImageInfo* pImageInfo, const wchar_t* pszFile) = 0;
	/*! \fn virtual STDMETHODIMP_(const MEDIA_FRAME*) CaptureCurrentImageEx()
	* \brief 截取当前视频图像（YUV数据）
	* \retval  MEDIA_FRAME结构数据
	*/
	virtual STDMETHODIMP_(INLEFrame*) CaptureCurrentImageEx() = 0;

	virtual STDMETHODIMP Load() = 0;

	virtual STDMETHODIMP_(bool) IsSeeking() = 0;

	virtual STDMETHODIMP_(bool) IsPaused() = 0;

	virtual STDMETHODIMP SyncBackForward(void) = 0;

	virtual STDMETHODIMP SetAudioVolumeCallBack(MULTI_CALLBACK_PLAYER pCallBack, void *pUserObject) = 0;

	//1:前一帧后一帧有声音
	//0:没有声音
	virtual STDMETHODIMP SetAudioPlayType(int nAudioType = 1) = 0;
	virtual STDMETHODIMP SetAudioInput(INLERender*  pMediaStream) = 0;

};

#else

class INLEPlayer : public IUnknown
{
public:
	typedef HRESULT(__stdcall *MULTI_CALLBACK_PLAYER)(void* pUserObj, UINT uMsg, WPARAM_PLAYER wParam, LPARAM_PLAYER lParam);
	/*! \fn virtual STDMETHODIMP_(long long) GetMovieLength(void)
	* \brief   得到影片总帧数  单位：帧（由RENDER 提供总帧数）
	* \retval  返回影片总帧数
	*/
	virtual STDMETHODIMP_(long long) GetMovieLength(void) = 0;
	/*! \fn virtual STDMETHODIMP  SetPlayTime(double dBegin, double dLength)
	* \brief 设置播放时间：单位：帧
	* \param dBegin [in] 播放开始帧位置
	* \param dLength [in] 播放总帧数
	* \retval  返回状态
	*/
	virtual STDMETHODIMP SetPlayTime(long long dBegin, long long dLength) = 0;
	/*! \fn virtual STDMETHODIMP GetPlayerViewMgr(IPlayerViewMgr** ppIViewMgr)
	* \brief 得到playview管理器接口
	* \param ppIViewMgr   [out] playview管理器接口
	* \retval  返回状态
	*/
	virtual STDMETHODIMP GetPlayerViewMgr(IPlayerViewMgr** ppIViewMgr) = 0;
	/*! \fn virtual STDMETHODIMP_(int) IsActivate(void)
	* \brief 播放器是否处于活动状态
	* \retval  返回状态
	*/
	virtual STDMETHODIMP_(PLAYER_STATE_FLAG) GetPlayerState(void) = 0;

	/*! \fn virtual STDMETHODIMP Load(void)
	* \brief 初始化当前视频文件播放线程
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Load() = 0;
	/*! \fn virtual STDMETHODIMP Play(void)
	* \brief 播放器开始播放
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Play(void) = 0;
	/*! \fn virtual STDMETHODIMP Pause(void)
	* \brief 播放器暂停
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Pause(void) = 0;;

	/*! \fn virtual STDMETHODIMP Stop(void)
	* \brief 播放器停止播放
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Stop(void) = 0;

	/*! \fn virtual STDMETHODIMP Seek(long long dTime, int accurate)
	* \brief seek到指定时间
	* \param dTime      [in] 指定时间点
	* \param accurate   [in] 精确seek标志
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Seek(long long dTime, int accurate) = 0;

	/*! \fn virtual STDMETHODIMP Forward(void)
	* \brief 前进一帧
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Forward(void) = 0;

	/*! \fn virtual STDMETHODIMP BackForward(void)
	* \brief 后退一帧
	* \retval  返回状态
	*/
	virtual STDMETHODIMP BackForward(void) = 0;

	/*! \fn virtual STDMETHODIMP Update(int bForce)
	* \brief 强制刷新视频窗口
	* \param bForce [in] 强制刷新窗口标志
	* \retval  返回状态
	*/
	virtual STDMETHODIMP Update(int bForce) = 0;
	/*! \fn virtual STDMETHODIMP CaptureCurrentImage(const CaptureImageInfo* pImageInfo, const wchar_t* pszFile)
	* \brief 截取当前视频图像保存为指定文件
	* \retval  返回状态
	*/
	virtual STDMETHODIMP CaptureCurrentImage(const CaptureImageInfo* pImageInfo, const wchar_t* pszFile) = 0;
	/*! \fn virtual STDMETHODIMP_(const MEDIA_FRAME*) CaptureCurrentImageEx()
	* \brief 截取当前视频图像（YUV数据）
	* \retval  MEDIA_FRAME结构数据
	*/
	virtual STDMETHODIMP_(INLEFrame*) CaptureCurrentImageEx() = 0;

	/*! \fn virtual SetCallBack(MULTI_CALLBACK pCallBack, void *pUserObject)
	* \brief 设置播放回调函数
	* \param pCallBack   [in]  回调函数
	* \param pUserObject [in]  接受回调的对象
	* \retval  返回状态
	*/
	virtual STDMETHODIMP SetCallBack(MULTI_CALLBACK_PLAYER pCallBack, void *pUserObject) = 0;

	/*! \fn virtual SetAudioVolumeCallBack(MULTI_CALLBACK pCallBack, void *pUserObject)
	* \brief 设置音量回调函数
	* \param pCallBack   [in]  回调函数
	* \param pUserObject [in]  接受回调的对象
	* \retval  返回状态
	*/
	virtual STDMETHODIMP SetAudioVolumeCallBack(MULTI_CALLBACK_PLAYER pCallBack, void *pUserObject) = 0;

	/*! \fn virtual STDMETHODIMP SetAudioPlayType(int nAudioType = 1)
	* \brief 设置音频在单帧前进或后退，是否音频单帧前进或后退，
	* \param nAudioType   [in]  默认启用
	* \retval  返回状态
	*/
	virtual STDMETHODIMP SetAudioPlayType(int nAudioType = 1) = 0;

	/*! \fn virtual STDMETHODIMP SetAudioInput(INLERender*  pMediaStream)
	* \brief 设置播放音频流
	* \param pMediaStream   [INLERender]
	* \retval  返回状态
	*/
	virtual STDMETHODIMP SetAudioInput(INLERender*  pMediaStream) = 0;

	/*! \fn virtual STDMETHODIMP SetVolume(int nVolume)
	* \brief 设置播放器音量
	* \param nVolume  值为0 - 100， 0表示静音，100表示最大声音
	* \retval  返回状态
	*/
	virtual STDMETHODIMP SetVolume(int nVolume) = 0;

};
#endif



