#pragma once
#include <scombase.h>
#include "COMMacro.h"
#include "TextDef.h"
#include "DecodeParam.h"
#include "CodecDefine.h"
#include "CommonInterface.h"
#include <DecMgr.h>

typedef struct _ThumbnailPicture
{
	BITMAPINFO		BmpInfo;
	BYTE			*pBits;
	int				nTime;
}ThumbnailPicture;

#define SNPASHOT_MESSAGE_PICTURE			0x00000001 //lParam:ThumbnailPicturez指针
#define SNAPSHOT_MESSAGE_DESTROY_DECODER	0x00000002 //lParam:ISnapshotStream指针
//缩略图回调，异步方式
typedef void (__stdcall *ThumbnailCallback)(void *pUserData, WPARAM wParam, LPARAM lParam);

// {8ADE6A85-0567-4c69-AF10-3B6CC335A29F}
DEFINE_GUID(IID_ISnapshotStream, 
			0x8ade6a85, 0x567, 0x4c69, 0xaf, 0x10, 0x3b, 0x6c, 0xc3, 0x35, 0xa2, 0x9f);
EXTERN_C const IID IID_ISnapshotStream;
MACRO_MIDL_INTERFACE(ISnapshotStream, "8ADE6A85-0567-4c69-AF10-3B6CC335A29F")
: public IUnknown
{
	/*! \fn virtual STDMETHODIMP OpenFile(BSTR *pFileName)
	* \brief 打开文件
	* \param pFileName [in] 文件名
	* \retval 打开结果
	*/
	virtual STDMETHODIMP OpenFile(BSTR pFileName) = 0;

	/*! \fn virtual STDMETHODIMP Init(int nWidth, int nHeight, BSTR *pszTempPath)
	* \brief 初始化参数
	* \param nWidth [in] 缩略图宽度
	* \param nHeight [in] 缩略图高度
	* \param pszTempPath [in] 缓存目录
	* \retval 初始化结果
	*/
	virtual STDMETHODIMP Init(int nWidth, int nHeight, BSTR pszTempPath, void *pUserData, ThumbnailCallback fnCallback, IMediaInfo* pMediaInfo) = 0;

	/*! \fn virtual STDMETHODIMP GetThumbnails(__int64 *pTimestampList, int *pTimestampCount)
	* \brief 获取指定时间戳缩略图
	* \param pTimes [in] 时间戳列表
	* \param nTimeCount [in] 时间戳数量
	* \retval 获取结果
	*/
	virtual STDMETHODIMP GetThumbnails(int *pTimes, int nTimeCount, ThumbnailPicture *pPic, int nSearchRange, BOOL bGetFromeBuffer) = 0;

	virtual STDMETHODIMP UninitDecoder() = 0;

	virtual STDMETHODIMP_(BOOL) HasDecoder() = 0;

};

// {118A8D06-BF42-49e3-A6DF-F325700A23BE}
//定义组件对象DecMgr
MIDL_DEFINE_GUID(CLSID, CLSID_CSnapshotStream, 0x118a8d06, 0xbf42, 0x49e3, 0xa6, 0xdf, 0xf3, 0x25, 0x70, 0xa, 0x23, 0xbe);

EXTERN_C const CLSID CLSID_CSnapshotStream;