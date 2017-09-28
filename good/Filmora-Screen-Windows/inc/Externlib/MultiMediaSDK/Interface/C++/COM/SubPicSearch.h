/**
@file   SubPicSearch.h 
@brief  智能搜索外挂字幕文件

	能够识别的外挂字幕文件包括ass/ssa/srt，其搜索方式分三种：
	- 1>.搜索指定文件夹下的所有字幕，；
	- 2>.搜索视频文件所在文件夹下的，所有与视频文件同名的字幕；
	- 3>.搜索指定文件夹下的，所有与视频文件同名的字幕
*/

#include <scombase.h>
#include "COMMacro.h"


/** 
@class 	 ISubPic 
@brief	 字幕描述类,用于获取字幕的 文件名 及其 描述信息.
*/

// {426726DA-3746-42bc-8F78-FA17AC1300B9}
DEFINE_GUID(IID_ISubPic, 0x426726da, 0x3746, 0x42bc, 0x8f, 0x78, 0xfa, 0x17, 0xac, 0x13, 0x0, 0xb9);
EXTERN_C const IID IID_ISubPic;
MACRO_MIDL_INTERFACE(ISubPic, "426726DA-3746-42bc-8F78-FA17AC1300B9")
: public IUnknown
{
public:

	/**
	@brief			获取带路径的字幕文件名
	@param pName [out] :  保存带路径字幕文件名
	@return		HRESULT : 函数调用状态
				- 1.S_OK    : 调用成功;
				- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP GetName(BSTR *pName) = 0;

	/**
	@brief			获取字幕附加描述信息
	@param pDescription [out] : 保存附加描述信息
	@return		HRESULT : 函数调用状态
				- 1.S_OK    : 调用成功;
				- 2.S_FALSE : 调用失败;
	@note		该函数暂时为被使用,获取描述信息为NULL;
	*/
	virtual STDMETHODIMP GetDescription(BSTR *pDescription) = 0;
};


/** 
@class 	 ISubPics 
@brief	 字幕描述类集合,是ISubPic的集合表示抽象类.
*/


// {460A57D2-6369-451f-965C-01C7C4222BFE}
DEFINE_GUID(IID_ISubPics, 0x460a57d2, 0x6369, 0x451f, 0x96, 0x5c, 0x1, 0xc7, 0xc4, 0x22, 0x2b, 0xfe);
EXTERN_C const IID IID_ISubPics;
MACRO_MIDL_INTERFACE(ISubPics, "460A57D2-6369-451f-965C-01C7C4222BFE")
: public IUnknown
{
public:
	
	/**
	@brief			获取搜索到外挂字幕数.
	@return			int : 返回搜索到字幕数目.
	*/
	virtual STDMETHODIMP_(int) GetCount() = 0;

	/**
	@brief			获取指定索引的字幕描述类.
	@param nIndex [in] : 设置字幕索引
	@param ppItem [out] : ISubPic类型的指针的指针
	@return		HRESULT : 函数调用状态
				- 1.S_OK    : 调用成功;
				- 2.S_FALSE : 调用失败;
	*/
	virtual STDMETHODIMP GetItem(int nIndex, ISubPic** ppItem) = 0;	
};

/** 
@class 	 ISubPicSearch 
@brief	 字幕搜索类,设置不同的搜索方式,创建字幕描述集合.
*/

// {F100E8F6-63A7-4100-A4B1-5D0D013FD285}
DEFINE_GUID(IID_ISubPicSearch, 	0xf100e8f6, 0x63a7, 0x4100, 0xa4, 0xb1, 0x5d, 0xd, 0x1, 0x3f, 0xd2, 0x85);
EXTERN_C const IID IID_ISubPicSearch;
MACRO_MIDL_INTERFACE(ISubPicSearch, "F100E8F6-63A7-4100-A4B1-5D0D013FD285")
: public IUnknown
{
public:

	/**
	@brief				 	按不同搜索方式,搜索字幕
	@param pFolder	[in]: 	  设置字幕搜索文件夹
	@param pVideoPath [in] 	: 	 设置视频文件所在路径
	@return				 	ISubPics : 搜索字幕文件集合
	@note				 
			- 1.如果pFolder为NULL，pVideoPath非NULL，那么搜索VideoPath所在文件夹下，与视频文件名相同的外挂字幕名;
			- 2.如果pFolder非NULL， pVideoPath为NULL， 那么搜索该文件夹下所有的外挂字幕文件;
			- 3.如果pFolder为非NULL，pVideoPath非NULL，那么搜索pFolder下，与视频文件同名的外挂字幕文件;
			- 4.如果pFolder为NULL, pVideoPath为NULL， 非法。			
	*/
	virtual STDMETHODIMP_(ISubPics *) CreateSubPics(const BSTR pFolder, const BSTR pVideoPath) = 0;
};

// {5F44370F-215A-440e-A913-182826DA1944}
//定义组件对象subPicSearch
MIDL_DEFINE_GUID(CLSID, CLSID_CSubPicSearch, 0x5f44370f, 0x215a, 0x440e, 0xa9, 0x13, 0x18, 0x28, 0x26, 0xda, 0x19, 0x44);
EXTERN_C const CLSID CLSID_CSubPicSearch;
