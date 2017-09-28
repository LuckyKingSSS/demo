unit
	VideoInfo;
	
{$MINENUMSIZE 4}
    
interface

uses
    Windows;

// MEDIA_CAPABILITIES
const
 MCAPS_CANT_SEEK			= $0001;	// 不支持Seek操作
 MCAPS_UNKNOWN_LENGTH		= $0002;	// 媒体播放时长未知
 

// 视频信息
type
 WSVideoInfo = record
	width : Integer;			// 宽度
	height : Integer;			// 高度
	frame_rate : double;		// 帧率
	total_frames : Integer;		// 总帧数
	xratio : Integer;			// x方向比例
	yratio : Integer;			// y方向比例
	dwFourCC : DWORD;			// 图像格式
	nBitsDepth : Integer;		// 图像位深度

	dwCapsFlag : DWORD;			// 媒体特性，由MEDIA_CAPABILITIES各值或运算而成，见MEDIA_CAPABILITIES定义
 end;


type
	PWSVideoInfo	= ^WSVideoInfo;


implementation

end.


