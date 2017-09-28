#pragma once

#include <stdio.h>
#include <stdlib.h>
/*
#ifdef WIN32
#include <qt_windows.h>
#include <objbase.h>
#else
#include <QtGui/qwindowdefs.h>
#endif*/

#include <vector>
#include <iostream>
using namespace std;

#ifdef WIN32
#include <windows.h>
#endif

#include "strconv.h"
#include "processtype.h"
#include "encodeparam.h"
#include "encodeparamex.h"
#include "subpicstyledef.h"
#include "decodeparam.h"
#include "codecdefine.h"
#include "wsplayer.h"
#include "videoslideparam.h"
#include "audioslideparam.h"
#include "textdef.h"
#include "metadatainfo.h"
#include "convertertype.h"

#include "BaseThread.h"
#include "scombase.h"
#include "COMMacro.h"
#include "cfactory.h"
#include "decmgr.h"
#include "texttodib.h"
#include "mediaprocess.h"
#include "encmgr.h"
#include "mediaconverter.h"
#include "slidedecoder.h"
#include "commoninterface.h"
#include "checkencparams.h"
using namespace scombase;

/*#include "wscominterface.h"*/

struct FadeInOutTime
{
	double dFadeInStart;
	double dFadeInLength;
	double dFadeOutStart;
	double dFadeOutLength;
};

struct CWatermarkEffect
{
	BOOL		bHasEffect;
	DWORD		WaterMarkType;
#ifdef WIN32
	wchar_t		pfileName[512];
#else
	wchar_t		pfileName[260];
#endif
	TEXTITEM	TextItem;
	FRECT		frRect;
	RECT		realRect;
	int			nOpacity;
};

struct CVideoEffect
{
	bool				bHasVideoEffect;
	int					nBrightness;
	int					nContrast;
	int					nSaturation;
	int					nRotation;
	int					uFlipFlag;
	int					uFilterFlag;
	RECT				CropRect;
	DEINTERLACE_STYLE	DeinterlaceStyle;
	IMAGE_RESIZE_METHOD	ResizeStyle;
};

/*
STDAPI WSCreateInstance(
						REFCLSID	rclsid,
						LPUNKNOWN	outer,
						DWORD		context,
						REFIID		riid,
						LPVOID *	ppv
						);
STDAPI SetCOMProfile( const char * filename );*/

