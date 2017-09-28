{
	功能：	音频连接
			   音频合成
}

unit AudioSlideDecoder;

interface

uses
  Windows, DecoderParam, AudioSlideParams;


type
    HAUDIODECODER = pointer;
    PSHORT = ^SHORT;

// 创建SlideShow音频解码器
// HAUDIODECODER __stdcall SDAudioCreate(const AudioSlideParam* );
function SDAudioCreate(const pParam : PAudioSlideParam) : HAUDIODECODER; stdcall;

// 销毁SlideShow音频解码器
// void __stdcall SDAudioDestroy(HAUDIODECODER hAudio);
procedure SDAudioDestroy(hAudio : HAUDIODECODER); stdcall;

// 置入一个音频源及其处理参数，多次调用即可置入多个音频源，szSrcAudio为音频源文件路径
// BOOL __stdcall SDAddAudioSource(HAUDIODECODER hAudio, const wchar_t* szSrcAudioFile, const AudioProcessParam* pParam);
function SDAddAudioSource(hAudio : HAUDIODECODER; const szSrcAudioFile : PWideChar; const pParam : PAudioProcessParam) : BOOL; stdcall;

//double __stdcall SDSeek(HAUDIODECODER hAudio, double time); // 定位到指定时间
//function SDSeek(hAudio: HAUDIODECODER; time: double): double; stdcall;

//int __stdcall SDGetNextSamples(HAUDIODECODER hAudio, short * buffer, int n_samples); // 取得下n_samples个sample数据 n_samples应该为声道数量的整数倍
//function SDGetNextSamples(hAudio: HAUDIODECODER; buffer: PSHORT; n_samples: Integer): Integer; stdcall;

//double __stdcall SDGetOffset(HAUDIODECODER hAudio); // 返回当前时间
//function SDGetOffset(hAudio: HAUDIODECODER): double; stdcall;

//const AudioInfo* __stdcall SDGetAudioInfo(HAUDIODECODER hAudio); // 取得音频信息
//function SDGetAudioInfo(hAudio: HAUDIODECODER): PAudioInfo; stdcall;

implementation

const
    DLLNAME = 'WS_SlideDecoder.dll';

function  SDAudioCreate              ; external DLLNAME Name 'SDAudioCreate';
procedure SDAudioDestroy             ; external DLLNAME Name 'SDAudioDestroy';
function  SDAddAudioSource           ; external DLLNAME Name 'SDAddAudioSource';
//新的接口没有提供这些函数
//function  SDSeek                     ; external DLLNAME Name 'SDSeek';
//function  SDGetNextSamples           ; external DLLNAME Name 'SDGetNextSamples';
//function  SDGetOffset                ; external DLLNAME Name 'SDGetOffset';
//function  SDGetAudioInfo             ; external DLLNAME Name 'SDGetAudioInfo';

end.
