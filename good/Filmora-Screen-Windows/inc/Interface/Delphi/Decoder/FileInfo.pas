// FileInfo.pas
unit
	FileInfo;


{$MINENUMSIZE 4}

interface

uses
    windows, VideoInfo, AudioInfo, DecoderParam, CodecDefine;


type

{$IFNDEF STREAM_TYPE_DEF}
{$DEFINE STREAM_TYPE_DEF}

 STREAM_TYPE =
(
	ST_NONE,
	ST_NAV,
	ST_VIDEO,
	ST_AUDIO,
	ST_SUBPIC
);
{$ENDIF}  //  STREAM_TYPE_DEF


StreamInfo = record

	uStreamType : STREAM_TYPE;
	uStreamID : UINT;
	dwFourCC : DWORD;
	nState : Integer;	// 0 - 正常  -1 找不到dwFourCC对应的解码器
	szName	  	: array [0..63] of WideChar;
	szDescription	  	: array [0..127] of WideChar;
	dStreamLength : double;
	nBitrate : Integer;
	vidInfo : WSVideoInfo;	// 当uStreamType为ST_VIDEO时有效
	audInfo : WSAudioInfo;	// 当uStreamType为ST_AUDIO时有效
	ptsStart	: Int64;	// 当前流的第一个时间戳
	ptsOffset	: Int64;	// 当前流的第一个时间戳和该路节目所有流中ptsStart最小的那个ptsStart的差值

	dwReserved1 : DWORD;
	dwReserved2 : DWORD;
	
end;
  PStreamInfo = ^StreamInfo;
  
SUPPIC_TYPE =
(
	SPT_Internal,	//内嵌字幕
	SPT_External	//外置字幕
);

SubPicStreamInfo = record

	uSPType : SUPPIC_TYPE;
	uStreamID : UINT;
	dwFourCC : DWORD;
	szLang : array [0..63] of WideChar;
	szDescription : array [0..127] of WideChar;
end;
PSubPicStreamInfo = ^SubPicStreamInfo;

ProgramInfo = record

	dwNumber : DWORD;
	uPID : UINT;

	dMediaLength : double;
	nBitrate : Integer;

	nVideoStreamCount : Integer;
	nAudioStreamCount : Integer;

	pVidStreamInfos : PStreamInfo;
	pAudStreamInfos : PStreamInfo;
	
  uTimeStampReferenceStreamID : UINT;	// 时间戳参考音频流ID

  nSubPicStreamCount : Integer;
	pSubPicInfos : PSubPicStreamInfo;

end;
  PProgramInfo = ^ProgramInfo;


//MEDIA_INFO_RELEASE_API = procedure(pMediaFileInfo: PMEDIA_FILE_INFO); stdcall;

MEDIA_FILE_INFO = record

	dwFourCC : DWORD;
  szName	  	: array [0..63] of WideChar;
  szDescription	  	: array [0..127] of WideChar;

  dMediaLength : double;
	nBitrate : Integer;

	nProgramCount : Integer;
	pProgramInfos : PProgramInfo;

//	fnRelease : MEDIA_INFO_RELEASE_API;		// 释放MediaInfo数据内存
  fnRelease : Pointer;

end;
  PMEDIA_FILE_INFO = ^MEDIA_FILE_INFO;

          
implementation

end.
