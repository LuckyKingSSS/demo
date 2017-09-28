
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

#define SPLIT_BASE_ON_NONE		0
#define SPLIT_BASE_ON_TIME		1
#define SPLIT_BASE_ON_SIZE		2


/*!
* \class IMediaEncParam
* \brief 编码参数接口 
*/

// {635AF5AC-3D40-4ca6-83B2-392E316345E0}
DEFINE_GUID(IID_IMediaEncParam, 
			0x635af5ac, 0x3d40, 0x4ca6, 0x83, 0xb2, 0x39, 0x2e, 0x31, 0x63, 0x45, 0xe0);
EXTERN_C const IID IID_IMediaEncParam;
MACRO_MIDL_INTERFACE(IMediaEncParam, "635AF5AC-3D40-4ca6-83B2-392E316345E0")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP SetFourCC(UINT dwFourCC)
	* \brief 设置编码Forcc
	* \param dwFourCC [in] 编码Forcc
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetFourCC(UINT dwFourCC) = 0;
	/*! \fn virtual STDMETHODIMP_(UINT) GetFourCC(void)
	* \brief  获得编码Forcc
	* \retval 获得结果
	*/
	virtual STDMETHODIMP_(UINT) GetFourCC(void) = 0;
	/*! \fn virtual STDMETHODIMP SetUseTwoPass(int bUse)
	* \brief 设置2pass标志
	* \param bUse [in] 2pass标志
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetUseTwoPass(int bUse) = 0;
	/*! \fn virtual STDMETHODIMP_(int) GetUseTwoPass(void)
	* \brief  获得2pass标志
	* \retval 获得结果
	*/
	virtual STDMETHODIMP_(int) GetUseTwoPass(void)=0;
	/*! \fn virtual STDMETHODIMP SetFileParam(const FileInfoParam * pFileParam)
	* \brief 设置文件信息参数
	* \param pFileParam [in] 文件参数
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetFileParam(const FileInfoParam * pFileParam) = 0;
	/*! \fn virtual STDMETHODIMP_(const FileInfoParam *) GetFileParam(void)
	* \brief 获得文件信息参数
	* \retval  FileInfoParam结构
	*/
	virtual STDMETHODIMP_(const FileInfoParam *) GetFileParam(void) = 0;
	/*! \fn virtual STDMETHODIMP SetEncParam(AV_STREAM_TYPE streamType, const StreamParam *pStreamParam)
	* \brief 设置编码参数
	* \param streamType [in]   设置的流类型  
	* \param pStreamParam [in] 设置的流参数
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetEncParam(AV_STREAM_TYPE streamType, const StreamParam *pStreamParam) = 0;
	/*! \fn virtual STDMETHODIMP_(const StreamParam *) GetEncParam(AV_STREAM_TYPE streamType)
	* \brief 获得编码参数
	* \param streamType [in] 流类型  
	* \retval  流参数
	*/
	virtual STDMETHODIMP_(const StreamParam *) GetEncParam(AV_STREAM_TYPE streamType) = 0;
	/*! \fn virtual STDMETHODIMP_(__int64) GetEstimateDiskSpace(void) 
	* \brief   估计转换需要的磁盘空间
	* \retval  估计转换需要的磁盘空间
	*/
	virtual STDMETHODIMP_(__int64) GetEstimateDiskSpace(void) = 0;
};

/*!
* \class IMediaConverter
* \brief converter接口 
*/

// {9684898A-3297-4da3-9264-6AAA5ED56600}
DEFINE_GUID(IID_IMediaConverter, 
			0x9684898a, 0x3297, 0x4da3, 0x92, 0x64, 0x6a, 0xaa, 0x5e, 0xd5, 0x66, 0x0);
