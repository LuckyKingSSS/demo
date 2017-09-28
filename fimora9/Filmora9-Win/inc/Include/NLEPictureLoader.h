//
//  NLELoadImage.h
//  NLEPlatformPro
//
//  Created by admin on 16/6/1.
//  Copyright (c) 2016年 wondershare. All rights reserved.
//

#ifndef __NLEPlatformPro__NLEPICTURELOADER__
#define __NLEPlatformPro__NLEPICTURELOADER__

#include <stdio.h>
#include <stdint.h>
#include <vector>

typedef struct tagNLEARGBFrame
{
    int width;
    int height;
	int nBitDepth;
	int duration; //显示时长，单位为毫秒(ms)，0表示没有设置时长
    uint8_t *buf;
}NLEPictureFrame;


typedef struct tagNLEPicture
{
	int nCount;
	NLEPictureFrame **pFrameArray;
}NLEPicture;

/**
@brief 加载图片文件，图片文件中可能包含有多帧图像，如GIF, TIFF, ICO文件
@param [out] imageVec 返回图片文件包含的所有图像
@param [in] pFileName 文件路径，完整的全路径
@return >=0,success;
	    <0, failed.
*/
int NLELoadMultiGBRAImages(NLEPicture **ppPicture, const wchar_t* pFileName);

void NLEReleaseMultiBGRAImages(NLEPicture **ppPicture);

#endif /* defined(__NLEPlatformPro__NLEPICTURELOADER__) */
