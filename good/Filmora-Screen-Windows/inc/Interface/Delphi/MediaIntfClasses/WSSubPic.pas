unit WSSubPic;

interface
uses
  Windows, WSTextArt;
type
  // 用户自定义的字幕样式
  TSubPicStyle = record
    bUseDefaultStyle: BOOL;       // 是否使用默认样式
    szFontName: array[0..127] of WideChar;  // 字体名
    nFontSize: Longint;           // 字体大小
    uStyle: UINT;                 // 字体样式 PCSFontStyleRegular | PCSFontStyleBold | PCSFontStyleItalic | PCSFontStyleUnderline
    Color: COLORREF;              // 字体颜色
    bShadow: BOOL;                // 是否使用阴影
    paramShadow: TShadowParam;    // 文字阴影（bShadow为真时有效）
{$IFDEF WIN32}
    nHalation: Longint;           // 光晕值 0为无光晕效果
    crHalation: COLORREF;         // 光晕颜色值
{$ENDIF}
    nAngle: Longint;              // 旋转角度 顺时针（0-360）
    nLeftTopX: Longint;           // 字幕图片左上角在视频图片上的X坐标
    nLeftTopY: Longint;           // 字幕图片左上角在视频图片上的Y坐标
  end;

  PUserSubPicRec = ^TUserSubPicRec;
  TUserSubPicRec = record
    uSubPicID: UINT;              // 字幕ID
    Style: TSubPicStyle;          // 字幕样式
    nOpacity: Longint;            // 透明度 Add by LSG
  end;

  TUserSubPicEx = record
    pSubPicPath: PWideChar;        // 字幕名
    Style: TSubPicStyle;            // 字幕样式
  end;
  PUserSubPicEx = ^TUserSubPicEx;

implementation

end.
