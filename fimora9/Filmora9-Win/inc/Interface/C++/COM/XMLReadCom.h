/*! \file XMLReadCom.h
* \brief	编码参数校验接口,
*/
#pragma once
#include <scombase.h>
#include "COMMacro.h"
//#include "CommonInterface.h"
#include "XMLRead.h"

/*!
* \class IXMLRead
* \brief 编码参数校验接口 
*/
// {C1FD2EEA-93C5-4828-A8F5-11C9BACB19FC}
DEFINE_GUID(IID_IXMLRead, 
		0xc1fd2eea, 0x93c5, 0x4828, 0xa8, 0xf5, 0x11, 0xc9, 0xba, 0xcb, 0x19, 0xfc);
EXTERN_C const IID IID_IXMLRead;
MACRO_MIDL_INTERFACE(IXMLRead,"C1FD2EEA-93C5-4828-A8F5-11C9BACB19FC")
:public IUnknown
{
	public:
		/*! \fn virtual STDMETHODIMP_(bool)initXml(BSTR ppName)
		* \brief 读取xml参数表
		* \param ppName [in] xml表存放路径
		* \retval 读取xml表是否成功
		*/
		virtual STDMETHODIMP_(bool)initXml(BSTR ppName)=0;
		/*! \fn virtual STDMETHODIMP_(int)unintXml()
		* \brief 卸载xml参数表
		* \retval 卸载xml表是否成功
		*/
		virtual STDMETHODIMP_(int)unintXml()=0;

		/*! \fn virtual STDMETHODIMP_(int)checkInputParam(userInputParam &userInputFileFourcc)
		* \brief 校验用户输入的参数 
		* \param userInputFileFourcc [in] 用户输入的音视频编码参数
		* \retval 校验结果
		*/
		virtual STDMETHODIMP_(int)checkInputParam(userInputParam &userInputFileFourcc)=0;

		/*! \fn virtual STDMETHODIMP_(int)checkInputParam(userInputParam &userInputFileFourcc)
		* \brief 校验用户输入的参数 
		* \param userInputFileFourcc [in] 用户输入的音视频编码参数
		* \param errResult [in/out] 错误返回值
		* \retval 校验结果
		*/
		virtual STDMETHODIMP_(int)checkInputParamEx(userInputParam &userInputFileFourcc,retErrCheckResult* errResult = NULL)=0;
};



/*!
* \class IXMLReadLodaDLL
* \brief 接口 
*/
// {03244199-C5F1-4dd1-AAF4-BD7704F63D71}
DEFINE_GUID(IID_IXMLReadLodaDLL, 
			0x3244199, 0xc5f1, 0x4dd1, 0xaa, 0xf4, 0xbd, 0x77, 0x04, 0xf6, 0x3d, 0x71);

