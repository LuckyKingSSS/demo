#pragma once

#include "NLEType.h"
#include <Guiddef.h>
#include <objbase.h>
#include "INLEProperties.h"
#include "INLETimelineBase.h"
#include "INLECodec.h"
#include "INLEMediaEncoder.h"
#include "INLEPlayer.h"
#include "INLEXml.h"
#include "INLELogger.h"
#include "INLEFrame.h"
#include "NLEComPtr.h"
#include "INLEDocumentSerialize.h"
#include "INLEFontLibrary.h"
#include "INLERepository.h"
#include "INLEBitmap.h"
#include "INLEImageProc.h"
#include "INLEBackBench.h"
#include "INLEMarkdown.h"


//{F5FBAF87-F7E5-FF43-1BD4-797906CF224E}
DEFINE_GUID(CLSID_NLEFactory, 0xF5FBAF87, 0xF7E5, 0xFF43, 0x1B, 0xD4, 0x79, 0x79, 0x06, 0xCF, 0x22, 0x4E);                   
//{CEF9AFA7-2A98-DE80-180C-36E40DD5E745}
DEFINE_GUID(IID_INLEFactory, 0xCEF9AFA7, 0x2A98, 0xDE80, 0x18, 0x0C, 0x36, 0xE4, 0x0D, 0xD5, 0xE7, 0x45);

/**
 *@brief 对象工厂，用于创建各个NLE对象。
 */
class INLEFactory :public IUnknown
{
public:
	/**
	 *@brief  	初始化
	 *@param	lpDiNLERectory 初始化目录，如安装路径等。
	 *@return 	标准返回
	 */
	virtual HRESULT __stdcall Init(LPCWSTR lpDiNLERectory) = 0;


	/**
	 *@brief  	创建xml对象
	 *@return 	INLEXml* __stdcall
	 */
	virtual NLEComPtr<INLEXml> __stdcall CreateXML() = 0;


	virtual NLEComPtr<INLEDocumentSerialize> __stdcall CreateDocumentSerialize() = 0;

	/**
	 *@brief  	创建属性对象
	 *@return 	INLEProperties* __stdcall
	 */
	virtual NLEComPtr<INLEProperties> __stdcall CreateNLEProperties() = 0;
	

	/**
	 *@brief  	创建一个黑帧
     *@param    nWidth 宽度
     *@param    nHeight 高度
     *@param    fmt 图像格式 @see NLEFormtFormat
	 *@param	pProp 属性表
	 *@return 	帧对象
	 */
	virtual NLEComPtr<INLEFrame> __stdcall CreateNLEVideoFrame(UINT nWidth, UINT nHeight,
		NLEFrameFormat fmt = NLE_FRAME_FORMAT_BGRA, INLEProperties *pProp = NULL) = 0;

	/**
	 *@brief  	创建音频帧
	 *@param	nSampleCount 帧数目
	 *@param	nSampleRate 采样率
	 *@param	nChannels 声道数
	 *@param	fmt 采样格式,如下：
	 *-		NLE_FRAME_FORMAT_PCM_S8  nSampleSize=1
	 *-		NLE_FRAME_FORMAT_PCM_S16 nSampleSize=2
	 *-		NLE_FRAME_FORMAT_PCM_S32 nSampleSize=4
	 *@return	帧接口
	 *@note		创建后总字节数=nSampleCount*nChannles*nSampleSize
	 */
	virtual NLEComPtr<INLEFrame> __stdcall CreateNLEAudioFrame2(UINT nSampleCount, UINT nSampleRate = 44100, UINT nChannels = 2,
		NLEFrameFormat fmt = NLE_FRAME_FORMAT_PCM_S16) = 0;

