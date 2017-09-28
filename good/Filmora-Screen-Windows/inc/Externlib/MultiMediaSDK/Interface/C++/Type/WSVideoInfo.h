
/*! \file WSVideoInfo.h
* \brief							视频信息结构体
*/

#pragma once

/*!
* \def MEDIA_CAPABILITIES
* \brief 定义媒体seek以及时长信息
*/

// MEDIA_Capabilities
#ifndef  MEDIA_CAPABILITIES
	#define MEDIA_CAPABILITIES

	#define MCAPS_CANT_SEEK			0x0001	// 不支持Seek操作
	#define MCAPS_UNKNOWN_LENGTH	0x0002	// 媒体播放时长未知
#define    MCAPS_SUPPORT_MOV_QUICKTIME_SPRITE 0x04 //确定是否支持
#endif //  MEDIA_Capabilities

/*!
* \struct WSVideoInfo
* \brief 视频信息
*/

// 视频信息
struct WSVideoInfo
{
	/*! 宽度*/
	int width;

	/*! 高度 */
	int height;

	/*! 帧率 */
	double frame_rate;	

	/*! 总帧数 */
	int total_frames;

	/*! x方向比例 */
	int xratio;	

	/*! y方向比例 */
	int yratio;	

	/*! 图像格式 */
	DWORD dwCompression;

	/*! 图像位深度 */
	int nBitsDepth;			

	/*! 媒体特性，由MEDIA_CAPABILITIES各值或运算而成，见MEDIA_CAPABILITIES定义 */
	DWORD dwCapsFlag;		
};

//if (MCAPS_CANT_SEEK == (dwCapsFlag & MCAPS_CANT_SEEK))
//{
//	// 不支持Seek操作
//}

