unit
	VideoSrc;

{$WARNINGS OFF}
    
interface

uses
    windows, VidInfo, AudInfo, DecoderParam;


type
    PVIDEODECODER  = pointer;	// Video解码器句柄定义
    PAUDIODECODER  = pointer;	// Video解码器句柄定义
    PSHORT         = ^SHORT;


// 取得视频信息 返回0成功 否则失败
//int __stdcall WGetVideoInfo(const wchar_t * pFileName, VideoInfo* pVInfo);
function WGetVideoInfo(const pFileName : PWideChar; pVInfo : PVideoInfo) : Integer; stdcall;

// 取得音频信息 返回0成功 否则失败
//int __stdcall WGetAudioInfo(const wchar_t * pFileName, AudioInfo* pAInfo);
function WGetAudioInfo(const pFileName : PWideChar; pAInfo : PAudioInfo) : Integer; stdcall;


// 返回PVIDEODECODER句柄 返回0为打开文件失败
//PVIDEODECODER __stdcall WVDCreate(const wchar_t * pFileName, const VideoDecParam* pVidParam = 0, int* pError = 0);
function WVDCreate(const pFileName : PWideChar; const pVidParam : PVideoDecParam = nil; pError : PInteger = nil) : PVIDEODECODER; stdcall;

// 销毁VideoDecoder对象句柄
//void __stdcall WVDDestroy(PVIDEODECODER pVidDecoder);	
procedure WVDDestroy(pVidDecoder : PVIDEODECODER); stdcall;


//const VideoInfo* __stdcall WVDGetVideoInfo(PVIDEODECODER pVidDecoder); // 取得视频信息
function WVDGetVideoInfo(pVidDecoder : PVIDEODECODER) : PVideoInfo; stdcall;

//LPBITMAPINFO __stdcall WVDGetFormat(PVIDEODECODER pVidDecoder); // 取得图像格式
function WVDGetFormat(pVidDecoder : PVIDEODECODER) : PBITMAPINFO; stdcall;

//double __stdcall WVDSeekTime(PVIDEODECODER pVidDecoder, double dTime, BOOL accurate = TRUE); // 定位到指定时间 dTime - 秒
function WVDSeekTime(pVidDecoder: PVIDEODECODER; dTime: Double; accurate : BOOL = TRUE): Double; stdcall;

//int __stdcall WVDSeek(PVIDEODECODER pVidDecoder, int frame, BOOL accurate = TRUE); // 定位到指定帧
function WVDSeek(pVidDecoder: PVIDEODECODER; frame: Integer; accurate : BOOL = TRUE): Integer; stdcall;

//LPBITMAPINFO __stdcall WVDGetFrame(PVIDEODECODER pVidDecoder); // 取得当前帧图像
function WVDGetFrame(pVidDecoder : PVIDEODECODER) : PBITMAPINFO; stdcall;

//int __stdcall WVDGetFrameOffset(PVIDEODECODER pVidDecoder); // 返回当前帧索引号
function WVDGetFrameOffset(pVidDecoder : PVIDEODECODER) : Integer; stdcall;

//int __stdcall WVDNext(PVIDEODECODER pVidDecoder); // 定位到下一帧
function WVDNext(pVidDecoder : PVIDEODECODER) : Integer; stdcall;



// 返回PVIDEODECODER句柄 返回0为打开文件失败
//PAUDIODECODER __stdcall WADCreate(const wchar_t * pFileName, const AudioDecParam* pAudParam = 0, int* pError = 0);
function WADCreate(const pFileName : PWideChar; const pAudParam : PAudioDecParam = ni; pError : PInteger = nill) : PAUDIODECODER; stdcall;

// 销毁VideoDecoder对象句柄
//void __stdcall WADDestroy(PAUDIODECODER pAudDecoder)
procedure WADDestroy(pAudDecoder : PAUDIODECODER); stdcall;


//double __stdcall WADSeek(PAUDIODECODER pAudDecoder, double time); // 定位到指定时间
function WADSeek(pAudDecoder : PAUDIODECODER; time : double) : double; stdcall;

//int __stdcall WADGetNextSamples(PAUDIODECODER pAudDecoder, short * buffer, int n_samples); // 取得下n_samples个sample数据 n_samples应该为声道数量的整数倍
function WADGetNextSamples(pAudDecoder : PAUDIODECODER; buffer : PSHORT; n_samples : Integer) : Integer; stdcall;

//double __stdcall WADGetOffset(PAUDIODECODER pAudDecoder); // 返回当前时间
function WADGetOffset(pAudDecoder : PAUDIODECODER) : double; stdcall;

//const AudioInfo* __stdcall WADGetAudioInfo(PAUDIODECODER pAudDecoder); // 取得音频信息
function WADGetAudioInfo(pAudDecoder : PAUDIODECODER) : PAudioInfo; stdcall;




implementation

const
    DLLNAME = 'WS_VideoSrc.dll';

function WVDCreate						; external DLLNAME Name 'WVDCreate';
procedure WVDDestroy					; external DLLNAME Name 'WVDDestroy';
																
function WADCreate						; external DLLNAME Name 'WADCreate';
procedure WADDestroy					; external DLLNAME Name 'WADDestroy';
																
function WGetVideoInfo					; external DLLNAME Name 'WGetVideoInfo';
function WGetAudioInfo					; external DLLNAME Name 'WGetAudioInfo';
																
																
function WVDGetVideoInfo				; external DLLNAME Name 'WVDGetVideoInfo'; 				
function WVDGetFormat					; external DLLNAME Name 'WVDGetFormat'; 			
function WVDSeekTime					; external DLLNAME Name 'WVDSeekTime'; 			
function WVDSeek						; external DLLNAME Name 'WVDSeek'; 			
function WVDGetFrame					; external DLLNAME Name 'WVDGetFrame'; 			
function WVDGetFrameOffset				; external DLLNAME Name 'WVDGetFrameOffset'; 			
function WVDNext						; external DLLNAME Name 'WVDNext'; 			
																
																
function WADSeek						; external DLLNAME Name 'WADSeek'; 			
function WADGetNextSamples				; external DLLNAME Name 'WADGetNextSamples'; 			
function WADGetOffset					; external DLLNAME Name 'WADGetOffset'; 			
function WADGetAudioInfo				; external DLLNAME Name 'WADGetAudioInfo'; 	
		

end.