EXTERN_C const IID IID_IXMLReadLodaDLL;
MACRO_MIDL_INTERFACE(IXMLReadLodaDLL,"03244199-C5F1-4dd1-AAF4-BD7704F63D71")
:public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP_(bool)initXml(BSTR ppName,WS_XML_PARSE_TYPE xmlParseType)
	* \brief 读取xml参数表
	* \param ppName [in] xml表存放路径
	* \retval 读取xml表是否成功
	*/
	virtual STDMETHODIMP_(bool)initXml(BSTR ppName,WS_XML_PARSE_TYPE xmlParseType = XML_PARSE_FILE_DEMUX)=0;

	/*! \fn virtual STDMETHODIMP_(int)unintXml_EX()
	* \brief 卸载xml参数表
	* \retval 卸载xml表是否成功
	*/
	virtual STDMETHODIMP_(int)unintXml_EX()=0;

	/*! \fn virtual STDMETHODIMP_(usrLoadFileDemuxDllParam *)GetFileDemuxDllNames(BSTR ppName,int &nDllNum)
	* \brief 校验用户输入的参数 
	* \param ppName [in] 待选择分离器的文件名 ,当ppname = null,获取所有分离器
	* \param nDllNum [in][out] 获取返回值所指向数组的大小；
	* \retval 校验结果
	*/
	virtual STDMETHODIMP_(const usrLoadFileDemuxDllParam *)GetFileDemuxDllNames(BSTR ppName,int &nDllNum,DECODE_DEMUX_MODE_XML_TYPE modeType)=0;

	/*! \fn virtual STDMETHODIMP_(usrLoadFileDecodeDllParam *)GetFileDecodeDllNames(DWORD dwDecodeFourcc,int &nDllNum)
	* \brief 校验用户输入的参数 
	* \param dwFourcc [in] 待选择解码器的的音视频fourcc
	* \param nDllNum [in][out] 获取返回值所指向数组的大小；
	* \retval 校验结果
	*/
	virtual STDMETHODIMP_(const usrLoadFileDecodeDllParam *)GetFileDecodeDllNames(DWORD dwDecodeFourcc,int &nDllNum,DECODE_DEMUX_MODE_XML_TYPE modeType)=0;

	/*! \fn virtual STDMETHODIMP_(usrLoadFileDecodeDllParam *)GetFileEncodeDllNames(DWORD dwEncodeFourcc)
	* \brief 校验用户输入的参数 
	* \param dwFourcc [in] 待选择编码器的的音视频fourcc
	* \param nDllNum [in][out] 获取返回值所指向数组的大小；
	* \retval 校验结果
	*/
	virtual STDMETHODIMP_(const usrLoadFileEncodeDllParam *)GetFileEncodeDllNames(DWORD dwEncodeFourcc,int &nDllNum)=0;

	/*! \fn virtual STDMETHODIMP_(usrLoadFileMuxDllParam *)GetFileMuxDllNames(DWORD dwMuxFourcc)
	* \brief 校验用户输入的参数 
	* \param dwFourcc [in] 待选择合成器的文件fourcc
	* \param nDllNum [in][out] 获取返回值所指向数组的大小；
	* \retval 校验结果
	*/
	virtual STDMETHODIMP_(const usrLoadFileMuxDllParam *)GetFileMuxDllNames(DWORD dwMuxFourcc,int &nDllNum)=0;

};

/*!
* \class IXMLReadLossParams
8 \brief 无损参数校验接口
*/

// {A20D87DA-D759-49ef-85F7-F5C3668259F5}
DEFINE_GUID(IID_IXMLReadLossParams, 
			0xa20d87da, 0xd759, 0x49ef, 0x85, 0xf7, 0xf5, 0xc3, 0x66, 0x82, 0x59, 0xf5);
EXTERN_C const IID IID_IXMLReadLossParams;

MACRO_MIDL_INTERFACE(IXMLReadLossParams,"A20D87DA-D759-49ef-85F7-F5C3668259F5")
:public IUnknown
{
	public:
	/*! \fn virtual STDMETHODIMP_(bool)initXml_Loss(BSTR ppName)
	* \brief 读取xml参数表
	* \param ppName [in] xml表存放路径
	* \retval 读取xml表是否成功
	*/
	virtual STDMETHODIMP_(bool)initXml_Loss(BSTR ppName)=0;

	/*! \fn virtual STDMETHODIMP_(int)unintXml_Loss()
	* \brief 卸载xml参数表
	* \retval 卸载xml表是否成功
	*/
	virtual STDMETHODIMP_(int)unintXml_Loss()=0;

	/*! virtual STDMETHODIMP_(int) GetMatchFormatParams();
	* \brief 获取无损匹配输出格式
	* \param dwFormatFourcc [in] 文件格式Foucc
	* \param dwVideoForcc   [in] 视频编码Foucc
	* \param dwFormatFourcc [in] 音频编码Foucc
	* \param nFormatNum		[in][out] 匹配格式数目
	* \retval 校验结果
	*/
	virtual STDMETHODIMP_(int) CheckMatchFormatParams(LossInputFormatInfo& nLossInputFormatInfo,LossOutputFormatInfo & nLossOutputFormatInfo) = 0;
};


// {540A7D1C-96D7-49b9-84FD-A9E51A56ADEA}

MIDL_DEFINE_GUID(CLSID,CLSID_CXMLRead, 0x540a7d1c, 0x96d7, 0x49b9, 0x84, 0xfd, 0xa9, 0xe5, 0x1a, 0x56, 0xad, 0xea);

//EXTERN_C const CLSID CLSID_CXMLRead;
