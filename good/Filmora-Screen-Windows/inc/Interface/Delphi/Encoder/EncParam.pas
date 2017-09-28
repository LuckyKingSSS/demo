unit
	EncParam;

{$MINENUMSIZE 4}
    
interface

uses
    Windows, CodecDefine;


type
 // 视频参数
	VideoParam = record
		dwFourCC : DWORD;		// 视频编码器 FourCC ： '1VMW'/'2VMW'/'3VMW' /'DIVX'/'462H'
		nWidth : Integer;		// 分辨率宽度
		nHeight : Integer;		// 分辨率高度
		dFrameRate : double;	// 帧率 0表示自动选择
		nBitrate : Integer;		// 码率 bps （恒定码率、可变最小码率）
		bIsVBR : BOOL;			// 是否使用变码率
		nMaxBitrate : Integer;	// 最大码率
		nResizeStyle : IMAGE_RESIZE_METHOD; // 图像缩放方式
		nInterpolation : IMAGE_INTERPOLATION;	// 图像插值算法
		dwCompression : DWORD;				// 图像格式
		nBitsDepth : Integer;					// 图像位深度
		
		//=============== MPEG 编码器参数 ===========================//
		// 使用Mpeg编码请参考     mpeg_param.pas                     // 
		dwFormat   : DWORD;   // 编码格式                            // 
		dwNorm     : DWORD;     // 电视制式                          //  
		dwAspect   : DWORD;   // 比例                                // 
		bFieldEnc  : BOOL;  // 是否使用交错模式                      //  
		//=============== MPEG 编码器参数 ===========================//
		
	end;

// 音频参数
	AudioParam = record
		dwFourCC : DWORD;		// 音频编码器 FourCC ： '1AMW'/'2AMW'/'3AMW' /' 3PM'
		nSampleRate : Integer;	// 采样率 Hz
		nChannels : Integer;	// 声道数量
		nBitrate : Integer;		// 码率 bps （恒定码率、可变最小码率）
		bIsVBR : BOOL;			// 是否使用变码率
		nMaxBitrate : Integer;	// 最大码率
		nBitsPerSample : Integer;	// Number of bits per sample of mono data
	end;	

// 编码参数
	EncodeParam = record
		video : VideoParam;		  // 视频参数
		audio : AudioParam;		  // 音频参数
		bVideoDisable : BOOL;	  // 禁用视频
		bAudioDisable : BOOL;	  // 禁用音频
//    case Integer of
//      0: (format : EncoderFormat);	  // 文件格式
//      1: (dwFourCC: DWORD);           //视频编码器
    dwFourCC: DWORD;        // 文件格式FourCC：' 4PM'
	end;
	
	EncoderInfo = record
	    dwFourCC	  	: DWORD;
	    szEncName	  	: array [0..63] of widechar;
	    szDescription	: array [0..127] of widechar;
	end;
	
type 
    PENCODERINFO = ^EncoderInfo;
    PPENCODERINFO = ^PENCODERINFO;
	
	FileFormatInfo = record
	    dwFourCC	  	: DWORD;
	    szExtName	  	: array [0..9] of widechar;
	    szFormatName	: array [0..63] of widechar;
	    szDescription	: array [0..127] of widechar;
	    nAudioEncs		: Integer;
	    nVideoEncs		: Integer;
	    ppAudioEncInfos	: PPENCODERINFO;
	    ppVideoEncInfos	: PPENCODERINFO;
	end;

type
  PFILEFORMATINFO = ^FileFormatInfo;
	    
type
	PENCODEPARAM  = ^EncodeParam;


// 编码时的预览使用回调函数
//typedef BOOL (__stdcall * ENCODE_CALLBACK)(void* pUserObj, UINT uMsg, WPARAM wParam, LPARAM lParam);
// uMsg -    0 进度消息  wParam - 进度(0-100)  lParam - ProgressInfo*
//           1 转换完成  wParam - 0 正常完成  1 用户终止 2 外部回掉终止 3 遭遇错误
//         100 第二次进度开始 
//           101 进度消息  wParam - 进度(0-100)  lParam - 0
//         110 第二次进度结束 在第二次进度期间 不能调用WEPause和WEStop函数
// 返回FALSE 则停止转换
type
  ENCODE_CALLBACK = function(pUserObj: Pointer; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): BOOL; stdcall; 


implementation

end.






