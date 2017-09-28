#ifndef _H_NLEKEY_H_
#define _H_NLEKEY_H_


#if defined(WIN32)

#ifdef NLEKEY_EXPORTS
#define NLE_KEY_API __declspec( dllexport )
#else
#define NLE_KEY_API __declspec( dllimport )
#endif

#else //WIN32
#define NLE_KEY_API
#endif //WIN32


#include "NLEType.h"

#include <vector>

typedef const WCHAR * NLE_CHAR;

/**@ingroup GroupKey
 *@brief 属性空间，使用NLEKey::kxxx方式访问
 */
namespace NLEKey
{
	namespace Filter
	{
		extern NLE_CHAR NLE_KEY_API
			/**
			*@brief FilterID，WCHAR*类型(GUID),例如: L"{12E5BC38-3508-44b0-AD1A-D0B37D19289F}.HueShift"即为HueShift滤镜
			*	- HRESULT SetString(LPCWSTR keyName, LPCWSTR szValue) ;
			*	- HRESULT GetString(LPCWSTR keyName, LPCWSTR *pValue) ;
			*/
			kFilterID,
			kFilterFriendName,
			kFilterLibName,
			kFilterCount,
			kFilterInputFrame,	//INLEFrame interface
			kFilterInputFrame2,	//INLEFrame interface

			kFilterRegion, // 滤镜生效的区域。NLERectF。相对值。大多数情况下，这是不用设置的
			
			/**@brief 万花筒*/
			kFilterID_Kaleidoscope,

			kFilterID_WaterSuffer,

			kFileterID_HueShift,

			kFilterID_Mirror,

			kFilterID_FlipUp,

			kFilterID_Crop,

			kFilterID_Mosaic,

			kFilterID_LightRoom,

			kFilterPitchShiter_Semitones,

			kFilterID_CircleHighlight,

			kFilterID_Rings;

		namespace Audio
		{
			extern NLE_CHAR NLE_KEY_API
				kFilterID_VolumeAmplifier,
				kFilterID_FadeInOut,
				kFilterID_PitchShiter,
				kFilterID_Denoise,
                kFilterID_ChannelProcessor;
		}
		/**
		@brief Filter属性
		*/
		namespace Property
		{
			extern NLE_CHAR NLE_KEY_API

				kCount,

				kFriendName,

				kVariantName,

				kType,

				kValue_Min,

				kValue_Max,

				kValue_Default,

				kValue_Current,

				kParamsArray[10];
		}

		namespace Mosaic
		{
			extern NLE_CHAR NLE_KEY_API
				kPercent; // 百分比。INT。取值范围[0, 100]。
		}
	};
	
	namespace Repository
	{
		extern NLE_CHAR NLE_KEY_API
			kEffectEngineDir,
			kAudioPluginsDir,
			kMotionTemplatePath,
			kMontageTemplatePath,
			kFishEyeTemplatePath;
	};
	
	/**基本属性*/
	namespace Base
	{
        extern NLE_CHAR NLE_KEY_API

		///class id, GUID
		kCLSID,

		///IID, GUID
		kIID,

		///Timeline的层,[0, max],int 类型。相对于自身的父TimeLine来说的层级
		kLevel,

		///名字，每个对象都可以设置名字,LPCWSTR/LPWSTR*, SetString / GetString
		kName,

        /**
        *@brief 文件名，全路径， LPCWSTR/LPWSTR*, SetString / GetString
        */
        kStrFileFullPath,

        /**
        *@brief 资源定位符 LPCWSTR/LPWSTR*, SetString / GetString
        */
		kResUrl,
        /**
        *@brief 资源路径{可包括环境变量；资源路径与kStrFileFullPath不一定一致，比如，蒙太奇的资源路径是模板文件的路径} LPCWSTR/LPWSTR*, SetString / GetString
        */
		kResPath,

		///Reserved
		kReserved;
	};
	
    /**
    非线编底层日志文件名， LPCWSTR/LPWSTR*, SetString / GetString
    */
    extern NLE_CHAR NLE_KEY_API kLogFileName;
      
