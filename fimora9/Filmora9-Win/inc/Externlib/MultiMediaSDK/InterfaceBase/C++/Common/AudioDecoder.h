#pragma once

#include "WSAudioInfo.h"
#include "DecodeParam.h"
#include "CodecFrame.h"
#include "FileInfo.h"

// 音频解码器基类接口定义
class AudioDecoder
{
public:
	virtual ~AudioDecoder() {};
	virtual void Close() = 0; // 关闭解码器
	virtual BOOL Open(const wchar_t * pFileName, const DecParam* pDecParam, int* pError) = 0; // 打开媒体文件

	virtual double AudioSeek(double time) = 0; // 定位到指定时间
	//virtual int GetNextSamples(BYTE * buffer, int nSize) = 0; // 取得nSize字节的音频数据，返回字节数
	virtual const MEDIA_FRAME* GetNextFrame() = 0; // 取得下一个音频Frame
	virtual double GetOffset() const = 0; // 返回当前时间

	virtual const WSAudioInfo* GetAudioInfo() const = 0; // 取得音频信息
	virtual const StreamInfo* GetAudioStreamInfo() const = 0; // 取得流信息

	virtual void SetCreateType(int nType) {m_nCreateType = nType;};
	virtual int GetCreateType() {return m_nCreateType;};

private:
	int m_nCreateType;
};

