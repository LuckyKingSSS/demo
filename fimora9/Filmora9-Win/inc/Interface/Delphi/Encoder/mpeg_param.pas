unit mpeg_param;

{$MINENUMSIZE 4}

interface

uses
  Windows;

{$IFNDEF _MPEG_PARAM_DEFINE}
{$DEFINE _MPEG_PARAM_DEFINE}


//***********************************************************
// 以下是文件格式定义
//***********************************************************

const

 MPEG_FORMAT_MPEG1			= 0;
 MPEG_FORMAT_VCD			= 1;
 MPEG_FORMAT_VCD_HIGH		= 101;
// MPEG_FORMAT_VCD_NSR		= 2;
 MPEG_FORMAT_MPEG2			= 3;
 MPEG_FORMAT_SVCD			= 4;
 MPEG_FORMAT_SVCD_HIGH		= 104;
// MPEG_FORMAT_SVCD_NSR		= 5;
// MPEG_FORMAT_VCD_STILL	= 6;
// MPEG_FORMAT_SVCD_STILL	= 7;
 MPEG_FORMAT_DVD_NAV		= 8;
// MPEG_FORMAT_DVD			= 9;
 MPEG_FORMAT_ATSC480i		= 10;
 MPEG_FORMAT_ATSC480p		= 11;
 MPEG_FORMAT_ATSC720p		= 12;
 MPEG_FORMAT_ATSC1080i		= 13;
 MPEG_FORMAT_ATSC1080p		= 14;
							
 MPEG_FORMAT_FIRST		= 0;
 MPEG_FORMAT_LAST		= MPEG_FORMAT_ATSC1080i;
// MPEG_STILLS_FORMAT(x) ((x)==MPEG_FORMAT_VCD_STILL||(x)==MPEG_FORMAT_SVCD_STILL)
// MPEG_ATSC_FORMAT(x) ((x)>=MPEG_FORMAT_ATSC480i && (x)<=MPEG_FORMAT_ATSC1080i)
// MPEG_HDTV_FORMAT(x) MPEG_ATSC_FORMAT(x)
// MPEG_SDTV_FORMAT(x) (!MPEG_HDTV_FORMAT(x))

// 以下是制式定义:0=component, 1=PAL, 2=NTSC, 3=SECAM, 4=MAC, 5=unspec
 NORM_COMPONENT		= 0;
 NORM_PAL			= 1; // PAL 25帧
 NORM_NTSC			= 2; // NTSC 29.97帧
 NORM_SECAM			= 3; // 25帧
 NORM_MAC			= 4;
 NORM_UNSPECIFIED	= 5;

// 比例
 ASPECT_SCREEN	= 1; //"1:1 pixels",
 ASPECT_4x3		= 2; //"4:3 display",
 ASPECT_16x9	= 3; //"16:9 display",
 ASPECT_2x1		= 4; //"2.21:1 display"

// 质量定义
 MPEG_QUALITY_LOW	 = 0; // 质量差
 MPEG_QUALITY_NORMAL = 1; //
 MPEG_QUALITY_HIGH	 = 2; // 质量好
						

{$ENDIF}  //  _MPEG_PARAM_DEFINE


implementation

end.
