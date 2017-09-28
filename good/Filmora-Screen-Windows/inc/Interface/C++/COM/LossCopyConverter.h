/* ### WS@H Project:Multimedia_Platform ### */

/*! \file MediaConverter.h
* \brief							转换部分接口声明
* Detailed
*IMediaConverter接口提供了，获取以加载的编码模块信息，设置编码参数，设置文件拆分信息，转换控制的方法。
*应用层可以通过调用GetEncodeParam方法获取到IMediaEncParam接口，用于设置编码参数。4.0跟以前有显著变化的是，
*结构体VideoParam，AudioParam变成了StreamParam，新结构体使用Buffer来保存各种特有的编码参数。
*SetFileParam方法用于设置文件的一些信息，如ID3。
*/

#pragma once
#include <scombase.h>
#include "COMMacro.h"
#include "ConverterType.h"
#include "CallBack.h"
#include "CommonInterface.h"
#include "CTCommonExternal.h"

//struct ST_MEDIA_FILE_INFO;
//struct KeyFrameIndexArr;

enum CLIP_LOSSCOPY_MODE
{
	LOSSCOPY_SPLIT_BASE_ON_TIME,	
	LOSSCOPY_SPLIT_BASE_ON_SIZE
};

enum FILE_LOSSCOPY_MODE
{
	FILE_LOSSCOPY_MODE_SPLITER,//把一个文件，分割成多个独立文件
	FILE_LOSSCOPY_MODE_MERGE, //抽取不同文件的相同流合成一个文件流
	FILE_LOSSCOPY_MODE_STREAMMIX //抽取不同文件的不同流混合成一个文件流
};

enum CALL_BACK_TYPE
{
	CALL_BACK_TYPE_PROCESS_CONVERTER,
	CALL_BACK_TYPE_PROCESS_KEYINDEX
};

enum MSG_LOSSCOPY_TYPE
{
	MSG_LOSSCOPY_COVERTER_TASK_BEGIN = 0,
	MSG_LOSSCOPY_COVERTER_TASK_END,		//对于多个文件分割，整个task结束
	MSG_LOSSCOPY_COVERTER_FILE_BEGIN, //文件转换开始
	MSG_LOSSCOPY_COVERTER_FILE_END,		//文件转换结束
	MSG_LOSSCOPY_COVERTER_CLIP_BEGIN,	//文件clip段开始
	MSG_LOSSCOPY_COVERTER_CLIP_END,		//文件clip结束
	MSG_LOSSCOPY_COVERTER_PROGRESS,		//转换总进度，进度最大值为所有待合并文件时长
	MSG_LOSSCOPY_KEYINDEX_PROGRESS,		//获取索引转换进度，
	MSG_LOSSCOPY_KEYINDEX_FILE_BEGIN,
	MSG_LOSSCOPY_KEYINDEX_FILE_END,
	MSG_LOSSCOPY_EXCEPTION_CLIP_END, //转换异常
	MSG_LOSSCOPY_COVERTER_CLIP_PROGRESS //针对分割，每个CLIP转换进度，对于每个CLIP都从0开始

};

//每一组的文件ID集合
struct MERGE_LOSSCOPY_CHECK_ITEM_INFO
{
   __int64* nFileIdList;
	__int64 nListSize;

};

//合并 分组
struct MERGE_LOSSCOPY_CHECK_RESULT_INFO
{
	MERGE_LOSSCOPY_CHECK_ITEM_INFO* pGrouplist;
	__int64 nListSize;
};




struct SPLITER_MSG_INFO
{
	__int64 nFileID;
	__int64 nClipIdx;
};

//CLIP TIEM
struct ClipTimeLoss
{
	__int64 dBeginTime;  //unit:1/10000000 s
	__int64 dLength;
	__int64 dFrameOffset; // package num
};

//CLIP SIZE
struct ClipSizeLoss
{
	__int64 nBeginPos;
	__int64 nSize;			//clip大小，单位字节
	__int64 dFrameOffset;
};

class         CWSSample;
/*!
* \class IMediaStreamEx
* \brief 获取流信息 
*/
// {35B968AB-69C3-4b2d-BB20-968F43886FEB}
DEFINE_GUID(IID_IMediaStreamEx, 
			0x35b968ab, 0x69c3, 0x4b2d, 0xbb, 0x20, 0x96, 0x8f, 0x43, 0x88, 0x6f, 0xeb);
