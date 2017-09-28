unit
	TextDef;
    
interface

{$MINENUMSIZE 4}

uses
    Windows;


const TEXT_BUF_MAX_LEN			=	1024;
const EFFECT_NAME_BUF_LEN		=	64;


// 艺术字参数结构定义
type ARTTEXTPARAM = record

	strArtID : Array[0..63] of WideChar;		// 艺术字类型ID
	nParam1 : Integer;	
	nParam2 : Integer;	
	nParam3 : Integer;	
	nParam4 : Integer;	
	nParam5 : Integer;
	dParam6 : double;	
	dParam7 : double;	// 1-7号参数意义根据具体的艺术字类型定义
end;

// 文字阴影结构定义
type SHADOWPARAM = record

	crColor : COLORREF;	// 阴影颜色
	nXOffset : Integer;	// 阴影X方向偏移
	nYOffset : Integer;	// 阴影Y方向偏移
	nOpacity : Integer;	// 阴影不透明度
	nBlurSize : Integer;// 阴影模糊程度
end;

  LPSHADOWPARAM = ^SHADOWPARAM;


// TEXT_STYLE
const PCSFontStyleRegular		= 0;		// 常规
const PCSFontStyleBold			= $0001;	// 粗体
const PCSFontStyleItalic		= $0002;	// 斜体
const PCSFontStyleUnderline		= $0004;	// 下划线
const PCSFontStyleStrikeout		= $0008;	// 删除线


// 文本数据结构定义
type TEXTITEM = record

	strText : Array[0..1023] of WideChar;		// 文本内容
	strFontName : Array[0..127] of WideChar;	// 字体名称
	crFont : COLORREF;			// 字体颜色
	nFontSize : Integer;		// 字号
	uStyle : UINT;			// TEXT_STYLE 由各style与运算而成

	bShadow : BOOL;		// 是否使用阴影
	paramShadow : SHADOWPARAM;	// 文字阴影（bShadow为真时有效） 艺术字不使用该参数 
	nHalation : Integer;		// 光晕值 0为无光晕效果
	crHalation : COLORREF;		// 光晕颜色值
	nAngle : Integer;			// 旋转角度 顺时针（0-360）

	xScale : Integer;			// 用于文字缩放 暂未使用
	yScale : Integer;			// 用于文字缩放 暂未使用

	dwReserved : DWORD;		// 不能使用
	dwReserved2 : DWORD;	// 不能使用
end;

  LPTEXTITEM = ^TEXTITEM;

// 艺术字数据结构定义
type ARTTEXTITEM = record

	txtBase : TEXTITEM;		// 基本文字数据
	artTxtParam : ARTTEXTPARAM;	// 艺术字参数
end;

  LPARTTEXTITEM = ^ARTTEXTITEM;
 
implementation

end.


