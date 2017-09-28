// 功能： 实现媒体文件的创建及编码
unit
	EncMgr; 
    
interface

uses
    Windows, EncParam, AudioInfo, VideoInfo;

type
	HMEDIAFILE  = pointer;	// 媒体文件句柄定义
	
//BOOL __stdcall FCInitPlugins();		// 应用程序启动时调用
function FCInitPlugins() : BOOL; stdcall;

//void __stdcall FCUninitPlugins();	// 应用程序退出时调用
procedure FCUninitPlugins(); stdcall;

// FileFormatInfo*  __stdcall FCGetFileFormatInfos(); // 获取已加载支持的所有文件格式，需在FCInitPlugins()调用成功后才可调用
function FCGetFileFormatInfos() : PFILEFORMATINFO; stdcall;

// 创建媒体文件 目前pVidInfo->dwFourCC = '21VY' pVidInfo->nBitsDepth = 12的输入数据 
//HMEDIAFILE __stdcall FCCreateMediaFile(onst wchar_t* pFileName, const EncodeParam* pEncParam,
//				  const VideoInfo* pVidInfo, const AudioInfo* pAudInfo);
function FCCreateFile(const pFileName: PWideChar; const pEncParam: PENCODEPARAM; 
						const pVidInfo: PWSVideoInfo; const pAudInfo: PWSAudioInfo) : HMEDIAFILE; stdcall;

// 关闭媒体文件
//void __stdcall FCDestroyMediaFile(HMEDIAFILE hFileEnc, ENCODE_CALLBACK pCallBack = 0, void* pUserObj = 0);
procedure FCCloseMediaFile(hFileEnc : HMEDIAFILE; pCallBack : ENCODE_CALLBACK = nil; pUserObj : Pointer = nil); stdcall;


// 写入音频数据（无压缩PCM数据）
//int __stdcall FCWriteAudioData(HMEDIAFILE hFileEnc, BYTE* pData, int nLen);
//旧的错误的:function FCWriteAudioData(hFileEnc : HMEDIAFILE; nType : Integer; pData : PBYTE; nLen : Integer) : BOOL; stdcall;
//function FCWriteAudioData(hFileEnc : HMEDIAFILE; pData : PBYTE; nLen : Integer) : BOOL; stdcall;
// 接口已经修改

// 写入一帧图像数据 目前只支持YV12图像
//int __stdcall FCWriteVideoFrame(HMEDIAFILE hFileEnc, BYTE* pBits, int nLen = 0);
//旧的错误的:function FCWriteVideoFrame(hFileEnc : HMEDIAFILE; nType : Integer; pBits : PBYTE; nLen : Integer = 0) : BOOL; stdcall;
//function FCWriteVideoFrame(hFileEnc : HMEDIAFILE; pBits : PBYTE; nLen : Integer = 0) : BOOL; stdcall;


implementation

const
    DLLNAME = 'WS_EncMgr.dll';

function FCCreateFile						; external DLLNAME index 1;
procedure FCCloseMediaFile				; external DLLNAME index 2;

//function FCWriteAudioData					; external DLLNAME index 3;
//function FCWriteVideoFrame					; external DLLNAME index 4;

function FCInitPlugins					; external DLLNAME index 5;
procedure FCUninitPlugins					; external DLLNAME index 6;
function FCGetFileFormatInfos			; external DLLNAME index 7;

end.


