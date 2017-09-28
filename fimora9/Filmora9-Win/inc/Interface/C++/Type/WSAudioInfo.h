
/*! \file WSAudioInfo.h
* \brief							音频信息结构体
*/

#pragma once

/*!
* \def MEDIA_CAPABILITIES
* \brief 定义媒体seek以及时长信息
*/

// MEDIA_CAPABILITIES
#ifndef  MEDIA_CAPABILITIES
	#define MEDIA_CAPABILITIES

	#define MCAPS_CANT_SEEK			0x0001	// 不支持Seek操作
	#define MCAPS_UNKNOWN_LENGTH	0x0002	// 媒体播放时长未知
#endif //  MEDIA_CAPABILITIES

// 音频信息

/*!
* \struct WSAudioInfo
* \brief 音频信息
*/
struct WSAudioInfo
{
	/*! 采样率 */
	int sample_rate;

	/*! 声道数 */
	int channels;

	/*! 每个sample占用位数 */
	int bitspersample;	

	/*! 每秒字节数 */
	int bytespersecond;	

	/*! 时间长度 */
	double length;	

	/*! 媒体特性，由MEDIA_CAPABILITIES各值或运算而成，见MEDIA_CAPABILITIES定义 */
	DWORD dwCapsFlag;		
};

