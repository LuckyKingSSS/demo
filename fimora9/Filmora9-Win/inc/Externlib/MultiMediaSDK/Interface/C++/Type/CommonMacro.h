#pragma once

#define CheckPointer(p,ret) {if((p)==NULL) return (ret);}

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef SAFE_ADDREF
#define SAFE_ADDREF(p) { if(p) { (p)->AddRef(); } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef SAFE_CLOSE
#define SAFE_CLOSE(p) { if(p) { fclose((p)); (p)=NULL; } }
#endif

#ifndef SAFE_COPY_WSTRING
#define  SAFE_COPY_WSTRING(pSrc, pDes)	\
	SAFE_DELETE_ARRAY(pSrc);		\
	if(pDes)	\
	{		\
	pSrc = new wchar_t[wcslen(pDes) + 10];	\
	wcscpy(pSrc, pDes);	\
	}
#endif

#ifndef SAFE_COPY_STRING
#define  SAFE_COPY_STRING(pDes, pSrc)	\
	SAFE_DELETE_ARRAY(pDes);		\
	if(pSrc)	\
	{		\
	pDes = new char[strlen(pSrc) + 10];	\
	strcpy(pDes, pSrc);	\
	}
#endif

#ifndef FAILED_BREAK
#define FAILED_BREAK(hr)	\
	if (FAILED(hr))		\
	break;
#endif

#ifndef SECTOR_SIZE
#define  SECTOR_SIZE 2048
#endif

#ifdef WIN32
#define DASH	'\\'
#define DASH_VIDEO_TS "\\VIDEO_TS"
#define DASH_VIDEO_TS2 "\\VIDEO_TS\\"
#define VIDEO_TS_EX "%s\\VIDEO_TS\\"
#define VIDEO_TS_VOB "\\VIDEO_TS\\VIDEO_TS.VOB"
#define VIDEO_TS_VOB_EX "\\VIDEO_TS\\VTS_%02d_%d.VOB"
#else
#define DASH	'/'
#define DASH_VIDEO_TS "/VIDEO_TS"
#define DASH_VIDEO_TS2 "/VIDEO_TS/"
#define VIDEO_TS_EX "%s/VIDEO_TS/"
#define VIDEO_TS_VOB "/VIDEO_TS/VIDEO_TS.VOB"
#define VIDEO_TS_VOB_EX "/VIDEO_TS/VTS_%02d_%d.VOB"
#endif
