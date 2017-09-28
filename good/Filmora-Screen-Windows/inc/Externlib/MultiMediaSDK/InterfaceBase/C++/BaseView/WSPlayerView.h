/*
 *  WSPlayerView.h
 *  PlayerView
 *
 *  Created by  wanggh on 5/13/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "Linux32_Typedef.h"
//#include <HIView.h>
#include "ViewCommon.h"
//#include "WSPlayer.h"

void* PVCreatePlayerView(void* parentView, VRECT* pRect);
void PVDestroyPlayerView(void* pPlayerView);

void PVSetCropMode(void* pPlayerView, BOOL bCropMode);
void PVSetMaskMode(void* pPlayerView, BOOL bMask);

void PVSetCallbackWnd(void* pPlayerView, void* delegate);

void PVSetCropRect(void* pPlayerView, LPRECT pCropRect);
void PVGetCropRect(void* pPlayerView, LPRECT pCropRect);

void PVSetDisplayResizeMode(void* pPlayerView, IMAGE_RESIZE_METHOD irm);
void PVSetDisplayScale(void* pPlayerView, int xratio, int yratio);

void PVUpdate(void* pPlayerView);

void* PVGetHwnd(void* pPlayerView);

void PVShowRGB32Image(void* pPlayerview, int width, int height, void* pBuff);
void PVShowRGB24Image(void* pPlayerview, int width, int height, int bytesPerRow, void* pBuff);


