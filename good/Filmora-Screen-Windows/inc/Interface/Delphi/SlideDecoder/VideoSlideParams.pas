{
	   视频合成的处理参数
}

unit VideoSlideParams;

interface

uses
  Windows, DecoderParam, CodecDefine;

type

  WatermarkParam = record
    hModule       : HMODULE;
    szID          : array [0..511] of WideChar;  // 水印资源ID
    Rect          : TRect;                       // 相对于剪裁后的图像分辨率中的位置
    nOpacity      : Integer;                     // 水印不透明度 (0 - 100)  0 - 完全透明  100 - 完全不透明
    lpbi          : PBITMAPINFO;                 // lpbi 非0则不使用hModule，支持32位透明图像，以及Y41A自定义格式
    lpBits        : PBYTE;                       // lpbi和lpBits需要应用程序负责其生命期
 		nAlign		    : Integer;										 // 水印对齐方向，为0时取Rect坐标，取值1-9对应九宫位置	 
  end;
  PWatermarkParam = ^WatermarkParam;

  VideoSlideParam = record
    nWidth          : Integer;                // 分辨率宽度
    nHeight         : Integer;
    dFrameRate      : Double;                 // 帧率 0表示自动选择
    dwCompression   : DWORD;                  // 图像格式，目前只支持'21VY'
    nBitsDepth      : Integer;                // 图像位深度 YV12为12位
    Watermark       : WatermarkParam;         // 水印参数
  end;
  PVideoSlideParam = ^VideoSlideParam;


  VideoProcessParam	= record
    Time          : CLIP_TIME;	   // 时间设置
    CropRect      : TRecT;         // 剪裁区域，为浮点数相对坐标
    nBrightness   : Integer;       // 亮度，-100～100，0保持原亮度
    nContrast     : Integer;       // 对比度，-100～100，0保持原对比度
    nSaturation   : Integer;       // 饱和度,-100～100，0保持原饱和度
    uExFilterID   : UINT;          // 扩展滤镜ID
    uFlipFlag     : UINT;          // 图像翻转设置
    DecMod        : DEC_MODEL;     // 解码方式
    vidDecParam   : DecParam;		// 解码参数
	uDeintelace	  : DEINTERLACE_STYLE;		// 去除隔行扫描方式
    nResizeStyle    : IMAGE_RESIZE_METHOD;    // 图像缩放方式
    nInterpolation  : IMAGE_INTERPOLATION;    // 图像插值算法
  end;
  PVideoProcessParam = ^VideoProcessParam;


implementation

end.
        
