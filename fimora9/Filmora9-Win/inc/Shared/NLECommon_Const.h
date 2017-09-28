#pragma once

#include <objbase.h>


#if defined(WIN32)

#ifdef NLECOMMON_EXPORTS
#define NLECOMMON_API extern __declspec( dllexport ) 
#else
#define NLECOMMON_API extern __declspec( dllimport )
#endif

#else //WIN32
#define NLECOMMON_API extern
#endif //WIN32

#include "NLEConfig.h"
#include "NLEType.h"
#include "NLEGraphics.h"


#define REGION_FULL NLERectF(0.0f, 0.0f, 1.0f, 1.0f)



namespace NLECommon
{
	namespace Const
	{
		///图片分辨率最大值:宽度
		NLECOMMON_API INT IMAGE_WIDTH_MAX;
		///图片分辨率最大值:高度
		NLECOMMON_API INT IMAGE_HEIGHT_MAX;
		///任意位置，表示帧是自由的,-1
		NLECOMMON_API NLEFrameIndex FRAMEINDEX_EVERYWHERE;
		///帧序号最小值
		NLECOMMON_API	NLEFrameIndex FRAMEINDEX_MIN;
		///帧序号最大值
		NLECOMMON_API	NLEFrameIndex FRAMEINDEX_MAX;

		NLECOMMON_API NLERange RANGE_INFINITY;

		///Timeline层次最小值
		NLECOMMON_API INT TIMELINE_LEVEL_MIN;

		///原点位置(0.0, 0.0)
		NLECOMMON_API NLEPointF POINTF_ORIGNAL;
		///中心位置(0.5,0.5)
		NLECOMMON_API NLEPointF POINTF_CENTER;
		///无缩放
		NLECOMMON_API NLESizeF NO_RESIZE_SCALE;
		///不平移
		NLECOMMON_API NLEPointF NO_TRANSLATE;
		///不透明
		NLECOMMON_API BYTE	NO_TRANSPARENT;
		///不旋转
		NLECOMMON_API float	NO_ROTATION;
		///铺满底图
		NLECOMMON_API NLERectF FULL_GRAPHICS_RENDER;
		///空渲染区域
		NLECOMMON_API NLERectF EMPTY_GRAPHICS_RENDER;

		NLECOMMON_API NLERational NORMAL_SPEED;
		
		NLECOMMON_API NLESize DEFAULT_RENDER_SIZE;

		NLECOMMON_API UINT TASKID_INVALID;

		namespace Color
		{			
			///透明黑
			NLECOMMON_API UINT BLACK_TRANSPARENT;
			///不透明黑
			NLECOMMON_API UINT BLACK_OPAQUE;

			///透明红
			NLECOMMON_API UINT RED_TRANSPARENT;
			///不透明红
			NLECOMMON_API UINT RED_OPAQUE;

			///透明绿
			NLECOMMON_API UINT GREEN_TRANSPARENT;
			///不透明绿
			NLECOMMON_API UINT GREEN_OPAQUE;

			///透明蓝
			NLECOMMON_API UINT BLUE_TRANSPARENT;
			///不透明蓝
			NLECOMMON_API UINT BLUE_OPAQUE;
		};
		///编辑音频信息
		namespace AudioEdit
		{
			///44100
			NLECOMMON_API INT Samplerate;
			///2
			NLECOMMON_API INT Channels;
			///16
			NLECOMMON_API INT BitsPerSample;
			///44100 * 4
			NLECOMMON_API INT BytesPerSecond;
		}
	};
};

