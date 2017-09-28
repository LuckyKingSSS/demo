#pragma once

//#ifdef _DEC_MODEL_DEFINE

//***********************************************************
// 以下是文件格式定义
//***********************************************************
#define MPEG_FORMAT_MPEG1			0
#define MPEG_FORMAT_VCD				1
#define MPEG_FORMAT_VCD_HIGH		101
//#define MPEG_FORMAT_VCD_NSR		2
#define MPEG_FORMAT_MPEG2			3
#define MPEG_FORMAT_SVCD			4
#define MPEG_FORMAT_SVCD_HIGH		104
//#define MPEG_FORMAT_SVCD_NSR		5
//#define MPEG_FORMAT_VCD_STILL		6
//#define MPEG_FORMAT_SVCD_STILL	7
#define MPEG_FORMAT_DVD_NAV			8
//#define MPEG_FORMAT_DVD			9
#define MPEG_FORMAT_ATSC480i		10
#define MPEG_FORMAT_ATSC480p		11
#define MPEG_FORMAT_ATSC720p		12
#define MPEG_FORMAT_ATSC1080i		13
#define MPEG_FORMAT_ATSC1080p		14

#define MPEG_FORMAT_FIRST		0
#define MPEG_FORMAT_LAST		MPEG_FORMAT_ATSC1080i
//#define MPEG_STILLS_FORMAT(x) ((x)==MPEG_FORMAT_VCD_STILL||(x)==MPEG_FORMAT_SVCD_STILL)
#define MPEG_ATSC_FORMAT(x) ((x)>=MPEG_FORMAT_ATSC480i && (x)<=MPEG_FORMAT_ATSC1080i)
#define MPEG_HDTV_FORMAT(x) MPEG_ATSC_FORMAT(x)
#define MPEG_SDTV_FORMAT(x) (!MPEG_HDTV_FORMAT(x))

// 以下是制式定义:0=component, 1=PAL, 2=NTSC, 3=SECAM, 4=MAC, 5=unspec
#define NORM_COMPONENT		0
#define NORM_PAL			1 // PAL 25帧
#define NORM_NTSC			2 // NTSC 29.97帧
#define NORM_SECAM			3 // 25帧
#define NORM_MAC			4
#define NORM_UNSPECIFIED	5

// 比例
#define ASPECT_SCREEN	1 //"1:1 pixels",
#define ASPECT_4x3		2 //"4:3 display",
#define ASPECT_16x9		3 //"16:9 display",
#define ASPECT_2x1		4 //"2.21:1 display"

// 质量定义
#define MPEG_QUALITY_LOW	0 // 质量差
#define MPEG_QUALITY_NORMAL	1 
#define MPEG_QUALITY_HIGH	2 // 质量好


//#endif // _DEC_MODEL_DEFINE