    namespace Codec
    {
        extern NLE_CHAR NLE_KEY_API
			/**
			@brief 文件FourCC，DWORD类型
			*/
			kFourCC,

            /**
            @brief 获取文件预估空间占用，NLElonglong类型
            */
            kEstimateDiskSpace,

            /**
            @brief 是否关闭视频编码，INT类型
            */
            kVideo_Disable,

            /**
            @brief 是否关闭音频编码，INT类型
            */
            kAudio_Disable;

		namespace Stream{
			extern NLE_CHAR NLE_KEY_API
				/**
				@brief 流类型，INT类型，定义详见 enum NLEMediaType
				*/
				kStream_Type,

				/**
				@brief 节目ID，INT类型
				*/
				kStream_ProgramID,

				/**
				@brief 与当前流相关联的流的ID，如鼠标流中会包含与鼠标流相关联的屏幕流的ID， INT类型
				*/
				kStream_RelatedStreamID;

		}


        /**
        @brief 视频编码参数
        */
        namespace Video{
            extern NLE_CHAR NLE_KEY_API			
				/**
				@brief 流ID，INT类型
				*/
				kID,

				/**
				@brief 视频编码器FourCC，DWORD类型
				*/
				kFourCC,

				/**
				@brief 解码器状态，INT类型，0 - 正常  -1 找不到dwFourCC对应的解码器
				*/
				kState,

				/**
				@brief 流名称，WCHAR* 类型
				*/
				kName,

				/**
				@brief 流描述信息，WCHAR* 类型
				*/
				kDescription,

				/**
				@brief 流长度，LONGLONG类型，单位是100ns
				*/
				kDuration,

				/**
				@brief 比特率，int类型，单位是bps
				*/
				kBitrate,

				/**
				@brief 视频编码输出宽度，INT类型
				*/
				kWidth,

				/**
				@brief 视频编码输出高度，INT类型
				*/
				kHeight,

				/**
				@brief 视频编码帧率，double类型
				*/
				kFrameRate,            
            
				/**
				@brief 视频总帧数，INT类型
				*/
				kTotalFrames,


				/**
				@brief 图像格式，DWORD类型
				*/
				kCompression,

				/**
				@brief 图像位深度,INT类型
				*/
				kBitsDepth,

				/**
				@brief 媒体特性，由MEDIA_CAPABILITIES各值或运算而成，见MEDIA_CAPABILITIES定义，DWORD类型
				*/
				kCapsFlag,

				/**
				@brief 视频编码是否使用动态编码，INT类型
				*/
				kIsVBR,

				/**
				@brief 视频编码输出最大码率，INT类型
				*/
				kMaxBitrate,

				/**
				@brief 视频调整大小选项，INT类型(0=fullscreen, 1=orinal scale, 2=16:9, 3=4:3, 4=LetterBox, 5=PanScan)
				*/
				kResizeMethod,

				/**
				@brief 视频去除扫描线选项，INT类型
				*/
				kInterpolation;
		}

        /**
        @brief 音频编码参数
        */
        namespace Audio{

			extern NLE_CHAR NLE_KEY_API 
				/**
				@brief 流ID，INT类型
				*/
				kID,

				/**
				@brief 音频编码器FourCC，DWORD类型
				*/
				kFourCC,

				/**
				@brief 解码器状态，INT类型，0 - 正常  -1 找不到dwFourCC对应的解码器
				*/
				kState,

				/**
				@brief 流名称，WCHAR* 类型
				*/
				kName,

				/**
				@brief 流描述信息，WCHAR* 类型
				*/
				kDescription,

				/**
				@brief 流长度，LONGLONG类型，单位是100ns
				*/
				kDuration,

				/**
				@brief 比特率，int类型，单位是bps
				*/
				kBitrate,

				/**
				@brief 音频编码采样率，INT类型
				*/
				kSampleRate,

				/**
				@brief 音频通道数，INT类型
				*/
				kChannels,