	/**
	 *@brief  	创建音频帧
	 *@param	ralFramerate 渲染帧率，一般与视频相同
	 *@param	nSampleRate 采样率，默认为44100
	 *@param	nChannels 声道数，默认为2
	 *@param	fmt 格式，默认为NLE_FRAME_FORMAT_PCM_S16
	 *@return 	标准返回
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual NLEComPtr<INLEFrame> __stdcall CreateNLEAudioFrame(NLEFramerate ralFramerate,
		UINT nSampleRate = 44100, 
		UINT nChannels = 2,
		NLEFrameFormat fmt = NLE_FRAME_FORMAT_PCM_S16) = 0;

    /**
	 *@brief  		从磁盘文件创建Timline
	 *@param[in]	pFullFilePath 源文件全路径名
	 *@param[in]	pProp 属性，默认为NULL
	 *@return 		INLETimelineBase对象
	 */
	virtual NLEComPtr<INLETimelineBase> __stdcall CreateTimelineFromFile(LPCWSTR pFullFilePath, INLEProperties *pProp = NULL) = 0;

	/**
	 *@brief  	从已有Timline创建出新Timline
	 *@param	pTimline 已存在的
	 *@param	pProp 创建方式
	 *@return 	新Timline，
	 *@note		令创建后的Timline名称为pReturn，则pTimline是pReturn的Clip了
	 *	- 当pTimeline为NULL时产生一个空Timlinec对象，但可以往里面加入新的Timeline
	 */
	virtual NLEComPtr<INLETimelineBase> __stdcall CreateTimeline(INLETimelineBase* pTimline, INLEProperties *pProp = NULL) = 0;


	/**
	 *@brief  	根据名称创建VideoFilterTimeline
	 *@param	pszFilterName 规则:GUID.FriendName，例子如下:
	 *			- {12E5BC38-3508-44b0-AD1A-D0B37D19289F}.HueShift
	 *@param	pProp 扩展使用，
	 *@return 	新Timeline
	 */
	virtual NLEComPtr<INLETimelineBase> __stdcall CreateVideoFilterTimeline(LPCWSTR pszFilterName, INLEProperties *pProp = NULL) = 0;

	/**
	*@brief  	根据名称创建AudioFilterTimeline
	*@param	pszFilterName 规则:GUID，例子如下: 12E5BC38-3508-44b0-AD1A-D0B37D19289F
	*@param	pProp 扩展使用，
	*@return 	新Timeline
	*/
	virtual NLEComPtr<INLETimelineBase> __stdcall CreateAudioFilterTimeline(LPCWSTR pszFilterName, INLEProperties *pProp = NULL) = 0;

	/**
	*@brief  	根据名称创建FontLibrary
	*@param
	*@param	pProp 扩展使用，
	*@return 	新FontLibrary
	*/
	virtual NLEComPtr<INLEFontLibrary> __stdcall CreateFontLibrary() = 0;
	/**
	*@brief  	根据名称创建CaptionTimeline
	*@param	
	*@param	pProp 扩展使用，
	*@return 	新Timeline
	*/
	virtual NLEComPtr<INLETimelineBase> __stdcall CreateCaptionTimeline(INLEProperties *pProp = NULL) = 0;



    /**
    *@brief 创建一个解码单元
    *@param pFullFilePath 源文件全路径名
    *@param pProp 创建方式
    *@return 新创建的解码单元
    */
	virtual NLEComPtr<INLECodec> __stdcall CreateCodecFromFile(LPCWSTR pFullFilePath, INLEProperties *pProp = NULL) = 0;

	/**
	*@brief 创建一个编码单元
	*@param pProp 创建方式
	*@return 新创建的解码单元
	*/
	virtual NLEComPtr<INLEMediaEncoder> __stdcall CreateMediaEncoder(INLEProperties *pProp = NULL) = 0;