EXTERN_C const IID IID_IMediaStreamEx;
MACRO_MIDL_INTERFACE(IMediaStreamEx, "35B968AB-69C3-4b2d-BB20-968F43886FEB")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP GetStreamInfo(IStreamInfo** ppStreamInfo)
	* \brief  获取当前流的信息
	* \param ppStreamInfo [in / out] 输出流信息接口
	* \retval 获取结果
	* \see IStreamInfo 
	*/
	virtual STDMETHODIMP GetStreamInfo(IStreamInfo** ppStreamInfo) = 0;

	/*! \fn virtual STDMETHODIMP ChangeStream(int nNewStreamID)
	* \brief  变换一路流
	* \param nNewStreamID [in] 新流ID
	* \retval 变换结果
	*/
	virtual STDMETHODIMP ChangeStream(int nNewStreamID) = 0;

	/*! \fn virtualSTDMETHODIMP_(CWSSample *) GetSampleEx(void)
	* \brief  获取当前的帧(视频) 或 Sample(音频)
	* \retval 获取的结果
	* \see CWSSample
	*/

	virtual STDMETHODIMP_(CWSSample *) GetSampleEx(void) = 0;

	/*! \fn virtual STDMETHODIMP_(int) Forward(void)
	* \brief  前进下一帧(视频) 或 下一个Sample (音频)
	* \retval 1 表示以前进一帧，0表示解码结束
	*/
	virtual STDMETHODIMP_(int) Forward(void) = 0;


	/*! \fn virtual STDMETHODIMP_(int) Backward(void)
	* \brief  后退上一帧(视频) 或 上一个Sample (音频)
	* \retval 1 表示成功后退到上一帧，0表示后退失败
	*/
	virtual STDMETHODIMP_(int) Backward(void) = 0;

	/*! \fn virtual STDMETHODIMP_(double) Seek(double dTime, int accurate)
	* \brief seek到某个时间点
	* \param dTime [in]  seek的时间点
	* \param accurate [in] 是否要精确seek
	* \retval seek到的时间点
	*/
	virtual STDMETHODIMP_(double) Seek(double dTime, int accurate) = 0;

	/*! \fn virtual STDMETHODIMP_(int) NotifyStreamMessage(const ULONG  nMessage,const LONGLONG  nParam, void*  lpParam, void*  pPrivate)
	* \brief 通知流结束·
	* \retval 
	*/
	virtual STDMETHODIMP_(int) NotifyStreamMessage(const ULONG  nMessage,const LONGLONG  nParam, void*  lpParam, void*  pPrivate) = 0;
	
	/*! \fn virtualSTDMETHODIMP_(CWSSample *) GetEmptySampleEx(void)
	* \brief  获取空帧(视频) 或 Sample(音频)
	* \retval 获取的结果
	* \see CWSSample
	*/

	virtual STDMETHODIMP_(CWSSample *) GetOriginSampleEx(void)=0;
};

//进度回调
typedef HRESULT (__stdcall *MEDIALOSSCOPY_MULTI_CALLBACK)(void* pUserObj, UINT uMsg, void* pParam, LPARAM lParam);


/*!
* \class IMerge_LossCopy_Check_Info
* \brief IMerge_LossCopy_Check_Info接口
*/

// {FC48C7F4-4AB9-4ad0-AE80-BD24FC224DF3}
DEFINE_GUID( IID_IMerge_LossCopy_Check_Info, 0xfc48c7f4, 0x4ab9, 0x4ad0, 0xae, 0x80, 0xbd, 0x24, 0xfc, 0x22, 0x4d, 0xf3);
EXTERN_C const IID IID_IMerge_LossCopy_Check_Info;
MACRO_MIDL_INTERFACE(IMerge_LossCopy_Check_Info, "FC48C7F4-4AB9-4ad0-AE80-BD24FC224DF3")
: public IUnknown
{
	
	/*! \fn virtual STDMETHODIMP_(MERGE_LOSSCOPY_CHECK_RESULT) GetMergeGroupInfo(void)
	* \brief 释放获取信息
	* \param void
	* \retval MERGE_LOSSCOPY_CHECK_RESULT 结果
	*/

	virtual STDMETHODIMP_(MERGE_LOSSCOPY_CHECK_RESULT_INFO*) GetMergeGroupInfo(void) = 0;

	/*! \fn virtual STDMETHODIMP ReleaseMergeGroupInfo(MERGE_LOSSCOPY_CHECK_RESULT groupinfo);
	* \brief 释放获取信息对象
	* \param 设置输入MERGE_LOSSCOPY_CHECK_RESULT groupinfo
	* \retval
	*/
	virtual STDMETHODIMP ReleaseMergeGroupInfo(MERGE_LOSSCOPY_CHECK_RESULT_INFO* groupinfo) = 0;
};



