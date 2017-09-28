unit
	ConvTaskInfo; 

{$MINENUMSIZE 4}
    
interface

uses
    Windows, EncParam, CodecDefine, DecoderParam;

type
ConvertState =  // 转换状态
(
    CS_NONE,		//
    CS_RUNNING,		// 正在转换
    CS_COMPLETE,	// 成功转换
    CS_ERROR      	// 遭遇错误
);



// 扩展滤镜ID定义 
// EX_FILTER_ID
const 
 EX_FILTER_GRAY			= $0001;
 EX_FILTER_EMBOSS		= $0002;
 EX_FILTER_NEGATIVE		= $0004;
 EX_FILTER_BLACK_WHITE	= $0008;
 EX_FILTER_WHITE_BLACK	= $0010;
 EX_FILTER_OLD_PHOTO	= $0020;
	

// FLIP_FLAG
 FLIP_HORIZONTAL		= $0001;
 FLIP_VERTICAL			= $0002;

type

{$IFNDEF WATERMARK_PARAM}
{$DEFINE WATERMARK_PARAM}

  WatermarkParam = record
    hModule       : HMODULE;
    szID          : array [0..511] of WideChar;  // 水印资源ID
    Rect          : TRect;                       // 相对于剪裁后的图像分辨率中的位置
    nOpacity      : Integer;                     // 水印不透明度 (0 - 100)  0 - 完全透明  100 - 完全不透明
    lpbi          : PBITMAPINFO;                 // lpbi 非0则不使用hModule，支持32位透明图像，以及Y41A自定义格式
    lpBits        : PBYTE;                       // lpbi和lpBits需要应用程序负责其生命期
    nAlign		  : Integer;
  end;
  PWatermarkParam = ^WatermarkParam;
  
{$ENDIF}  //  WATERMARK_PARAM

// Effect设置

 EffectData = Record

    nVolume : Integer;	// 0 - 200
    nBrightness : Integer;	// -100～100
    nContrast : Integer; 	// -100～100
    nSaturation : Integer;	// -100～100
    uExFilterID : UINT; 	// 滤镜ID
    uFlipID : UINT; 	// 翻转ID
    uDeintelace : DEINTERLACE_STYLE;// 去除隔行扫描方式
end;



 TaskItem = record

     szInputFile: array [0..511] of WideChar;   //输入文件名

     VidDecParam : DecParam; 	//视频解码参数
     AudDecParam : DecParam; 	//音频解码参数
     Effect : EffectData;   	//特效参数
     crCrop : TRECT;         	//视频减切区域
     Watermark : WatermarkParam;	//水印
    // Slide Time 片断在时间线上的时间
     dStart : double;		// 相对输出（音）视频的起止时间，单位：秒，若 剪切长度 != 输出长度，则会产生拉伸效果
     dEnd : double;		// 此值为零则无拉伸效果 
				// Slide Time 为时间线上的时间

     dTrimStart : double;        	// 减切开始时间（文件绝对时间）
     dTrimEnd : double;        	// 减切结束时间（文件绝对时间）视频连接时必须填写正确的时间

     uDecMode : DEC_MODEL;

     EncParam : EncodeParam;
     szOutputFile : array [0..511] of WideChar;	// 输出文件名, 路径全名

     nConvState : ConvertState;
    
     dwUserData : DWORD;		// 用户数据
    
  end;
  PTaskItem = ^TaskItem;



implementation

end.