				/**
				@brief 音频是否动态编码，INT类型
				*/
				kIsVBR,

				/**
				@brief 音频编码最大码率，INT类型
				*/
				kMaxBitrate,

				/**
				@brief 音频每个采样的bit数，INT类型
				*/
				kBitsPerSample,

				/**
				@brief 每秒音频数据占用的字节数，INT类型
				*/
				kBytesPerSecond;
        }
        
        namespace AttachedFrame
        {
            extern NLE_CHAR NLE_KEY_API
                /**
                @brief 中间帧缓冲，DWORD类型
                */
                kBagFrame,
                kAttacherEnable;
        }
    };
      
    
	/**Render属性*/
	namespace Render
	{	
		extern LPCWCHAR NLE_KEY_API
			///渲染模式，NLEInt,取值如下:
			///- RenderMode_Direct: 见 RenderMode_Direct 定义处的注释
			///- RenderMode_Fill:  填充满Region，对画面进行Resize
			kRender_Mode,

			///渲染分辨率，单位：像素, NLESize
			kRender_Size,

			///渲染区域，使用百分比表示，范围[0, 1], NLERectF
			///若Timeline没有父结点，此值必须为{0,0,1,1}
			kRender_Region,

			///渲染的透明度，使用百分比表示，范围[0, 1]，DOUBLE
			kRender_Alpha,

            ///渲染的背景颜色
            kRender_BackgroundColor,

			///表示不进行覆盖检查。INT。1 - 不检查覆盖。一般适用于滤镜。
			///优先级高于kRender_FullCover{即设置该标识之后，kRender_FullCover标识无效}
			kRender_NoCover,
			///表示是否完全覆盖在自己之下的层。INT。1 - 表示完全覆盖
			///是一个动态属性，根据本层显示的区域、透明度进行设置。{渲染时，可以根据此标识减少不必要的渲染，以加快速度}
			kRender_FullCover,
			///表示该片段隐藏。INT。{1 - 不可见，方便编辑的时候进行调试}
			kRender_Hidden,
            ///表示该片段的视频隐藏。INT。{1 - 不可见，方便编辑的时候进行调试}
            kRender_Hidden_Video,
            ///表示该片段的音频隐藏。INT。{1 - 不可见，方便编辑的时候进行调试}
            kRender_Hidden_Audio,

			///渲染的掩码文件{与渲染区域配合使用}
			kRender_MaskFile, // 蒙版文件路径。std::wstring。蒙版文件路径，或者是以英文逗号分隔的点序列{示例："0.0,0.0,0.2,0.0,0.2,1.0,0.0,1.0"，为左边一个矩形}表示需显示的部分
            ///渲染的掩码区域；NLERectF
			kRender_MaskRegion,
			///场景是否描边，与kRender_MaskFile配合使用
			kRender_BorderWidth, // 描边粗细。INT。0 - 不描边；大于0 - 描边的粗细
			kRender_BorderColor, // 描边颜色。INT。RGB颜色值;

			///渲染的视频帧率。NLERational
			kRender_Framerate,

			///Timeline位置,INT类型,SetInt / GetInt
			kRender_Position,

			///渲染的视频帧数。Int
			kRender_TotalFrameCount,

			/// 依靠点(重心点), NLEPointF。默认值在(0, 0)，左上角。范围在[-0.5, 0.5]
			kRender_AnchorPointF,

			/// 缩放, NLESizeF
			kRender_ResizeScale,

			///旋转角度,左手坐标系。double
			kRender_RotationAngle,

			///超出底图范围的部分的绘制模式。INT。0 - 超出范围的不绘制，1 - 左右，2 - 上下，3 - 对角
			kRender_OORMode,

			///渲染的音频的采样率。 Int
			kRender_SampleRate,

			///渲染的音频的通道数。 Int 
			kRender_Channels,

			///渲染的音频的采样宽度。 Int
			kRender_BitsPerSample,

			///音频音调
			kRender_AudioPitch,

