unit
	AudioInfo;

{$MINENUMSIZE 4}
    
interface

uses
    Windows;

// MEDIA_CAPABILITIES
const
 MCAPS_CANT_SEEK			= $0001;	// 不支持Seek操作
 MCAPS_UNKNOWN_LENGTH		= $0002;	// 媒体播放时长未知


// 音频信息
type
 WSAudioInfo = record
	sample_rate : Integer;		// 采样率
	channels : Integer;			// 声道数
	bitspersample : Integer;	// 每个sample占用位数
	bytespersecond : Integer;	// 每秒字节数
	length : double;			// 总帧数时间长度

	dwCapsFlag : DWORD;			// 媒体特性，由MEDIA_CAPABILITIES各值或运算而成，见MEDIA_CAPABILITIES定义
 end;

type
	PWSAudioInfo	= ^WSAudioInfo;

implementation

end.


