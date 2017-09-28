#ifndef _H_INLEXML_H_
#define _H_INLEXML_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

//{81ECF6C7-BB04-C984-F1C1-A6769020C89E}
DEFINE_GUID(CLSID_NLEXml, 0x81ECF6C7, 0xBB04, 0xC984, 0xF1, 0xC1, 0xA6, 0x76, 0x90, 0x20, 0xC8, 0x9E);         

//{37320E9F-D138-1FFC-9EF6-6102F1D1573C}
DEFINE_GUID(IID_INLEXml, 0x37320E9F, 0xD138, 0x1FFC, 0x9E, 0xF6, 0x61, 0x02, 0xF1, 0xD1, 0x57, 0x3C); 

#define XML_BUFFER_SIZE 512
/**@ingroup GroupSDK
 * @brief xml读写底层接口
 *	内部使用Markup库实现，接口抽象规则如下:
 *	- xml只能有一个根节点(Element)
 *	- 一个节点有两个区域(Region):Attribute Region和Element Region
 *	- 在Attribute Region可设置属性，在Element Region才可增加节点或定位结点(MovetoNextElem)
 *	- 加入一个新节点(AddElem)或定位到一个节点(MovetoNextElem)，默认进入Attribute Region
 *	- 退出Element Region后即为Attribute Region
 */
class INLEXml :public IUnknown
{
public:
	
	/**
	 *@brief  	加载xml
	 *@param	wszFilePath 全路径名
	 *@return 	HRESULT __stdcall
	 */
	virtual HRESULT __stdcall Load(LPCWSTR wszFilePath) = 0;

	
	/**
	 *@brief  	保存xml
	 *@param	wszFilePath 全路径名
	 *@return 	标准返回	 
	 *@note		可完成SaveAs功能
	 */
	virtual HRESULT __stdcall Save(LPCWSTR wszFilePath) = 0;


	/**
	 *@brief  	设置二进制数据文件
	 *@param	wszBlobFile 路径名
	 *@param	bExisted 是否已存在
	 *@return 	标准返回
	 */
	virtual HRESULT __stdcall SetBlobFile(LPCWSTR wszBlobFile, bool bExisted) = 0;        //
	
	/**
	 *@brief 添加节点并默认进入到Attribute Region，此时可设置属性
	 */
	virtual HRESULT __stdcall AddElem(LPCWSTR wszName, LPCWSTR wszData = NULL) = 0;
	virtual HRESULT __stdcall AddElemInt(LPCWSTR wszName, int iValue) = 0;
	virtual HRESULT __stdcall AddElemInt64(LPCWSTR wszName, NLElonglong ullValue) = 0;
	virtual HRESULT __stdcall AddElemDouble(LPCWSTR wszName, double dValue) = 0;
	virtual HRESULT __stdcall AddElemData(LPCWSTR wszName, unsigned char* pBuf, int pBufSize) = 0;

	/**
	 *@brief 设置属性
	 */
	virtual HRESULT __stdcall SetAttrib(LPCWSTR wszAttrib, LPCWSTR wszValue) = 0;
	virtual HRESULT __stdcall SetAttribInt(LPCWSTR wszAttrib, int nValue) = 0;
	virtual HRESULT __stdcall SetAttribInt64(LPCWSTR wszAttrib, NLElonglong ullValue) = 0;
	virtual HRESULT __stdcall SetAttribDouble(LPCWSTR wszAttrib, double dValue) = 0;
	/**
	 *@brief 获取节点值
	 */
	virtual HRESULT __stdcall GetElem(LPCWSTR* pstr, wchar_t *pwszBuffer, unsigned int nBufferSize = XML_BUFFER_SIZE) = 0;
	virtual HRESULT	__stdcall GetElemAsInt32(int* piValue) = 0;
	virtual HRESULT __stdcall GetElemAsInt64(NLElonglong* pllValue) = 0;
	virtual HRESULT __stdcall GetElemAsDouble(double* pdValue) = 0;
	virtual HRESULT __stdcall GetElemDataSize(int* bufSize) = 0;
	virtual HRESULT __stdcall GetElemAsData(unsigned char* pBuf, int bufSize) = 0;
	/**
	 *@brief 获取属性值
	 */
	virtual HRESULT __stdcall GetAttrib(LPCWSTR wszAttrib, wchar_t *pwszBuffer, unsigned int nBufferSize = XML_BUFFER_SIZE) = 0;
	virtual HRESULT __stdcall GetAttribAsInt32(LPCWSTR wszAttrib, int* piValue) = 0;
	virtual HRESULT __stdcall GetAttribAsInt64(LPCWSTR wszAttrib, NLElonglong* pllValue) = 0;
	virtual HRESULT __stdcall GetAttribAsDouble(LPCWSTR wszAttrib, double* pdValue) = 0;

	/**
	 *@brief 定位
	 */
	virtual LPCWSTR __stdcall GetTagName() = 0;
	/**
	 *@brief 移到下一个节点，进入Attribute Region
	 */
	virtual HRESULT __stdcall MovetoNextElem(LPCWSTR wszName) = 0;
	/**
	 *@brief 进入Element Region，此后不能设置Attribite
	 */
	virtual HRESULT __stdcall EnterElemRegion() = 0;

	/**
	 *@brief 离开Element Region，进入Attribute Region
	 */
	virtual HRESULT __stdcall LeaveElemRegion() = 0;
	
	virtual HRESULT __stdcall ResetMainPos() = 0;

	virtual HRESULT __stdcall RemoveElem() = 0;
};                                                                              



#endif //_H_INLEXML_H_