			///Render的起始和结束帧序号,NLERange
			kRender_RangFrameNumber,

			///播放速度，NLERational
			kRender_Scale,
			///播放速度使能，INT
			kRender_Scale_Enable,

			///倒放,INT,3:enable audio 1: enable video, 0:disable all
			kRender_BackplayEnable,

			///稳像，INT，1：enable, 0:disable
			kRender_VideoStabilizerEnable,
			kRender_VideoStabilizerLevel,

			///定帧，INT，1：enable, 0:disable
			kRender_VideoFreezeEnable,

			///重放次数，INT,范围 >= 1
			kRender_Replay,
			kRender_Replay_Enable,

			///INT, 1:保留音频  0:去除音频
			kRender_Keep_Audio,

			// 去鱼眼Profile
			kRender_FishEyeEnable,
			kRender_FishEyeFileName,
			kRender_FishEyeLevel,

			///去噪强度，目前仅限于音频，背景噪音消除强度，INT类型，取值如下:
			///- 0：不降噪
			///- 1：弱
			///- 2：中
			///- 3：强
			kRender_Denoise,

			///去雾, INT 类型，范围[  ]
			kRender_Defog,

			///并发,INT, 1: enable, 0:disable
			kRender_PrepareConcurrency,

			/**
			@brief 是否使用视频流,INT类型，1表示有视频流；0表示没有视频流
			*/
			kRender_VideoEnable,

			/**
			@brief 是否使用音频流,INT类型，1表示有音频流；0表示没有音频流
			*/
			kRender_AudioEnable,
        
        /**
         *@brief 是否启用缓存,INT类型，1:启用;0:不启用
         */
        kRender_CacheEnable,
        
        /**
         *@brief 是否启用裸流,INT类型，1:启用;0:不启用；默认不启用
         */
        kRender_RawEnabled;

	}
	/**Source属性*/
	namespace Source
	{
		extern LPCWCHAR NLE_KEY_API

			//流的个数，类型为INT
			kSource_StreamCount,

			//流信息，类型为Interface, key需要拼接为 kSourceStream_xx, 其中xx为流的序号，如0， 1， 2
			kSource_Stream,

			///渲染分辨率，单位：像素, NLESize
			kSource_Size,

			///源视频的总帧数
			kSource_TotalFrameCount,

			//源视频的总时长，以100ns为单位
			kSource_Duration,

			///渲染区域，使用百分比表示，范围[0, 1], NLERectF
			kSource_Region,

			///渲染的视频帧率。NLERational
			kSource_Framerate,

			///渲染的音频的采样率。 Int
			kSource_SampleRate,

			///渲染的音频的通道数。 Int 
			kSource_Channels,

			///渲染的音频的采样宽度。 Int
			kSource_BitsPerSample,

			/////渲染的音频的每秒字节数，单位：字节。 Int
			//kSource_BytesPerSecond,

			///Render的起始帧序号,NLERange
			kSource_RangeFrameNumber,

			/**
			@brief 是否有视频流,INT类型，1表示有视频流；0表示没有视频流
			*/
			kSource_HasVideoStream,

			/**
			@brief 是否有音频流，INT类型，1表示有音频流；0表示没有音频流
			*/
			kSource_HasAudioStream,

			/**
			@brief render的类型，参见enum NLEScreenRecordingStreamType
			*/
			kSource_ScreenRecordingStreamType,

			/**
			@brief 文件类型，INT类型，值详见enum NLEFileType
			*/
			kSource_FileType,
			
			/**
			@brief 录制的视频中是否有鼠标流，INT类型， 1表示有鼠标流； 0表示没有鼠标流
			*/
			kSource_ScreenRecordingHasMouseStream;

		extern NLE_KEY_API std::vector<LPCWCHAR>
			///{ kSource_Size, kSource_TotalFrameCount,kSource_Framerate }
			kSource_VideoKeys,
			///{ kSource_SampleRate,kSource_Channels, kSource_BitsPerSample}
			kSource_AudioKeys;
	}

