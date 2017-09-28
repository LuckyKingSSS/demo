
#pragma once

#include "CodecFrame.h"

struct TimeData
{
    int hour;
    int minute;
    int second;
    int millisec;
};

/*************************************************************
进度消息通知 wParam: 0   正在处理  lParam: 0 - 100
100 编码完成  lParam : 0 正常完成
1 用户终止 
2 外部回掉终止 
3 遭遇错误 
*************************************************************/

// 编码时的预览使用回调函数
//typedef int (__stdcall * ENCODE_CALLBACK)(void* pUserObj, UINT uMsg, WPARAM wParam, LPARAM lParam);
// uMsg -    0 进度消息  wParam - 进度(0-100)  lParam - ProgressInfo*
//           1 转换完成  wParam - 0 正常完成  1 用户终止 2 外部回掉终止 3 遭遇错误
//         100 第二次进度开始 
//           101 进度消息  wParam - 进度(0-100)  lParam - 0
//         110 第二次进度结束 在第二次进度期间 不能调用WEPause和WEStop函数
// 返回FALSE 则停止转换

struct ProgressInfo
{
    double dTotalLength;		// 总长度
    double dCurrTime;			// 当前时间
    int	   nPercent;			// 百分比（0 - 100）
    int	   nPercentPercent;		// 万分比（0 - 10000）
    const MEDIA_FRAME* pImageFrame;	//图像数据

    TimeData	 tmPrepaid; // 当前任务已用时间(系统时间)
    TimeData	 tmRemain;  // 当前任务剩余时间(系统时间)
    double		 dTransedTime;//当前任务已转换片断时间
    double		 dTransRate;  //转换速率，单位：每秒转换的片断长度
};

enum FileInfoType
{
	ID3,
};

#ifndef FILE_PARAM
#define FILE_PARAM
	struct FileInfoParam
	{
		FileInfoType formateType;
		int  nBufSize;
		BYTE *pBuffer;     
	};
#endif


#ifndef STREAM_PARAM
#define  STREAM_PARAM
	struct StreamParam
	{
		UINT	dwFourCC;
		int      nBufSize;
		BYTE     *pBuffer;
	};
#endif

