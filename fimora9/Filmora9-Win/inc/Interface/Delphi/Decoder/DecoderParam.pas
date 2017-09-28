unit DecoderParam;

{$MINENUMSIZE 4}

interface

uses
  Windows, CodecDefine;


// DECODE_EX_PARAM_MESSAGE
// SetExDecodeParam函数使用的uMsg
const DEM_DEINTERLACE = 			1000;	// 隔行扫描插值算法： wParam - DEINTERLACE_STYLE


{$IFNDEF STREAM_TYPE_DEF}
{$DEFINE STREAM_TYPE_DEF}
type
  DecParam = record
    uMediaType:       UINT;        // ' FWS'  ' BOV'  ' GPM'
    uProgramID:       UINT;        // uPID of ProgramInfo
    nTrack:      Integer;     //视频轨道编号
    nSubTrack:   Integer;     //字幕编号 （适用于Video解码器）
    bDirectCopy: BOOL;
    uDeintelace : DEINTERLACE_STYLE;	// 隔行扫描插值算法
    wParam:      WPARAM;
    lParam:      LPARAM;
	bUseThreadDecode : BOOL;	// 使用线程解码缓冲 播放器创建解码器时需要将该参数置FALSE才能做到实时Deintelace
	                        	// 多个CPU系统上打开线程可以提升CPU使用率  
  end;
 
  PDecParam = ^DecParam;
  
{$ENDIF}  //  STREAM_TYPE_DEF


implementation

end.
