//VideoDecoder.h

#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include "WSVideoInfo.h"
#include "DecodeParam.h"
#include "CodecFrame.h"
#include "FileInfo.h"

// DECODE_EX_PARAM_MESSAGE
// SetExDecodeParam函数使用的uMsg
#define DEM_DEINTERLACE				1000	// 隔行扫描插值算法： wParam - DEINTERLACE_STYLE
#define DEM_SET_STILLIAMGE_PARAM	1001	// 设置静态图像文件参数（Still Image） 
											// wParam - frame rate, pointer of double data
											// lParam - duration, pointer of double data(second)
#define DEM_GET_SRC_DECODER			1002	// 取得原始视频解码器


// 视频解码器基类接口定义
class VideoDecoder
{
public:
	virtual ~VideoDecoder() {};
	virtual void Close() = 0; // 关闭解码器
	virtual BOOL Open(const wchar_t * pFileName, const DecParam* pDecParam, int* pError) = 0; // 打开媒体文件
	
	virtual DWORD SetExDecodeParam(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	virtual const WSVideoInfo* GetVideoInfo() const = 0; // 取得视频信息
	//virtual LPBITMAPINFO GetFormat() const = 0; // 取得图像格式
	virtual const StreamInfo* GetVideoStreamInfo() const = 0; // 取得流信息

	virtual double VideoSeekTime(double dTime, BOOL accurate = TRUE) = 0; // 定位到指定时间 dTime - 秒
	virtual int VideoSeek(int frame, BOOL accurate = TRUE) = 0; // 定位到指定帧

	virtual const MEDIA_FRAME* GetFrame() const = 0; // 取得当前帧图像
	virtual int GetFrameOffset() const = 0; // 返回当前帧索引号
	virtual int Forward(int nFrameInc) = 0; // 前进nFrameInc帧 返回前进的帧数

	virtual void SetCreateType(int nType) {m_nCreateType = nType;};		//创建方式
	virtual int GetCreateType() {return m_nCreateType;};
	virtual int ModifyInfo(WPARAM wParam, LPARAM lParam) {return 1;};

private:
	int m_nCreateType;
};

__inline double FrameToTime(int frame, double frame_rate)
{
	return frame / frame_rate;
}

__inline int TimeToFrame(double time, double frame_rate)
{
	return (int)(time * frame_rate + 0.5);
}


