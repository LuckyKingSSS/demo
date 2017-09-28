unit YUVImage;

interface

type
//#ifndef _YUVIMAGE_DEF
//#define _YUVIMAGE_DEF

{$ifndef _YUVIMAGE_DEF}
   {$define _YUVIMAGE_DEF}
  // YUV图像格式定义
  YUV_IMAGE_FORMAT =
   (
      YIF_444,
      YIF_422,
      YIF_YV12   // 420
   );

// YUV图像数据结构定义
  _YUVImage = record
    format  :YUV_IMAGE_FORMAT;

    y       :pByte;     // Y数据指针
    u       :pByte;     // U数据指针
    v       :pByte;     // V数据指针
    a       :pByte;     // Alpha 通道（不透明度）

    width   :integer;   // 图像宽度
    height  :integer;   // 图像高度

    y_pitch :integer;   // Y数据每行所占字节数
    u_pitch :integer;   // U数据每行所占字节数
    v_pitch :integer;   // V数据每行所占字节数
  end;
  PYUVImage = ^_YUVImage;
{$endif}
implementation

end.