EXTERN_C const IID IID_IMediaConverter;
MACRO_MIDL_INTERFACE(IMediaConverter, "9684898A-3297-4da3-9264-6AAA5ED56600")
: public IUnknown
{
	/*! \fn virtual STDMETHODIMP SetOutputFile(const BSTR pOutputPath)
	* \brief 设置输出文件
	* \param pOutputPath [in]   输出文件
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetOutputFile(const BSTR pOutputPath) = 0;
	/*! \fn virtual STDMETHODIMP GetEncodeParam(IMediaEncParam** pMediaEncParam)
	* \brief 得到编码参数接口
	* \param pMediaEncParam [out]  编码参数接口
	* \retval  获得结果
	*/
	virtual STDMETHODIMP GetEncodeParam(IMediaEncParam** pMediaEncParam) = 0;
	/*! \fn virtual STDMETHODIMP SetClipTime(double dBeginTime, double dLength) 
	* \brief 设置clip时间
	* \param dBeginTime [in]   clip开始时间
	* \param dLength [in]      clip长度
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetClipTime(double dBeginTime, double dLength) = 0;
	/*! \fn virtual STDMETHODIMP GetClipTime(double *pBeginTime, double *pLength)
	* \brief 获得clip时间
	* \param dBeginTime [out]   clip开始时间
	* \param dLength [out]      clip长度
	* \retval  获得结果
	*/
	virtual STDMETHODIMP GetClipTime(double *pBeginTime, double *pLength) = 0;
	/*! \fn virtual SetCallBack(MULTI_CALLBACK pCallBack, void *pUserObject)
	* \brief 设置回调函数
	* \param pCallBack   [in]  回调函数
	* \param pUserObject [in]  接受回调的对象
	* \retval  返回状态 
	*/
	virtual STDMETHODIMP SetCallBack(MULTI_CALLBACK pCallBack, void *pUserObj) = 0;
	/*! \fn virtual STDMETHODIMP SetMediainfoTime(double mediatime)
	* \brief   设置转换媒体时间长度
	* \param mediatime   [in]  媒体时间长度
	* \retval  无 
	*/
	virtual STDMETHODIMP SetMediainfoTime(double mediatime)=0;
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
	/*! \fn virtual STDMETHODIMP SetSplitParam(int nStyle, UINT dwLimit)
	* \brief 设置Split参数
	* \param nStyle [in]   Split类型
	* \param dwLimit [in]  Split限制
	* \retval  设置结果
	*/
	virtual STDMETHODIMP SetSplitParam(int nStyle, UINT dwLimit) = 0;
	/*! \fn virtual STDMETHODIMP GetSplitParam(int *pStyle, UINT *pLimit)
	* \brief 获得Split参数
	* \param nStyle [out]   Split类型
	* \param dwLimit [out]  Split限制
	* \retval  获得结果
	*/
	virtual STDMETHODIMP GetSplitParam(int *pStyle, UINT *pLimit) = 0;
	/*! \fn virtual STDMETHODIMP_(int) GetTargetCount(void) 
	* \brief 获得转换目标数目参数
	* \retval  获得结果
	*/
	virtual STDMETHODIMP_(int) GetTargetCount(void) = 0;
	/*! \fn virtual STDMETHODIMP GetTargetFileName(int nIndex, BSTR * ppFileName)
	* \brief 获得转换目标文件名
	* \param nIndex [in]       目标索引值
	* \param ppFileName [out]  目标文件名
	* \retval  获得结果
	*/
	virtual STDMETHODIMP GetTargetFileName(int nIndex, BSTR * ppFileName) = 0;
};


//定义组件对象MediaConverter
MIDL_DEFINE_GUID(CLSID, CLSID_CMediaConverter, 0x7e55d7f9, 0x454a, 0x4d4c, 0xbd, 0x2d, 0x8f, 0xec, 0x18, 0x7b, 0x65, 0x7f);

EXTERN_C const CLSID CLSID_CMediaConverter;

// {235FEC1A-B7E6-4fab-B89F-0743344E484A}
//定义组件对象CLSID_CLossLessCopy
MIDL_DEFINE_GUID(CLSID, CLSID_CLossLessCopy, 0x235fec1a, 0xb7e6, 0x4fab, 0xb8, 0x9f, 0x7, 0x43, 0x34, 0x4e, 0x48, 0x4a);

EXTERN_C const CLSID CLSID_CLossLessCopy;