    /**
    *@brief 创建日志对象
    
    *@param pFileName 用于保存日志内容的日志文件名
    *@param nMaxFileSize 设置日志文件的最大字节数，当超过最大字节数后会重命名当前日志文件，
    *       然后用指定的日志文件名再创建一个新的日志文件
    *@param nMaxFileCount 设置日志文件的最大个数，当保存的日志文件超出最大文件个数时会自动删除最早创建的一个日志文件

    *@return 返回日志对象
    *@remark 日志文件名为:
    *  xxx.log
    *  xxx_1.log
    *  xxx_2.log
    *  ...
    */
    virtual NLEComPtr<INLELogger> STDMETHODCALLTYPE CreateLogger(const wchar_t *pFileName,
        int nMaxFileSize = 5 * 1024 * 1024, int nMaxFileCount = 5) = 0;


	virtual NLEComPtr<INLELogger> __stdcall GetDefaultLogger() = 0;

	/**
	 *@brief  	创建Player
	 *@return 	Player对象
	 */
    virtual NLEComPtr<INLEPlayer> __stdcall CreateNLEPlayer() = 0;

	/**
	 *@brief 获取OpenGL渲染上下文
	 */
	virtual LPVOID __stdcall GetOpenGLContext() = 0;

	/**
	*@brief 获取存储仓库
	*/
	virtual NLEComPtr<INLERepository> __stdcall GetRepository() = 0;

	/**
	 *@brief 创建空图像
	 */
	virtual NLEComPtr<INLEImage> __stdcall CreateImage() = 0;

	virtual NLEComPtr<INLEImageProc> __stdcall CreateImageProc() = 0;

	virtual NLEComPtr<INLEBackBench> __stdcall GetBackgroundTaskManager() = 0;
	/**
	 *@brief  	最后一个错误码
	 *@return 	标准返回	 
	 *@note		在搞用其它函数后如果发现出错，则可能通过此函数查看。
	 */
	virtual HRESULT __stdcall GetLastError() = 0;

	/**
	*@brief  	根据模板创建MontageTimeline
	*@param	filePath 模板文件全路径名
	*@param	montageName 蒙太奇名称
	*@return 	新Timeline
	*/
	virtual NLEComPtr<INLETimelineBase> __stdcall CreateMontageTimeline(WCHAR* filePath, WCHAR* montageName) = 0;

	/**
	*@brief  	反初始化
	*@return 	标准返回
	*/
	virtual HRESULT __stdcall Deinit() = 0;

	virtual NLEComPtr<INLEMarkBook> __stdcall CreateMarkBook(LPCWSTR pFullFilePath, INLEProperties *pPropSource = NULL) = 0;

	/**
	*@brief  		从磁盘目录创建Timline
	*@param[in]	pFullFilePath 源目录全路径名{文件路径和文件夹路径都可以}
	*@param[in]	pPropSource 属性，默认为NULL
	*@return 		INLETimelineBase对象
	*/
	virtual NLEComPtr<INLETimelineBase> __stdcall CreateTimelineFromPath(LPCWSTR pFullFilePath, INLEProperties *pPropSource = NULL) = 0;

	/**
	*@brief  		从磁盘文件一个复合Timline，该接口只适用于屏幕录制功能录制出来的文件
	1. 桌面录制+系统声音组合为一个子Timline;
	2.游戏视频+系统声音组合为一个子Timline;
	3.麦克声音为一个子Timline;
	4.摄像头视频为一个子Timline
	*@param[in]	pFullFilePath 源文件全路径名
	*@param[in]	pProp 属性，默认为NULL
	*@return 		INLETimelineBase对象列表
	*/
	virtual NLEComPtr<INLETimelineBase> __stdcall CreateTimelineForScreenRecording(LPCWSTR pFullFilePath, INLEProperties *pPropSource = NULL) = 0;

	/**
	*@brief  	根据名称创建OFXVideoFilterTimeline
	*@param	pszFilterName 示例:"uk.co.thefoundry.OfxInvertExample"
	*@param	pProp 扩展使用，
	*@return 	新Timeline
	*/
	virtual NLEComPtr<INLETimelineBase> __stdcall CreateOFXVideoFilterTimeline(LPCWSTR pszFilterName, INLEProperties *pProp = NULL) = 0;
	
};

INLEFactory* GetNLEFactoryInstance();