/*!
* \class ILossCopyMediaInfo
* \brief ILossCopyMediaInfo接口
*/

// {104B3F4D-B9CB-48f1-9D3D-504B2494FEF2}
DEFINE_GUID(IID_ILossCopyMediaInfo, 
0x104b3f4d, 0xb9cb, 0x48f1, 0x9d, 0x3d, 0x50, 0x4b, 0x24, 0x94, 0xfe, 0xf2);
EXTERN_C const IID IID_ILossCopyMediaInfo;
MACRO_MIDL_INTERFACE(ILossCopyMediaInfo, "104B3F4D-B9CB-48f1-9D3D-504B2494FEF2")
: public IUnknown
{
	virtual STDMETHODIMP_(const ST_MEDIA_FILE_INFO*) GetFileMediaInfo() = 0;
};


/*!
* \class IMediaConverter
* \brief converter接口 ,时间单位 1/ 10000000 秒
*/

// {FB138BF6-5614-4d3e-A1D7-33720929D77E}
DEFINE_GUID(IID_IMediaLossCopyConverter, 
0xfb138bf6, 0x5614, 0x4d3e, 0xa1, 0xd7, 0x33, 0x72, 0x9, 0x29, 0xd7, 0x7e);
EXTERN_C const IID IID_IMediaLossCopyConverter;
MACRO_MIDL_INTERFACE(IMediaLossCopyConverter, "FB138BF6-5614-4d3e-A1D7-33720929D77E")
: public IUnknown
{
	/*! \fn virtual STDMETHODIMP AddInputFile(const BSTR pInputPath)
	* \brief 设置输入文件
	* \param pInputPath [in]   输入文件
	* \retval  文件ID ,-1表示添加文件失败
	*/
	virtual STDMETHODIMP_(__int64) AddInputFile(const BSTR pInputPath) = 0;
	/*! \fn virtual STDMETHODIMP RemoveInputFile(__int64 nFileID)
	* \brief 移除指定文件
	* \param nFileID [in]   输入文件ID
	* \retval 设置结果
	*/
	virtual STDMETHODIMP RemoveInputFile(__int64 nFileID) = 0;
	/*! \fn virtual STDMETHODIMP  ClearAllInputFile(void)
	* \brief 清空所有文件
	* \retval  设置结果
	*/
	virtual STDMETHODIMP ClearAllInputFile(void) =0;

	/*! \fn virtual STDMETHODIMP SetOutputFolder(const BSTR pOutputPath)
	* \brief 设置输出文件
	* \param pOutputPath [in]   merge 和 STREAMMIX模式设置全路径名，其他设置输出文件夹路径不带\\
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetOutputFolder(const BSTR pOutputPath) = 0;
	/*! \fn virtual STDMETHODIMP AddClipTime( __int64 nFileID ,ClipTimeLoss ctClipTime) 
	* \brief 设置clip时间
	* \param nFileID [in]		文件ID
	* \param ClipTimeLoss [in]   cliptime,如果文件获取索引<=1 ,那么ClipTimeLoss.dFrameOffset = -1
	* \retval  设置结果
	*/
	virtual STDMETHODIMP AddClipTime( __int64 nFileID ,ClipTimeLoss ctClipTime) = 0;
	/*! \fn virtual STDMETHODIMP RemoveClipTime( __int64 nFileID ,__int64 nClipIndex) 
	* \brief 获得clip时间
	* \param nFileID [in]		文件ID
	* \param nClipIndex [in]	CLIP段ID
	* \retval  获得结果
	*/
	virtual STDMETHODIMP RemoveClipTime( __int64 nFileID ,__int64 nClipIndex) = 0;
	/*! \fn virtual STDMETHODIMP GetClipTime(__int64 nFileID ,__int64 nClipIndex,ClipTimeLoss* ctClipTime) 
	* \brief 移除clip时间
	* \param nFileID [in]		文件ID
	* \param nClipIndex [in]	CLIP段ID
	* \param ctClipTime [out]   clip时间
	* \retval  获得结果
	*/
	virtual STDMETHODIMP GetClipTime(__int64 nFileID ,__int64 nClipIndex,ClipTimeLoss* ctClipTime) = 0;
	/*! \fn virtual STDMETHODIMP SetClipTime(__int64 nFileID ,__int64 nClipIndex,ClipTimeLoss ctClipTime)
	* \brief 设置clip时间
	* \param nFileID [in]		文件ID
	* \param nClipIndex [in]	CLIP段ID
	* \param ctClipTime [out]   clip时间
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetClipTime(__int64 nFileID ,__int64 nClipIndex,ClipTimeLoss ctClipTime) = 0;


	/*! \fn virtual STDMETHODIMP AddClipSize(__int64 nFileID,ClipSizeLoss csClipSize) 
	* \brief 设置clip大小
	* \param nFileID [in]		文件ID
	* \param csClipSize [in]   clip开始大小,默认从文件第一帧开始计算clip大小，后续clip ，clip大小，单位字节
	* \retval  设置结果
	*/
	virtual STDMETHODIMP AddClipSize(__int64 nFileID,ClipSizeLoss csClipSize) = 0;
	/*! \fn virtual STDMETHODIMP GetClipSize(__int64 nFileID, __int64 nClipIndex,ClipSizeLoss* csClipSize)
	* \brief 获得clip大小
	* \param nFileID [in]		文件ID
	* \param nClipIndex [in]	CLIP段ID
	* \param csClipSize [out]   clip开始时间,clip大小，单位字节
	* \retval  获得结果
	*/
	virtual STDMETHODIMP GetClipSize(__int64 nFileID, __int64 nClipIndex,ClipSizeLoss* csClipSize) = 0;


	/*! \fn virtual STDMETHODIMP_(__int64) GetFileClipCount(__int64 nFileID)
	* \brief 获得待处理文件CLIP数目
	* \param nFileID [in]		文件ID
	* \retval  获得结果
	*/
	virtual STDMETHODIMP_(__int64) GetFileClipCount(__int64 nFileID) = 0;


	/*! \fn virtual SetCallBack(MEDIALOSSCOPY_MULTI_CALLBACK pCallBack, void *pUserObj,CALL_BACK_TYPE cbType)
	* \brief 设置回调函数
	* \param pCallBack   [in]  回调函数
	* \param pUserObject [in]  接受回调的对象
	* \param cbType		 [in]  回调类型
	* \retval  返回状态 
	*/
	virtual STDMETHODIMP SetCallBack(MEDIALOSSCOPY_MULTI_CALLBACK pCallBack, void *pUserObj,CALL_BACK_TYPE cbType) = 0;

	/*! \fn virtual STDMETHODIMP Start(void) 
	* \brief   开始转换
	* \retval  返回状态 
	*/
	virtual STDMETHODIMP Start(void) = 0;
	/*! \fn virtual STDMETHODIMP Pause(void) 
	* \brief   转换暂停
	* \retval  返回状态 
	*/
	virtual STDMETHODIMP Pause(void) = 0;
	/*! \fn virtual STDMETHODIMP Resume(void) 
	* \brief   恢复转换
	* \retval  返回状态 
	*/
	virtual STDMETHODIMP Resume(void) = 0;
	/*! \fn virtual STDMETHODIMP Stop(void) 
	* \brief   停止转换
	* \retval  返回状态 
	*/
	virtual STDMETHODIMP Stop(void) = 0;
	/*! \fn virtual STDMETHODIMP SetFileLossCopyMode(FILE_LOSSCOPY_MODE nStyle, UINT dwLimit)
	* \brief 设置Split参数
	* \param nStyle [in]   文件转换类型
	* \param dwLimit [in]  文件转换限制[0:不调整到关键帧点；1:调整到关键点]
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetFileLossCopyMode(FILE_LOSSCOPY_MODE nStyle, UINT dwLimit) = 0;
	/*! \fn virtual STDMETHODIMP GetFileLossCopyMode(FILE_LOSSCOPY_MODE *pStyle, UINT *pLimit) = 0;
	* \brief 获得Split参数
	* \param nStyle [out]   文件转换类型
	* \param dwLimit [out]  文件转换限制[保留]
	* \retval  获得结果
	*/
	virtual STDMETHODIMP GetFileLossCopyMode(FILE_LOSSCOPY_MODE *pStyle, UINT *pLimit) = 0;
	/*! \fn virtual STDMETHODIMP_(__int64) GetTargetCount(__int64 nFileID) ; 
	* \brief 获得转换目标数目参数
	* \retval  获得结果
	*/
	virtual STDMETHODIMP_(__int64) GetTargetCount(__int64 nFileID) = 0;
	/*! \fn virtual STDMETHODIMP GetTargetFileName(__int64 nFileID,__int64 nIndex, BSTR * ppFileName) = 0;
	* \brief 获得转换目标文件名
	* \param nIndex [in]       目标索引值
	* \param ppFileName [out]  目标文件名
	* \retval  获得结果
	*/
	virtual STDMETHODIMP GetTargetFileName(__int64 nFileID,__int64 nIndex, BSTR * ppFileName) = 0;

	/*! \fn virtual STDMETHODIMP STDMETHODIMP_(DWORD) Wait(UINT uMilliSecondsTimeOut = 0xFFFFFFFF) 
	* \brief 等待转换结束,兼容WIN的模式，MAC可以不用调；
	* \param UINT uMilliSecondsTimeOut  等待时间
	* \retval  等待结果EWS_OK转换已结束 EWS_TIMEOUT等待超时，转换未结束
	*/
	virtual STDMETHODIMP_(DWORD) Wait(UINT uMilliSecondsTimeOut = 0xFFFFFFFF) = 0;

	/*! \fn virtual STDMETHODIMP_(int) CheckMergeInfo() = 0; 
	* \brief 获取 group数目；
	* \retval  返回状态 
	*/
	virtual STDMETHODIMP_(IMerge_LossCopy_Check_Info*) CheckMergeInfo(void) = 0;

	/*! \fn virtual STDMETHODIMP GetFileKeyFrameIndex(__int64 nFileID,KeyFrameIndexArr** keyFrameIndexArr); 
	* \brief 获取关键帧索引信息；
	* \param nFileID [in]       文件索引值
	* \retval  返回关键帧索引数组
	*/
	virtual STDMETHODIMP GetFileKeyFrameIndex(__int64 nFileID,KeyFrameIndexArr** keyFrameIndexArr) = 0;

	
	/*! \fn virtual STDMETHODIMP_(ILossCopyMediaInfo*) GetFileMediaInfo(__int64 nFileID); 
	* \brief 获取关键帧索引信息
	* \param nFileID [in]      文件ID
	* \param pFileMediaInfo [out]    文件信息 
	* \retval  返回关键帧索引数组
	*/
	virtual STDMETHODIMP_(ILossCopyMediaInfo*) GetFileMediaInfo(__int64 nFileID) = 0;


	/*! \fn virtual STDMETHODIMP SelectFileStream( __int64 nFileID ,__int64 uStreamIndex,E_AV_STREAM_TYPE uStreamType) 
	* \brief 选择文件 的进行无损流
	* \param nFileID [in]		文件ID
	* \param uStreamIndex [in]   流id
	* \param uStreamType [in]    流类型
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SelectFileStream( __int64 nFileID ,__int64 uStreamIndex,E_AV_STREAM_TYPE uStreamType) = 0;


	/*! \fn virtual STDMETHODIMP GetSelectStream( __int64 nFileID ,__int64 uStreamIndex,E_AV_STREAM_TYPE uStreamType,IMediaStream ** pMediaStream)
	* \brief 选择文件 的进行无损流
	* \param nFileID [in]		文件ID
	* \param uStreamIndex [in]   流id
	* \param uStreamType [in]    流类型
	* \param pMediaStream [out]  流
	* \retval  设置结果
	*/
	virtual STDMETHODIMP GetSelectStream( __int64 nFileID ,__int64 uStreamIndex,E_AV_STREAM_TYPE uStreamType,IMediaStreamEx ** pMediaStream) = 0;

	/*! \fn virtual STDMETHODIMP ComfirmDeMux(void)
	* \brief 调用all stream 后，确认Demux创建成功与否
	* \retval  设置结果
	*/

	virtual STDMETHODIMP ComfirmDeMux(void) = 0;
	
	/*! \fn virtual STDMETHODIMP ComfirmMux(void)
	* \brief start前，进行确认MUX创建成功与否
	* \retval  设置结果
	*/
	virtual STDMETHODIMP ComfirmMux(void) = 0;

	/*! \fn virtual STDMETHODIMP_(int) GetSeekCap(__int64 nFileID)
	* \brief 选择文件 的进行无损流
	* \param nFileID [in]		文件ID
	* \retval  设置结果 
	*/
	virtual STDMETHODIMP_(int) GetSeekCap(__int64 nFileID) =0;
	
	/*! \fn virtual STDMETHODIMP SetOutputFileExtentName(const BSTR ppExtentName)
	* \brief 设置文件后缀
	* \param ppExtentName [in]		文件后缀,.mp4,目前只有 合并模式生效，以后去掉
	* \retval  设置结果 
	*/
	
	virtual STDMETHODIMP SetOutputFileExtentName(const BSTR ppExtentName) = 0;


	/*! \fn virtual STDMETHODIMP ResetDemuxAndMux(void)
	* \brief 在转换完成后，调用该函数，保留目前已经进行相关参数设置，恢复转换前初始状态。
	* \param 
	* \retval  设置结果 
	*/
	virtual STDMETHODIMP ResetDemuxAndMux(void) = 0;

	/*! \fn 	virtual STDMETHODIMP ChangeClipPosInFile(__int64 nFileID,__int64  nClipIndexOne,__int64  nClipIndexTwo);
	* \brief 改变一个文件中clip段的转换顺序
	* \param nFileID [in]		文件ID
	* \param nClipIndexOne [in]	CLIP段ID
	* \param nClipIndexTwo [int] CLIP段ID
	* \retval  设置结果
	*/
	virtual STDMETHODIMP ChangeClipPosInFile(__int64 nFileID,__int64  nClipIndexOne,__int64  nClipIndexTwo)=0;


	/*! \fn 	virtual STDMETHODIMP ChangeFilePosInConvter(__int64 nFileIDOne,__int64 nFileIDTwo);
	* \brief 改变转换中的文件的转换顺序
	* \param nFileIDOne [__int64]		文件ID
	* \param nFileIDTwo [__int64]		文件ID
	* \retval  设置结果
	*/
	virtual STDMETHODIMP ChangeFilePosInConvter(__int64 nFileIDOne,__int64 nFileIDTwo)=0;

	
	/*! \fn 	virtual STDMETHODIMP MoveFilePosInConvter(__int64 nFileID,__int64 nPos) = 0;
	* \brief 改变转换中的文件的转换顺序,在当前位置前插入
	* \param nFileID [__int64]		文件ID
	* \param nPos  [__int64]		当前加入文件顺序，从0开始
	* \retval  设置结果
	*/
	virtual STDMETHODIMP MoveFilePosInConvter(__int64* nFileID,__int64 nNum,__int64 nPos) = 0;

		/*! \fn 	virtual STDMETHODIMP CancelGetKeyFrameIndex(void) = 0;
	* \brief 取消获取关键帧索引
	* \retval  设置结果
	*/
	virtual STDMETHODIMP CancelGetKeyFrameIndex(__int64 nFileID) = 0;




};

// {B4BFC2EE-D154-4139-9986-07A6BCC5D3F5}
//定义组件对象CLSID_CMediaLossCopyConverter
MIDL_DEFINE_GUID(CLSID, CLSID_CMediaLossCopyConverter,0xb4bfc2ee, 0xd154, 0x4139, 0x99, 0x86, 0x7, 0xa6, 0xbc, 0xc5, 0xd3, 0xf5);
EXTERN_C const CLSID CLSID_CMediaLossCopyConverter;
//

