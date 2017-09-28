/**
@file	SubPicStyleDef.h 
@brief	字幕样式通用结构体

定义常用的字幕样式结构体，以及字幕消息宏
*/
#pragma once 
#include "TextDef.h"

/**
@brief	用户自定义的字幕消息

	用于SetExDecodeParam(UINT uMsg, WPARAM wParam, LPARAM lParam) 的uMsg参数
	- wParam:为USERSUBPIC结构体的地址
	- lParam: 为不透明度
*/
#define USER_SUBPIC 8888  

/**
@brief	用户自选字幕消息

	用于SetExDecodeParam(UINT uMsg, WPARAM wParam, LPARAM lParam) 的uMsg参数
	- wParam: 为USERSUBPIC结构体的地址
	- lParam: 为不透明度
*/
#define USER_SUBPICEX 8889 


/**
@struct tagSubPicStyle
@brief 用户自定义的字幕样式
*/
typedef struct tagSubPicStyle
{
	int        bUseDefaultStyle;  ///< 是否使用默认样式
	wchar_t     szFontName[128];  ///<  字体名
	int         nFontSize;        ///<  字体大小
	UINT        uStyle;           ///<  字体样式 PCSFontStyleRegular | PCSFontStyleBold | PCSFontStyleItalic | PCSFontStyleUnderline
	COLORREF    Color;            ///<  字体颜色
	int        bShadow;           ///<  是否使用阴影
	SHADOWPARAM	paramShadow;	  ///<  文字阴影（bShadow为真时有效）
#ifdef WIN32
	int			nHalation;		  ///<  光晕值 0为无光晕效果
	COLORREF	crHalation;		  ///<  光晕颜色值
#endif

	int			nAngle;			  ///<  旋转角度 顺时针（0-360）
	int         nLeftTopX;        ///<  字幕图片左上角在视频图片上的X坐标
	int         nLeftTopY;        ///<  字幕图片左上角在视频图片上的Y坐标
}SUBPICSTYLE, *PSUBPICSTYLE;

/**
@struct tagUserSubPic
@brief 字幕ID关联的字幕样式
*/

typedef struct tagUserSubPic
{
	UINT         uSubPicID;        ///<  字幕ID
	SUBPICSTYLE  Style;            ///<  字幕样式
}USERSUBPIC, *PUSERSUBPIC;


/**
@struct tagUserSubPicEx
@brief 字幕关联的字幕样式
*/
typedef struct tagUserSubPicEx
{
	wchar_t     *pSubPicPath;        ///<   字幕名	
	SUBPICSTYLE  Style;            ///<  字幕样式
}USERSUBPICEX, *PUSERSUBPICEX;
