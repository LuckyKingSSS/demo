/********************************************************
关于一般文字和艺术字的数据类型
*********************************************************/

/*! \file TextDef.h
* \brief	关于一般文字和艺术字的数据类型
*/

#ifndef __PCS_TEAM_TEXTDEF_20051208_CREAM520_0_H__
#define __PCS_TEAM_TEXTDEF_20051208_CREAM520_0_H__

#pragma once


#define TEXT_BUF_MAX_LEN			1024
#define EFFECT_NAME_BUF_LEN			64

#define TEXT_FONT_NAME_LEN			128
#ifdef WIN32

/*!
* \struct ARTTEXTINFO
* \brief 艺术字信息
*/
typedef struct tagARTTEXTINFO
{
	/*! 模块句柄*/
	HANDLE  hInst;

	/*! 艺术字描述 如 Hollow， Trape， Fashion， Rainbow*/
	wchar_t  szArtID[64];

	/*! 艺术字的ID*/
	UINT      uBmpID;

	/*! 保留字段*/
	DWORD     dwReserve;
}ARTTEXTINFO, *LPARTTEXTINFO;
#else
typedef void* LPARTTEXTINFO;
#endif

/*!
* \struct TEXTTRANSFORM
* \brief 位置信息
*/
typedef struct tagTEXTTRANSFORM
{
	/*! 左上角顶点坐标*/
	POINT ptLeftTop;

	/*! 左下角顶点坐标*/
	POINT ptLeftBottom;

	/*! 右上角顶点坐标*/
	POINT ptRightTop;

	/*! 右下角顶点坐标*/
	POINT ptRightBottom;
}TEXTTRANSFORM, * LPTEXTTRANSFORM;

#ifdef WIN32
// 艺术字参数结构定义
struct ARTTEXTPARAM
{
	wchar_t	strArtID[EFFECT_NAME_BUF_LEN];  // 艺术字类型ID
	int		nParam1;	
	int		nParam2;	
	int		nParam3;	
	int		nParam4;	
	int		nParam5;
	double	dParam6;	
	double	dParam7;	// 1-7号参数意义根据具体的艺术字类型定义
};

#endif

#ifndef WIN32
//rgb颜色定义，各项取值范围为[0, 255]
struct RGBCOLOR
{
	int Red;
	int Green;
	int Blue;
	int Opacity;
};

#define COLORREF RGBCOLOR
#endif

// 文字阴影结构定义
/*!
* \struct SHADOWPARAM
* \brief 文字阴影结构定义
*/
struct SHADOWPARAM
{
	/*! 阴影颜色*/
	COLORREF	crColor;	

	/*! 阴影X方向偏移*/
	int			nXOffset;

	/*! 阴影Y方向偏移*/
	int			nYOffset; 

#ifdef WIN32
	/*! 阴影不透明度*/
	int			nOpacity;	
#endif

	/*! 阴影模糊程度*/
	int			nBlurSize;	
};

// TEXT_STYLE
const int PCSFontStyleRegular		= 0;		// 常规
const int PCSFontStyleBold			= 0x0001;	// 粗体
const int PCSFontStyleItalic		= 0x0002;	// 斜体
const int PCSFontStyleUnderline		= 0x0004;	// 下划线
#ifdef WIN32
const int PCSFontStyleStrikeout		= 0x0008;	// 删除线
#endif

// 文本数据结构定义
/*!
* \struct TEXTITEM
* \brief 文本数据结构定
*/
struct TEXTITEM
{
	/*! 文本内容*/
	wchar_t		strText[TEXT_BUF_MAX_LEN];		

	/*! 字体名称*/
	wchar_t		strFontName[TEXT_FONT_NAME_LEN];

	/*! 字体颜色*/
	COLORREF	crFont;		

	/*! 字号*/
	int			nFontSize;	

	/*! TEXT_STYLE 由各style或运算而成*/
	UINT		uStyle;	 

	/*! 是否使用阴影*/
	int		bShadow;

	/*! 文字阴影（bShadow为真时有效） 艺术字不使用该参数 */
	SHADOWPARAM	paramShadow;

#ifdef WIN32
	/*! 光晕值 0为无光晕效果*/
	int			nHalation;	

	/*! 光晕颜色值*/
	COLORREF	crHalation;	
#endif
	
	/*! 旋转角度 顺时针（0-360）*/
	int			nAngle;	
	
	/*! 用于文字缩放 暂未使用*/
	int			xScale;	

	/*! 用于文字缩放 暂未使用*/
	int			yScale;		 

	/*! 不能使用*/
	DWORD		dwReserved;		

	/*! 不能使用*/
	DWORD		dwReserved2;
};
typedef TEXTITEM* LPTEXTITEM;

#ifdef WIN32
// 艺术字数据结构定义

/*!
* \struct ARTTEXTITEM
* \brief 艺术字数据结构定义
* \see TEXTITEM ARTTEXTPARAM
*/
struct ARTTEXTITEM
{
	/*! 基本文字数据*/
	TEXTITEM		txtBase;

	/*! 艺术字参数*/
	ARTTEXTPARAM	artTxtParam;
};
typedef ARTTEXTITEM* LPARTTEXTITEM;
#else
typedef void* LPARTTEXTITEM;
#endif

#endif //__PCS_TEAM_TEXTDEF_20051208_CREAM520_0_H__

