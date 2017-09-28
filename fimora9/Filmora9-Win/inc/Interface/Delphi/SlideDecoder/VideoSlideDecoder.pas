{
	功能： 视频连接
}

unit VideoSlideDecoder;

interface

uses
  Windows, VideoSlideParams, DecoderParam;

type
    HVIDEODECODER = pointer;

// 创建SlideShow视频解码器
// HVIDEODECODER __stdcall SDVideoCreate(const VideoSlideParam* pParam);
function SDVideoCreate(const pParam : PVideoProcessParam) : HVIDEODECODER; stdcall;

// 销毁SlideShow视频解码器
// void __stdcall SDVideoDestroy(HVIDEODECODER hVideo);
procedure SDVideoDestroy(hVideo : HVIDEODECODER); stdcall;

// 置入一个视频频源及其处理参数（含剪裁、亮度对比度调节、特效设置等），多次调用即可置入多个视频源
// BOOL __stdcall SDAddVideoSource(HVIDEODECODER hVideo, const wchar_t* szSrcVideoFile, const VideoProcessParam* pParam);
function SDAddVideoSource(hVideo : HVIDEODECODER; const szSrcVideoFile : PWideChar; const pParam : PVideoProcessParam) : BOOL; stdcall;


implementation

const
    DLLNAME = 'WS_SlideDecoder.dll';

function  SDVideoCreate              ; external DLLNAME Name 'SDVideoCreate';
procedure SDVideoDestroy             ; external DLLNAME Name 'SDVideoDestroy';
function  SDAddVideoSource           ; external DLLNAME Name 'SDAddVideoSource';

end.