	namespace SourceRender
	{
		extern NLE_CHAR NLE_KEY_API
			/**
			@brief 流序号，表示在解析出的流列表中的位置，INT类型，从0开始
			*/
			kSourceRender_CurrentVideoStreamIndex,

			/**
			@brief 流序号，表示在解析出的流列表中的位置，INT类型，从0开始
			*/
			kSourceRender_CurrentAudioStreamIndex;

	}
	namespace PreProcess
	{
		extern NLE_CHAR NLE_KEY_API
			kPreProcess_SourceGUID,
			kPreProcess_ShiftSpeed,
			/**
			@brief 设置音频去噪强度，double类型，范围为0 - 3.0
			*/
			kPreProcess_DenoiseStrength;
	}

	namespace Caption
	{
		extern NLE_CHAR NLE_KEY_API
			kCaption_DataLength,
			kCaption_Data;
	}
	namespace Motion
	{
		extern NLE_CHAR NLE_KEY_API
			///是否启用动画，INT类型，1表示启用，0表示不启用
			kMotion_Enable,
			///动画的资源定位符。格式：animName@filePath
			kMotion_URI,
			///动画的循环模式，INT类型。0 - none，1 - circle，2 - reverse。
			kMotion_Loop,
			kMotion_RangeTime; // 动画的时间范围。单位：毫秒。NLERange，表示起始和结束的时间点
	}

	namespace Transition
	{
		extern NLE_CHAR NLE_KEY_API
			///是否启用转场
			kTransition_Enable,
			///转场的效果ID。字符串。形式：0FCB240F-5328-4e85-869A-2D989F50D387
			kTransition_ID,
			kTransition_RangeTime, // 转场的时间范围。单位：毫秒。NLERange，表示起始和结束的时间点
			kTransitionID_Cube;
	}

	namespace Montage
	{
		extern NLE_CHAR NLE_KEY_API
			kMontage_SceneIdx, // 场景编号。INT。指定本身是蒙太奇的第几场景。取值[0, sceneCnt -1]。{场景参数}
			kMontage_SceneCnt; // 场景个数。INT。指定蒙太奇的包含多少个场景
	}

	namespace Test
	{
		extern NLE_CHAR NLE_KEY_API
			kTestInt,
			kTestInt64,
			kTestDouble,
			kTestSize,
			kTestSizeF,
			kTestPoint,
			kTestPointF,
			kTestRect,
			kTestRectF,
			kTestData,
			kTestRetional;
	}
    
    
    namespace ComSupport
    {
        extern NLE_CHAR NLE_KEY_API kDllName;
    }

	namespace PostProcess
	{
		
		extern NLE_CHAR NLE_KEY_API
			//INT类型，0，禁用， 1，启用
			kMix2Mono,
			/// 音频控制，Int 类型，范围[-100, 100]			
			kVolume,
			/// 左右声道平衡控制，Int 类型，范围[-100, 100]。0 - 不调整，-100 - 完全左声道，100 - 完全右声道
            kBalance,
			/// 淡出淡出的时间,Int类型，单位毫秒
			kAudioFadeInTime,
			kAudioFadeOutTime,
                        /**
			亮度值，INT类型，范围为 [-100, 100]
			*/
			kBrightness,

			/**
			饱和度值，INT类型，范围为 [-100, 100]
			*/
			kSaturation,

			/**
			色度值，INT类型，范围为 [-100, 100]
			*/
			kHue,

			/**
			对比度值，INT类型，范围为 [-100, 100]
			*/
			kContrast;
	}

	namespace XML
	{
		extern NLE_CHAR NLE_KEY_API
			ELEM_PROPERTIES,
			ATTRI_PROPERTIES_COUNT,
			ELEM_PROPERTIES_CHILD,
			ELEM_PROPERTIES_PROPERTIES_CHILD,
			ELEM_PROPERTIES_TIMELINE,

			ELEM_PROPERTIES_MONTAGE,
			ATTRI_PROP_KEY,
			ATTRI_PROP_TYPE,
			ATTRI_PROP_VALUE;
	}

