
/*! \file MediaPlay.h
* \brief							播发器部分接口说明
* Detailed
*播放器接口提供了播放控制功能，以及设置多个视频显示View。 为了设置音频流输入，通过查询接口IAudioInput实现。
*为了实现双播放器，通过IMediaPlay接口的GetPlayerViewMgr方法获取IPlayViewMgr接口，通过其CreatePlayView方法，
*添加多个View，对于每个View，通过IPlayerView接口的SetVideoInput方式，设置该View的视频流输入接口，并指明该流是否是原始流。
*通过SetParentWND方法，设置视频显示窗口。SetRect设置视频显示区域
*/
#pragma once
#include <scombase.h>
#include "COMMacro.h"
#include "CommonInterface.h"
#include "CallBack.h"
#include "WSPlayer.h"

/*!
* \class IPlayerView
* \brief PlayerView接口 
*/

// {8E7CE0D9-BF91-4e4f-AF9A-E25C6D9A2C12}
DEFINE_GUID(IID_IPlayerView, 
			0x8e7ce0d9, 0xbf91, 0x4e4f, 0xaf, 0x9a, 0xe2, 0x5c, 0x6d, 0x9a, 0x2c, 0x12);
EXTERN_C const IID IID_IPlayerView;
MACRO_MIDL_INTERFACE(IPlayerView, "8E7CE0D9-BF91-4e4f-AF9A-E25C6D9A2C12")
: public IUnknown
{
public:
	/*! \fn STDMETHODIMP SetVideoInput(int bSource, IMediaStream* pMediaStream)
	* \brief 设置视频输入流
	* \param bSource [in] 设置的源类型 0 非源视频流 1 源视频流 
	* \param pMediaStream [in] 设置的视频流
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetVideoInput(int bSource, IMediaStream* pMediaStream) = 0;
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
};

/*!
* \class IPlayerViewMgr
* \brief PlayerView管理器接口 
*/

// {08E9EED2-63E7-4895-9C46-8577AD7745CC}
DEFINE_GUID(IID_IPlayerViewMgr, 
			0x8e9eed2, 0x63e7, 0x4895, 0x9c, 0x46, 0x85, 0x77, 0xad, 0x77, 0x45, 0xcc);
EXTERN_C const IID IID_IPlayerViewMgr;
MACRO_MIDL_INTERFACE(IPlayerViewMgr, "08E9EED2-63E7-4895-9C46-8577AD7745CC")
: public IUnknown
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

struct CaptureImageInfo ;
// {05A20FDC-2032-4c70-9134-AE340D8FC36F}
DEFINE_GUID(IID_IMediaPlayer, 
			0x5a20fdc, 0x2032, 0x4c70, 0x91, 0x34, 0xae, 0x34, 0xd, 0x8f, 0xc3, 0x6f);
EXTERN_C const IID IID_IMediaPlayer;
MACRO_MIDL_INTERFACE(IMediaPlayer, "05A20FDC-2032-4c70-9134-AE340D8FC36F")
: public IUnknown
{
public:
	typedef HRESULT (__stdcall *MULTI_CALLBACK)(void* pUserObj, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
	virtual STDMETHODIMP SetCallBack(MULTI_CALLBACK pCallBack, void *pUserObject) = 0;
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
	virtual STDMETHODIMP Update(int bForce)=0;
	/*! \fn virtual STDMETHODIMP CaptureCurrentImage(const CaptureImageInfo* pImageInfo, const wchar_t* pszFile)
	* \brief 截取当前视频图像保存为指定文件
	* \retval  返回状态 
	*/
	virtual STDMETHODIMP CaptureCurrentImage(const CaptureImageInfo* pImageInfo, const wchar_t* pszFile)=0;
	/*! \fn virtual STDMETHODIMP_(const MEDIA_FRAME*) CaptureCurrentImageEx() 
	* \brief 截取当前视频图像（YUV数据）
	* \retval  MEDIA_FRAME结构数据 
	*/
	virtual STDMETHODIMP_(const MEDIA_FRAME*) CaptureCurrentImageEx() =0;

    virtual STDMETHODIMP Load() = 0;

	virtual STDMETHODIMP_(bool) IsSeeking() = 0;

	virtual STDMETHODIMP_(bool) IsPaused() = 0;

	/*virtual STDMETHODIMP SyncBackForward(void) = 0;

	virtual STDMETHODIMP SetAudioVolumeCallBack(MULTI_CALLBACK pCallBack, void *pUserObject)=0;

	//1:前一帧后一帧有声音
	//0:没有声音
	virtual STDMETHODIMP SetAudioPlayType(int nAudioType = 1) = 0;*/
};

//定义组件对象MediaPlayer
// {6487969C-8280-4371-B037-6FDD91321A3E}
MIDL_DEFINE_GUID(CLSID, CLSID_CMediaPlayer, 0x6487969c, 0x8280, 0x4371, 0xb0, 0x37, 0x6f, 0xdd, 0x91, 0x32, 0x1a, 0x3e);

EXTERN_C const CLSID CLSID_CMediaPlayer;