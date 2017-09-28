/*****************************************************************
定义文字编辑和艺术字的接口
**********************************************************************/

#ifndef __PCS_TEXT_20051207_H__
#define __PCS_TEXT_20051207_H__

#include "TextDef.h"
#include "Image.h"

extern "C"
{

#ifdef WIN32
void __stdcall TextInit();

// 关闭GDI+
void __stdcall TextUninit();
#endif

//MAC下必须首先调用这个函数，里面做了一些初始化
int  __stdcall GetSupportFontCount();

int __stdcall GetSupportFontName(int nIndex, wchar_t * pFontName);

//int __stdcall SetNormalTransform(LPTEXTTRANSFORM lp);
//int __stdcall GenerateNormalText(HDIBIMAGE hImage, const LPTEXTITEM pItem, HDIBIMAGE hImageTexture = NULL);

HDIBIMAGE __stdcall GenerateNormalText(const LPTEXTITEM pItem, LPTEXTTRANSFORM lpTextTransform, HDIBIMAGE hImageTexture = NULL);

HDIBIMAGE __stdcall GenerateNormalTextEx(const LPTEXTITEM pItem, int width = 0, int height = 0, HDIBIMAGE hImageTexture = NULL);

#ifdef WIN32

int __stdcall GetArtTextCount();

int __stdcall GetArtTextInfo(int nIndex, LPARTTEXTINFO pInfo);

int __stdcall  GetIndexFromID(wchar_t * pArtID);

//int __stdcall SetArtTransform(LPTEXTTRANSFORM lp);
//int __stdcall GenerateArtText(HDIBIMAGE hImage, LPARTTEXTITEM pArtItem);

HDIBIMAGE __stdcall GenerateArtText(LPARTTEXTITEM pArtItem, LPTEXTTRANSFORM lpTextTransform); 

HDIBIMAGE __stdcall GenerateArtTextEx(LPARTTEXTITEM pArtItem, int width, int height);

#endif

}
#endif //__PCS_TEXT_20051207_H__
