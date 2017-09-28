// 
// 外挂字幕解码器
//

#pragma once

#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

#include "PCSText.h"
#include "Image.h"
#include "CodecFrame.h"
#include "xTraceU.h"
#include "SubPicStyleDef.h"
#include "SingleLock.h"

typedef void* HSUBPIC;

#ifdef MAX_PATH
#undef MAX_PATH
#endif
#define MAX_PATH 1024


// 字幕显示类型
enum DisplayType
{
	DT_TIME,   // 根据播放时间显示
	DT_FRAME,  // 根据视频帧数显示
};

// 字幕信息节点，每个节点表示一个字幕文件
typedef struct tagSubPicInfoItem
{
	unsigned int   dwFourCC;              // 字幕文件类型FourCC
	UINT    uSubPicID;             // 字幕ID
	wchar_t szFileName[MAX_PATH];  // 字幕文件名
	wchar_t szLangType[64];        // 字幕语言类型
	wchar_t szDescription[128];
}SUBPICINFOITEM, *PSUBPICINFOITEM;

// 字幕对应的时间
typedef struct tagTime
{
	int nHour;
	int nMin;
	int nSec;
	int nMilliSec;
}TIME, *PTIME;

// 按时间显示
typedef struct tagTimeDisplay
{
	TIME StartTime;
	TIME EndTime;
}TIMEDISPLAY, *PTIMEDISPLAY;

// 按帧显示
typedef struct tagFrameDisplay
{
	int nStartFrame;
	int nEndFrame;
}FRAMEDISPLAY, PFRAMEDISPLAY;

// 字幕内容显示位置
typedef struct tagPos
{
	POINT LeftTop;
	POINT RightBottom;
}POS, *PPOS;

// 一行字幕可以分为多个片段，不同片段可以有不同的字体颜色和样式
typedef struct tagSegment
{
	TEXTITEM Text;
}SEGMENT, *PSEGMENT;

// 一个字幕节点中可能包含多行字幕内容
// 这个结构体表示一行字幕内容
typedef struct tagContent
{
	int             nIndex; // 字幕内容编号
	POS             Pos;    // 字幕内容显示位置
	vector<SEGMENT> Segments;
}CONTENT, *PCONTENT;

// 一个字幕节点
typedef struct tagSubPicItem
{
	UINT             uID;      // 该字幕节点在字幕文件中的索引
	DisplayType      DType;    // 字幕显示方式（按时间、按帧）
	union
	{
		TIMEDISPLAY  td;       // 按时间显示
		FRAMEDISPLAY fd;       // 按帧显示
	};
	vector<CONTENT>  Contents; // 字幕内容
}SUBPICITEM, *PSUBPICITEM;

#ifdef __cplusplus
extern "C"
{
#endif

// 返回视频文件同目录下的字幕信息节点数组
// dwFourCC为0时结束
// pszVideoName：视频文件名（含路径）
const SUBPICINFOITEM* __stdcall GetSubPicInfoItems(const wchar_t* pszVideoName);

// 销毁字幕信息节点数组
void __stdcall DestorySubPicInfoItems();


// 创建外挂字幕解码器
// pszFileName：外挂字幕文件名（含路径）
// nVideoWidth：视频宽
// nVideoHeight: 视频高
HSUBPIC __stdcall CreateSubPicDec(wchar_t* pszFileName, int nVideoWidth, int nVideoHeight);

//获取当前创建的字幕流相关信息：字幕名、字幕id 、字幕forcc等
const SUBPICINFOITEM* __stdcall GetSubPicStreamInfo(HSUBPIC pSubpic);

void __stdcall DestorySubPicDec(HSUBPIC hSubPic);

void __stdcall ResetSubPicDec(HSUBPIC hSubPic);

SubPicFrame* __stdcall SubPicGetNextFrame(HSUBPIC hSubPic);

// 更新当前的字幕图片（比如样式改变后）
SubPicFrame* __stdcall UpdateCurrentFrame(HSUBPIC hSubPic);

void __stdcall DestroySubPicFrame(HSUBPIC hSubPic, SubPicFrame* pFrame);

// 按帧来seek
// 返回当前字幕节点index, -1表示失败
int __stdcall SubPicSeek(HSUBPIC hSubPic, int nFrame, double dFrameRate);

// 按时间来seek（时间单位为秒)
// 返回当前字幕节点index, -1表示失败
int __stdcall SubPicSeekEx(HSUBPIC hSubPic, double dTime, double dFrameRate);

// 设置用户定义的字幕样式
void __stdcall SetUserSubPicStyle(HSUBPIC hSubPic, SUBPICSTYLE* pStyle);

////用户自己去选择字幕文件，不再采用同文件夹下去自动识别
//BOOL __stdcall SelectSubPic(const wchar_t* pszSubPicName);
//
////确定字幕加载策略,默认key = 1时,采用文件夹自动识别方式;kye =0采用用户选择字幕方式;
//void __stdcall SetSubPicUser(int key);


#ifdef __cplusplus
}
#endif