	namespace Watermark
	{
		extern NLE_CHAR NLE_KEY_API
			/**
			水印文件路径，WCHAR* 类型
			*/
			kFilePath,
			/**
			水印目标区域，NLERectF类型，使用百分比表示，范围[0, 1]
			*/
			kRegion,
			/**
			水印透明度，INT 类型，取值[0, 255]
			*/
			kOpacity;
	}

	namespace Scene3D
	{
		extern NLE_CHAR NLE_KEY_API
			/*
			资源文件名，全路径{支持环境变量}
			*/
			kResourceFileName,
			/*
			.scene文件名，全路径{支持环境变量}
			*/
			kSceneFileName,
			/*
			纹理配置字符串
			*/
			kTextureString;
	}

	namespace Timelapse
	{
		extern NLE_CHAR NLE_KEY_API
			/*
			加载图像的文件名{不包含路径}的列表，字符串。是一个XML列表：
			<sequence count="3">
				<li>001.jpg</li>
				<li>002.jpg</li>
				<li>003.jpg</li>
			</sequence>
			列表里面的内容和INLEFactory::CreateTimelineFromDirectory传递的路径组合起来形成图像的全路径
			可选属性。如果设置了，则优先使用；不设置，则根据路径和kSequenceMode直接枚举目录下的相关图像文件
			*/
			kFileList,
			/*
			加载文件的模式，字符串。是一个正则表达式，用于匹配目录下的文件
			*/
			kSequenceMode,
			/*
			加载目录下的图像文件类型，字符串。举例：.jpg
			*/
			kImageType,
			/*
			序列的帧率。类型NLENational
			*/
			kFramerate,
			/*
			序列的尺寸。类型NLESize
			*/
			kSize,
			/*
			序列缓冲的帧数。INT
			*/
			kCache;

	}

	namespace MarkBook
	{
		namespace Render
		{
			extern NLE_CHAR NLE_KEY_API
				/**
				 *@brief Markdown属性是否起作用,INT 类型, 1 or 0
				 */
				 kRender_MarkdownsEnable;
		};
	};

	namespace Mouse
	{
		extern NLE_CHAR NLE_KEY_API			
			/**
			*@brief 点击鼠标左键时播放的声音文件路径，类型为LPCWSTR/LPWSTR*, SetString / GetString
			*/
			kMouse_SoundFilePath,

			/**
			*@brief 光标的透明度，值为0 - 1.0，double类型
			*/
			kMouse_Opacity,

			/**
			*@brief 光标图标的缩放比例，double类型, 如1.0表示100%， 0.2表示20%
			*/
			kMouse_Scale,

			/**
			*@brief 鼠标事件，用于指明响应哪种鼠标事件，类型为MouseEvent
			*/
			kMouse_Event,
			
			/**
			*@brief 鼠标点击时播放的声音的音量，Int 类型，范围[-100, 100]	
			*/
			kMouse_AudioVolume,
			
			/**
			*@brief 鼠标点击时播放的声音是否静音，INT类型，1表示静音，0表示正常播放
			*/
			kMouse_AudioMute;
	}

    namespace Keyboard
    {
        extern NLE_CHAR NLE_KEY_API
            /**
            *@brief 用来标识是否是Keyboard Timeline
            */
            kKeyboard_IsKeyBoard;
    }

    namespace Freeze {
        extern NLE_CHAR NLE_KEY_API
            /**
            *@brief 用于设定定帧的位置{内部使用}。INT。帧数
            */
            kFreeze_Position,
            /**
            *@brief 用于设定定帧参数{内部使用}。INT。帧数
            */
            kFreeze_Scale,
            /**
            *@brief 用于设定定帧使能。INT。1 - 开启
            * 开启定帧后，默认以当前的renderRng.Start作为定帧的位置、renderRng.Count()作为定帧的长度
            */
            kFreeze_Enabled;
    }
};


#endif //_H_NLEKEY_H_
 
