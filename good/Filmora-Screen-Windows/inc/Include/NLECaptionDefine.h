#include "NLEGraphics.h"

#ifndef _NLE_CAPTION_DEFINE_TYPE_
#define _NLE_CAPTION_DEFINE_TYPE_

#define CAPTION_STREAM_WIDTH 1280
#define CAPTION_STREAM_HEIGHT 960
#define CAPTION_RES_DEF_WIDTH 640
#define CAPTION_RES_DEF_HEIGHT 480

#define  KEY_DEFINE_VK_NONE 0
#define  UNICODE_CODE2CHAR_SIZE 4
#define	TEXT_MAX_LENGTH 1024
#ifdef WIN32

#define  KEY_DEFINE_VK_RETURN 13
#define  KEY_DEFINE_VK_LINEFEED 10
#define  KEY_DEFINE_VK_BACKSPACE 3 //delete front
#define  KEY_DEFINE_VK_LEFT 18
#define  KEY_DEFINE_VK_UP 19
#define  KEY_DEFINE_VK_RIGHT 20
#define  KEY_DEFINE_VK_DOWN 21
#define  KEY_DEFINE_VK_TAB 9
#define  KEY_DEFINE_VK_DELETE 7
#define  KEY_DEFINE_VK_SPACE 32
#define  KEY_DEFINE_VK_V 86
#else
#define  KEY_DEFINE_VK_RETURN   13      //Big Enter
#define  KEY_DEFINE_VK_LINEFEED 10
#define  KEY_DEFINE_VK_BACKSPACE 127  //delete front
#define  KEY_DEFINE_VK_LEFT    63234
#define  KEY_DEFINE_VK_UP      63232
#define  KEY_DEFINE_VK_RIGHT   63235
#define  KEY_DEFINE_VK_DOWN    63233
#define  KEY_DEFINE_VK_TAB  9
#define  KEY_DEFINE_VK_DELETE 63272//46
#define  KEY_DEFINE_VK_SPACE   49
//    #define  KEY_DEFINE_VK_RETURN  36
#endif


#define  NLE_DEFAULT_DRAG_WIDTH  1
#define  NLE_DERAULT_DRAGBOX_SIZE		4
#define  NLE_DERAULT_ROTATION_LENGTH		20
#define	DEFAULT_AUTOALIGNWIDTH 5

//typedef enum tagNLEShiftState
//{
//	NLE_SS_NONE_TYPE = 0, 
//	NLE_SS_SHIFT_TYPE = 1,
//	NLE_SS_ALT_TYPE = 2,
//	NLE_SS_CTRL_TYPE = 4,
//	NLE_SS_LEFT_TYPE = 8,
//	NLE_SS_RIGHT_TYPE = 16,
//	NLE_SS_MIDDLE_TYPE = 32,
//	NLE_SS_DOUBLE_TYPE = 64,
//}NLEShiftState;


#define NLE_SS_NONE_TYPE 0
#define NLE_SS_SHIFT_TYPE 1
#define NLE_SS_ALT_TYPE 2
#define NLE_SS_CTRL_TYPE 4
#define NLE_SS_LEFT_TYPE  8

typedef LONG NLEShiftState;

#define NLE_MB_LEFT_TYPE  0
#define NLE_MB_RIGHT_TYPE 1
#define NLE_MB_MIDDLE_TYPE 2


#define NLE_CAPSTREAM_WIDTH 1280
#define NLE_CAPSTREAM_HEIGHT 960

#define FACESIZE 256
#define DEFAULT_FRAMERATE 25
#define MAXPASTESIZE 1024

#define M_PI 3.14159265358979323846
#define POLYBASESIZE 600
#define POLYLINEBASEHEIGHT 10

#define  DEFAULT_DRAG_WIDTH  9

//XML DEFINE

#define NODE_CAPTIONRES_NLETITLEARRAY			"NLETitleArray"
#define NODE_CAPTIONRES_DURATION					"Duration"
#define NODE_CAPTIONRES_NLETITLEITEM			"NLETitleItem"
#define NODE_CAPTIONRES_NLEITEMTYPE				"NLEItemType"
#define NODE_TITLEITEM_TEXTPARAM					"TextParam"
#define NODE_TITLEITEM_IMAGEPARAM					"ImageParam"
#define NODE_TITLEITEM_VIDEOPARAM					"VideoParam"
#define NODE_TITLEITEM_SHAPEPARAM					"ShapeParam"
#define NODE_TITLEITEM_EFFECTPARAM				"EffectParam"
#define NODE_TITLEITEM_ANIMATIONPARAM			"AnimationParam"
#define NODE_TITLEITEM_TEXTNODE			"TextNode"


#define NODE_TITLEITEM_TEXTDATA						"TextData"
#define NODE_TITLEITEM_CHARDATA						"CharData"
#define NODE_TEXTPARAM_TITLE							"Title"
#define NODE_TEXTPARAM_ROWSPACE					"RowSpace"
#define NODE_TEXTPARAM_LINESPACE					"LineSpace"
#define NODE_TEXTPARAM_GROUPIDX					"GroupIDX"

#define NODE_TEXTPARAM_TEXTURE						"Texture"
#define NODE_TEXTPARAM_GRADIENT					"Gradient"

#define NODE_TEXTPARAM_ALIGN							"Align"
#define NODE_TEXTPARAM_RADIUS						"Radius"
#define NODE_TEXTPARAM_ANGLE							"Angle"

#define NODE_TEXTPARAM_FONT							"Font"
#define NODE_TEXTPARAM_FACE							"Face"
#define NODE_TEXTPARAM_SHADOW						"Shadow"
#define NODE_TEXTPARAM_BORDER						"Border"

//Font
#define NODE_TEXTPARAM_FONT_SIZE					"Size"
#define NODE_TEXTPARAM_FONT_BOLD					"Bold"
#define NODE_TEXTPARAM_FONT_ITALIC				"Italic" 
#define NODE_TEXTPARAM_FONT_FONTNAME			"FontName"
//Face
#define NODE_TEXTPARAM_FACE_ENABLE				"Enable"
#define NODE_TEXTPARAM_FACE_COLOR1				"Color1"
#define NODE_TEXTPARAM_FACE_COLOR2				"Color2"
#define NODE_TEXTPARAM_FACE_GRADTYPE			"GradType"
#define NODE_TEXTPARAM_FACE_ALPHA				"Alpha"
#define NODE_TEXTPARAM_FACE_BLURRADIUS		"BlurRadius"
#define NODE_TEXTPARAM_FACE_EFFECT				"Effect"

//Border
#define NODE_TEXTPARAM_BORDER_ENABLE				"Enable"
#define NODE_TEXTPARAM_BORDER_SIZE					"Size"
#define NODE_TEXTPARAM_BORDER_COLOR1			"Color1"
#define NODE_TEXTPARAM_BORDER_COLOR2			"Color2"
#define NODE_TEXTPARAM_BORDER_GRADTYPE			"GradType"
#define NODE_TEXTPARAM_BORDER_ALPHA				"Alpha"
#define NODE_TEXTPARAM_BORDER_BLURRADIUS		"BlurRadius"

//Shadow
#define NODE_TEXTPARAM_SHADOW_ENABLE			"Enable"
#define NODE_TEXTPARAM_SHADOW_COLOR				"Color"
#define NODE_TEXTPARAM_SHADOW_ALPHA				"Alpha"
#define NODE_TEXTPARAM_SHADOW_BLURRADIUS	"BlurRadius"
#define NODE_TEXTPARAM_SHADOW_DISTANCE		"Distance"
#define NODE_TEXTPARAM_SHADOW_DIRECTION		"Direction"


#define NODE_TEXTPARAM_TEXTURE_ENABLE						"Enable"
#define NODE_TEXTPARAM_TEXTURE_TEXTUREIMAGEPATH		"TextureImagePath"

#define NODE_TEXTPARAM_GRADIENT_ENABLE					"Enable"
#define NODE_TEXTPARAM_GRADIENT_COLOR1					"Color1"
#define NODE_TEXTPARAM_GRADIENT_COLOR2					"Color2"
#define NODE_TEXTPARAM_GRADIENT_GRADTYPE				"GradType"


#define NODE_EFFECTPARAM_ZORDER					"ZOrder"
#define NODE_EFFECTPARAM_IMAGEPOSX				"ImagePosX"
#define NODE_EFFECTPARAM_IMAGEPOSY				"ImagePosY"
#define NODE_EFFECTPARAM_EFFECTIN					"EffectIn"
#define NODE_EFFECTPARAM_EFFECTOUT				"EffectOut"
#define NODE_EFFECTPARAM_IMAGEVZOOM			"VZoom"
#define NODE_EFFECTPARAM_IMAGEHZOOM			"HZoom"
#define NODE_EFFECTPARAM_STARTTIME				"StartTime"
#define NODE_EFFECTPARAM_DURATION				"Duration"
#define NODE_EFFECTPARAM_GROUP						"Group"
#define NODE_EFFECTPARAM_HOLLOW					"Hollow"


#define NODE_TITLEITEM_MOTIONID									"MotionID"
#define NODE_TITLEITEM_TRANSITIONID							"TransitionID"
#define NODE_TITLEITEM_ANIMATION_CONFIG					"AnimationConfig"
#define NODE_TITLEITEM_ANIMATION_ANIMATIONTYPE		"AnimationType"
#define NODE_TITLEITEM_ANIMATION_INTERPOLATETYPE 	"InterpolateType"
#define NODE_TITLEITEM_ANIMATION_DURATIONTYPE		"DurationType"
#define NODE_TITLEITEM_ANIMATION_STARTVALUE			"StartValue"
#define NODE_TITLEITEM_ANIMATION_ENDVALUE				"EndValue"
#define NODE_TITLEITEM_ANIMATION_PROPERTY				"Property"


//IMAGE
#define NODE_IMAGEPARAM_IMAGEPATH							"ImagePath"
#define NODE_IMAGEPARAM_TEMPIMAGEPATH					"TempImagePath"
//VIDEO

#define NODE_VIDEOPARAM_VIDEOPATH							"VideoPath"
#define NODE_VIDEOPARAM_IMAGEPATH							"ImagePath"
#define NODE_VIDEOPARAM_VIDEOCANCHANGECOLOR		"CanChangeColor"
#define NODE_VIDEOPARAM_VIDEOCOLOR							"Color"
#define NODE_VIDEOPARAM_VIDEO_DURATION					"Duration"
#define NODE_VIDEOPARAM_VIDEO_PAUSETIME				"PauseTime"
#define NODE_VIDEOPARAM_VIDEOGRADIENTENABLE		"Enable"
#define NODE_VIDEOPARAM_VIDEOGRADIENTCOLOR1		"Color1"
#define NODE_VIDEOPARAM_VIDEOGRADIENTCOLOR2		"Color2"
#define NODE_VIDEOPARAM_GRADIENTANGLE					"GradientAngle"
//END XML DEFINE


//Shape
#define NODE_SHAPEPARAM_ID                         "ShapeID"
#define NODE_SHAPEPARAM_GEOMETRY_NODE              "ShapeGeometry"
#define NODE_SHAPEPARAM_GEOMETRY_WIDTH             "ShapeGeometryWidth"
#define NODE_SHAPEPARAM_GEOMETRY_HEIGHT            "ShapeGeometryHeight"
#define NODE_SHAPEPARAM_GEOMETRY_POINTCNT          "ShapeGeometryPointCnt"
#define NODE_SHAPEPARAM_GEOMETRY_ROTATE          "ShapeGeometryRotate"

#define NODE_SHAPEPARAM_COLOR_NODE                 "ShapeColor"

#define NODE_SHAPEPARAM_COLOR_ENABLE               "ColorEnable"
#define NODE_SHAPEPARAM_COLOR_TYPE                 "ColorType"

#define NODE_SHAPEPARAM_FACE_NODE             "ShapeFace"
#define NODE_SHAPEPARAM_BORDER_NODE           "ShapeBorder"
#define NODE_SHAPEPARAM_BORDER_WIDTH          "BorderWidth"
#define NODE_SHAPEPARAM_BORDER_BLUR			  "BorderBlur"


#define NODE_SHAPEPARAM_SINGLE_COLOR_NODE			"SingleColor"
#define NODE_SHAPEPARAM_GRADIENT_COLOR_NODE         "GradientColor"
#define NODE_SHAPEPARAM_GRADIENT_COLOR_Dir          "GradientDir"
#define NODE_SHAPEPARAM_GRADIENT_COLOR_START_NODE   "GradientColorStart"
#define NODE_SHAPEPARAM_GRADIENT_COLOR_END_NODE		"GradientColorEnd"


#define NODE_SHAPEPARAM_COLOR_A                     "A"
#define NODE_SHAPEPARAM_COLOR_R						"R"
#define NODE_SHAPEPARAM_COLOR_G						"G"
#define NODE_SHAPEPARAM_COLOR_B						"B"








typedef LONG NLEMouseButton;

typedef enum tagNLEMouseType
{
	NLE_MT_TL = 0,
	NLE_MT_TC = 1,
	NLE_MT_TR = 2,
	NLE_MT_RC = 3,
	NLE_MT_RB = 4,
	NLE_MT_BC = 5,
	NLE_MT_LB = 6,
	NLE_MT_LC = 7,
	NLE_MT_ROT = 8,
	NLE_MT_DEFAULT = 9,
	NLE_MT_SIZE_ALL = 10,
	NLE_MT_FRAME_SIZE_ALL = 11,
	NLE_MT_CTRL_POINT = 12,
	NLE_MT_TEXT_EDIT = 13,
}NLEMouseType;

typedef enum tagNLEDragBoxType
{
	NLE_DBT_TL = 0,
	NLE_DBT_TC = 1,
	NLE_DBT_TR = 2,
	NLE_DBT_RC = 3,
	NLE_DBT_RB = 4,
	NLE_DBT_BC = 5,
	NLE_DBT_LB = 6,
	NLE_DBT_LC = 7,
	NLE_DBT_ROTATION = 8,
}NLEDragBoxType;

typedef enum tagNLEObjAlignType
{
	NLE_OAT_LEFT = 0,
	NLE_OAT_RIGHT = 1,
	NLE_OAT_H_MID = 2,
	NLE_OAT_TOP = 3,
	NLE_OAT_BOTTOM = 4,
	NLE_OAT_V_MID = 5,
}NLEObjAlignType;

typedef enum tagNLECaptionObjType
{
	NLE_DOT_NONE = 0,
	NLE_DOT_TEXT = 1,
	NLE_DOT_IMAGE = 2,
	NLE_DOT_VIDEO = 3,
	NLE_DOT_SHAPE = 4,
	NLE_DOT_TIPTEXT = 5,
	NLE_DOT_SVGTEXT = 6,
}NLECaptionObjType;

enum UndoReDoType
{
	UR_MODIFY = 0,
	UR_ADD = 1,
	UR_DEL = 2,
};

enum ZOrderType
{
	ZT_POSITIVE_ORDER = 0,
	ZT_REVERSE_ORDER = 1,
};

typedef enum tagNLEAlignType
{
	NLE_AT_LEFT = 0,
	NLE_AT_RIGHT = 1,
	NLE_AT_CENTER = 2,
	NLE_AT_AVERAGE = 3,
	NLE_AT_OVERLAY = 4,
}NLEAlignType;

typedef enum tagObjProperty
{
	OP_NONE = 0,
	OP_FONT_SIZE = 1,
	OP_FONT_NAME = 2,
	OP_FONT_BOLD = 3,
	OP_FONT_ITALIC = 4,
	OP_FONT_ALIGN = 5,
	OP_FONT_CHARSPACE = 6,
	OP_FONT_LINESPACE = 7,
	OP_FONT_RADIUS = 8,
	OP_SHADOW_ALPHA = 10,
	OP_SHADOW_BLURRADIUS = 11,
	OP_SHADOW_DISTANCE = 12,
	OP_SHADOW_DIRECTION = 13,
	OP_BORDER_ALPHA = 20,
	OP_BORDER_GRADTYPE = 21,
	OP_BORDER_SIZE = 22,
	OP_BORDER_BLURRADIUS = 23,
	OP_FACE_ALPHA = 30,
	OP_FACE_GRADTYPE = 31,
	OP_FACE_BLURRADIUS = 32,
	OP_FACE_GRADIENTANGLE = 33,
	OP_FACE_FACEEFFECT = 34,
	OP_IMAGE_GRADTYPE = 40,
	OP_IMAGE_ALPHA = 41,
	OP_IMAGE_PENWIDTH = 42,
	OP_FILLTYPE = 50,
	OP_VIDEO_GRADIENTANGLE = 60,
	OP_TEXT_EDIT = 80,
	OP_OBJ_POSCHANGE = 90,
	OP_OBJ_MOTIONCHANGE = 100,
	OP_OBJ_ANGLE = 101,
	OP_OBJ_SCALE = 102,

	OP_SHAPE_FACEALPHA = 201,
	OP_SHAPE_BORDERALPHA = 202,
	OP_SHAPE_BORDERBLUR = 203,
	OP_SHAPE_BORDERSIZE = 204,

	OP_SHAPE_CTRLPOS = 301,
	OP_SVG_COLOR = 401,
}ObjProperty;



class INLECaptionBaseObj;

typedef struct tagNLECaptionObjData
{
	NLECaptionObjType ObjType;
	INLECaptionBaseObj* pObj;
	int		GroupID;
	UndoReDoType nType;

	tagNLECaptionObjData()
	{
		ObjType = NLE_DOT_NONE;
		pObj = NULL;
		GroupID = 0;
		nType = UR_MODIFY;
	}
}NLECaptionObjData;


typedef struct tagCaptionEffectParam
{
	float  nImageCentX;
	float  nImageCentY;
	int  nImageAngle;
	float  dRatio;
	float  dEffectIn;
	float  dEffectOut;
	int		AlphaType;
	float		m_HZoom;
	float		m_VZoom;

	tagCaptionEffectParam()
	{
		nImageCentX = 50.0;
		nImageCentY = 50.0;
		nImageAngle = 0;
		dRatio = 0;
		dEffectIn = 30.0;
		dEffectOut = 30.0;
		AlphaType = 0;
		m_HZoom = 1.0;
		m_VZoom = 1.0;
	}
}CaptionEffectParam;

struct NLEImageParam
{
	wchar_t			wszFilePath[MAX_PATH];
	NLEImageParam()
	{
		memset(wszFilePath, 0, sizeof(wchar_t)* MAX_PATH);
	}
};



enum NLEFaceFillType
{
	NLE_FFT_COLOR = 0,
	NLE_FFT_GRADIENT = 1,
	NLE_FFT_TEXTURE = 2,
};

struct NLEGradient
{
	int	IsEnable;
	DWORD	Color1;
	DWORD	Color2;
	int			nGradType;

	NLEGradient()
	{
		IsEnable = 0;
		Color1 = RGB(255, 255, 255);
		Color2 = RGB(255, 255, 255);
		nGradType = 0;
	}
};

struct NLEFont
{
	double		nSize;				// 字号(单位:像素)
	int		IsBold;				// 是否是粗体
	int		IsItalic;				// 是否是斜体
	float		nRotation;		//旋转角度
	wchar_t	wszName[FACESIZE];	// 字体名
	int		nVSize;				//垂直方向分量（用于动画）
	int	    nHSize;			//水平方向分量（用于动画）

	NLEFont()
	{
		nSize = 68.0;
		IsBold = 0;
		IsItalic = 0;
		nRotation = 0.0f;
		wcscpy_s(wszName, MAX_PATH, L"Arial");
		nVSize = 100;
		nHSize = 100;
	}

	bool operator == (const NLEFont& a) const
	{
		return ((a.IsBold == IsBold) &&
			(a.IsItalic == IsItalic) &&
			(a.nHSize == nHSize) &&
			(a.nSize == nSize) &&
			(a.nVSize == nVSize) &&
			(0 == wcscmp(a.wszName, wszName)));
	}
};

struct NLEShadow
{
	int			IsEnable;			// 1-Enable, 0-Disable
	int			Color;			// Start Color
	int			nAlpha;			// 0-255
	int			nBlurRadius;		// 0-10
	int			nDistance;		// 0-100
	int			nDirection;		// 0-7
	bool			GradedEnabed;//界面自定义部分

	NLEShadow()
	{
		IsEnable = 0;
		Color = RGB(0, 255, 0);
		nAlpha = 255;
		nBlurRadius = 3;
		nDistance = 3;
		nDirection = 3;
		GradedEnabed = false;
	}

	bool operator == (const NLEShadow& a) const
	{
		return ((a.IsEnable == IsEnable) &&
			(a.Color == Color) &&
			(a.nAlpha == nAlpha) &&
			(a.nBlurRadius == nBlurRadius) &&
			(a.nDistance == nDistance) &&
			(a.nDirection == nDirection));
	}
};

struct NLEBorder
{
	int			IsEnable;		// 1-Enable, 0-Disable
	int			nSize;			// 0-10
	int			Color1;			// Start Color
	int			nAlpha;			// 0-255
	int			nBlurRadius;	// 0-10
	bool			GradedEnabed;//界面自定义部分

	NLEBorder()
	{
		IsEnable = 0;
		nSize = 3;
		Color1 = RGB(255, 0, 0);
		nAlpha = 255;
		nBlurRadius = 0;
		GradedEnabed = false;
	}
	bool operator == (const NLEBorder& a) const
	{
		return ((a.IsEnable == IsEnable) &&
			(a.nSize == nSize) &&
			(a.Color1 == Color1) &&
			(a.nAlpha == nAlpha) &&
			(a.nBlurRadius == nBlurRadius));
	}
};

struct NLETexture
{
	int				IsEnable;				// 1-Enable, 0-Disable
	wchar_t			wszFilePath[MAX_PATH];		// Texture Image Path

	NLETexture()
	{
		IsEnable = 0;
		memset(wszFilePath, 0, sizeof(wchar_t)* MAX_PATH);
	}
	bool operator == (const NLETexture& a) const
	{
		return ((a.IsEnable == IsEnable) &&
			(0 == wcscmp(a.wszFilePath, wszFilePath)));
	}
};
struct NLEFace
{
	int			IsEnable;		// 1-Enable, 0-Disable
	int			Color1;			// Start Color
	int			nAlpha;			// 0-255
	int			nBlurRadius;	// 0-10     最新定义为Effect参数  0-255
	int			Effect;			// 0-42
	NLETexture textureInfo;	// 纹理信息
	bool			GradedEnabed;//界面自定义部分

	NLEFace()
	{
		IsEnable = 1;
		Color1 = RGB(255, 255, 255);
		nAlpha = 255;
		Effect = 0;
		nBlurRadius = 0;
		GradedEnabed = false;
	}
	bool operator == (const NLEFace& a) const
	{
		return ((a.IsEnable == IsEnable) &&
			(a.Color1 == Color1) &&
			(a.nAlpha == nAlpha) &&
			(a.Effect == Effect) &&
			(a.textureInfo == textureInfo));
	}
};


struct NLEVideoObj
{
	wchar_t			wszFilePath[MAX_PATH];		// Background Video Path
	wchar_t			wszImagePath[MAX_PATH];
	int					bCanChangeColor;
	DWORD			Color1;
	float					fDuration;
	float					fPauseTime;
	NLEGradient		Gradient;
	NLEVideoObj()
	{
		Color1 = RGB(255, 255, 255);
		bCanChangeColor = 0;
		memset(wszFilePath, 0, sizeof(wchar_t)* MAX_PATH);
		memset(wszImagePath, 0, sizeof(wchar_t)* MAX_PATH);
		fDuration = 0.0;
		fPauseTime = 0.0;
	}
};


typedef struct tagNLEAnimation
{
	float					dAnimationDuration;
	float					dPauseTime; //拉伸后暂停帧数
	int					nLoopCount;//拉伸后，背景动画循环帧数
	float					dLoopStart;  //循环播放开始时间
	float					dLoopEnd;// 循环播放结束时间

	tagNLEAnimation()
	{
		dAnimationDuration = 0.0;
		dPauseTime = 0.0;
		nLoopCount = 0;
		dLoopStart = 0.0;
		dLoopEnd = 0.0;
	}
}NLEAnimation;


typedef struct tagNLEAnimationConfig
{
	int	AnimationID; //用来与应用层交互定位当前编辑的动画配置项
	//float	startTime;
	//float	duration;
	int	nAnimationType;
	int	nInterpolateType;
	int	nDurationType;
	float	nStartValue;
	float	nEndValue;
	int	nProperty; //动画关联属性ID

	tagNLEAnimationConfig()
	{
		AnimationID = 0;
		//startTime = 0.0;
		//duration = 0.0;
		nAnimationType = 0;
		nInterpolateType = 0;
		nDurationType = 0;
		nStartValue = 0;
		nEndValue = 0;
		nProperty = 0;
	}
}NLEAnimationConfig;

typedef struct tagNLECaptionObjPointsInfo
{
	NLEPointF SizePt[8];
	NLEPointF RotatePt;
	NLEPointF CtrlPt[8];

	tagNLECaptionObjPointsInfo()
	{
		memset(SizePt, 0, sizeof(SizePt));
		memset(&RotatePt, 0, sizeof(RotatePt));
		memset(CtrlPt, 0, sizeof(CtrlPt));
	}
}NLECaptionObjPointsInfo;

typedef enum tagNLECharAnimationType
{
	AT_IN,
	AT_OUT,
}NLECharAnimationType;

enum NLEAnimationProperty
{
	AP_FACEALPHA = 0,
	AP_BORDERALPHA,
	AP_SHADOWALPHA,
	AP_SIZE,
	AP_FACEBLUR,
	AP_BORDERBLUR,
	AP_SHADOWBLUR,
	AP_BORDERSIZE,
	AP_CHARSPACE,
	AP_POS_X,
	AP_POS_Y,
	AP_ROTATION,
	AP_FACECOLOR,
	AP_BORDERCOLOR,
	AP_SHADOWCOLOR,
	AP_FONT_VSIZE,
	AP_FONT_HSIZE,
	AP_POS_PT,
};

enum InterploateType
{
	IT_Linear = 0,
	IT_Quad = 1,
	IT_Cubic = 2,
	IT_Quart = 3,
	IT_Quint = 4,
	IT_Sine = 5,
	IT_Expo = 6,
	IT_Circ = 7,
	IT_Elastic = 8,
	IT_Back = 9,
	IT_Bounce = 10,

	IT_Anti_Quad = 11,
	IT_Anti_Cubic = 12,
	IT_Anti_Quart = 13,
	IT_Anti_Quint = 14,
	IT_Anti_Sine = 15,
	IT_Anti_Expo = 16,
	IT_Anti_Circ = 17,
	IT_Anti_Elastic = 18,
	IT_Anti_Back = 19,
	IT_Anti_Bounce = 20,

	IT_COLOR = 100,
	IT_TICKL = 101,
	IT_ANTITICKL = 102,
	IT_WAVY = 103,
};

typedef enum tagDurationType
{
	DT_ALL_CHAR = 0,//所有字符同时动画
	DT_PER_CHAR = 1, //单个字符均匀动画,需将总时长分配到每个字符上
	DT_ANTIPER_CHAR = 2,//单个字符反向均匀动画
	DT_CENTER_PER_CHAR = 3,//从中间单个字符展开
	DT_ANTICENTER_PER_CHAR = 4,//两边向中间单个字符收起
	DT_MIX_CHAR = 5, //文字动画存在交集
	DT_RAD_CHAR = 6, //单个字符不规则时长动画，范围为（总时长范围内的随机值）
	DT_SHORT_MIX_CHAR = 7, //文字动画存在交集
	DT_LONG_MIX_CHAR = 8, //文字动画存在交集
	DT_ROW_ALL_CHAR = 10,//以行为单位运动
	DT_ROW_MIX_CHAR = 11,//以行为单位运动
	DT_ANTIROW_MIX_CHAR = 12,//逆序以行为单位运动
	DT_WORD_ALL_CHAR = 20, //以单词为单位运动
	DT_WORD_MIX_CHAR = 21, //以单词为单位运动,动画存在交集
	DT_WORD_SHORT_MIX_CHAR = 22, //以单词为单位运动,动画存在交集
}DurationType;


typedef enum tagMotionType
{
	MT_ALL_FADE = 0,
	MT_SIZE = 1,
	MT_ALL_BLUR = 2,
	MT_BORDER_SIZE = 3,
	MT_CHARSPACE = 4,
	//MT_ROTATION = 5,
	MT_FONT_VSIZE = 6,
	MT_FONT_HSIZE = 7,
	//MT_POS_L_R_X = 101,
	//MT_POS_R_L_X = 102,
	//MT_POS_R_L_ROLL_X = 110,
	//MT_POS_T_B_Y = 201,
	//MT_POS_B_T_Y = 202,
	MT_POS_Y_UP_MOVE = 203,
	MT_POS_Y_DOWN_MOVE = 204,
	MT_POS_Y_RAD_UP_DOWN_MOVE = 205,
	//MT_POS_X_LEFT_RIGHT_MOVE = 206,
	//MT_POS_X_RIGHT_LEFT_MOVE = 207,
	//MT_POS_Y_UP_DOWN_MOVE = 208,
	//MT_POS_Y_DOWN_UP_MOVE = 209,
	//MT_POS_B_T_ROLL_Y = 210,
	//MT_POS_X_LEFT_MOVE = 211,
	//MT_POS_X_RIGHT_MOVE = 212,
}MotionType;

const WORD Arbic_Position[][4] =  // first, last, middle, alone
{
	{ 0xfe80, 0xfe80, 0xfe80, 0xfe80 },    // 0x621
	{ 0xfe82, 0xfe81, 0xfe82, 0xfe81 },
	{ 0xfe84, 0xfe83, 0xfe84, 0xfe83 },
	{ 0xfe86, 0xfe85, 0xfe86, 0xfe85 },
	{ 0xfe88, 0xfe87, 0xfe88, 0xfe87 },
	{ 0xfe8a, 0xfe8b, 0xfe8c, 0xfe89 },
	{ 0xfe8e, 0xfe8d, 0xfe8e, 0xfe8d },
	{ 0xfe90, 0xfe91, 0xfe92, 0xfe8f },   // 0x628
	{ 0xfe94, 0xfe93, 0xfe93, 0xfe93 },
	{ 0xfe96, 0xfe97, 0xfe98, 0xfe95 },   // 0x62A
	{ 0xfe9a, 0xfe9b, 0xfe9c, 0xfe99 },
	{ 0xfe9e, 0xfe9f, 0xfea0, 0xfe9d },
	{ 0xfea2, 0xfea3, 0xfea4, 0xfea1 },
	{ 0xfea6, 0xfea7, 0xfea8, 0xfea5 },
	{ 0xfeaa, 0xfea9, 0xfeaa, 0xfea9 },
	{ 0xfeac, 0xfeab, 0xfeac, 0xfeab },   // 0x630  
	{ 0xfeae, 0xfead, 0xfeae, 0xfead },
	{ 0xfeb0, 0xfeaf, 0xfeb0, 0xfeaf },
	{ 0xfeb2, 0xfeb3, 0xfeb4, 0xfeb1 },
	{ 0xfeb6, 0xfeb7, 0xfeb8, 0xfeb5 },
	{ 0xfeba, 0xfebb, 0xfebc, 0xfeb9 },
	{ 0xfebe, 0xfebf, 0xfec0, 0xfebd },
	{ 0xfec2, 0xfec3, 0xfec4, 0xfec1 },
	{ 0xfec6, 0xfec7, 0xfec8, 0xfec5 },  // 0x638
	{ 0xfeca, 0xfecb, 0xfecc, 0xfec9 },
	{ 0xfece, 0xfecf, 0xfed0, 0xfecd },  //0x63A
	{ 0x63b, 0x63b, 0x63b, 0x63b },
	{ 0x63c, 0x63c, 0x63c, 0x63c },
	{ 0x63d, 0x63d, 0x63d, 0x63d },
	{ 0x63e, 0x63e, 0x63e, 0x63e },
	{ 0x63f, 0x63f, 0x63f, 0x63f },
	{ 0x640, 0x640, 0x640, 0x640 },   // 0x640
	{ 0xfed2, 0xfed3, 0xfed4, 0xfed1 },
	{ 0xfed6, 0xfed7, 0xfed8, 0xfed5 },
	{ 0xfeda, 0xfedb, 0xfedc, 0xfed9 },
	{ 0xfede, 0xfedf, 0xfee0, 0xfedd },
	{ 0xfee2, 0xfee3, 0xfee4, 0xfee1 },
	{ 0xfee6, 0xfee7, 0xfee8, 0xfee5 },
	{ 0xfeea, 0xfeeb, 0xfeec, 0xfee9 },
	{ 0xfeee, 0xfeed, 0xfeee, 0xfeed },   // 0x648
	{ 0xFEF0, 0xFBE8, 0xFBE9, 0xFEEF },  // 0x649
	{ 0xfef2, 0xfef3, 0xfef4, 0xfef1 },   // 0x64A
};

const WORD theSet1[23] = {
	0x626, 0x628, 0x62a, 0x62b, 0x62c, 0x62d, 0x62e,
	0x633, 0x634, 0x635, 0x636, 0x637, 0x638, 0x639, 0x63a,
	0x641, 0x642, 0x643, 0x644, 0x645, 0x646, 0x647, 0x64a

};

const WORD theSet2[36] = {
	0x622, 0x623, 0x624, 0x625, 0x626, 0x627, 0x628, 0x629, 0x62a, 0x62b, 0x62c, 0x62d, 0x62e, 0x62f,
	0x630, 0x631, 0x632, 0x633, 0x634, 0x635, 0x636, 0x637, 0x638, 0x639, 0x63a,
	0x641, 0x642, 0x643, 0x644, 0x645, 0x646, 0x647, 0x648, 0x649, 0x64a,

	0x6EE
};

const WORD arabic_specs[][2] =   //0644
{
	{ 0xFEF5, 0xFEF6 },	//0x622
	{ 0xFEF7, 0xFEF8 },	//0x623
	{ 0xFEF9, 0xFEFA },	//0x625
	{ 0xFEFB, 0xFEFC }, //0x627
};

const WORD ComboRangs[][2] = {
	{ 0x610, 0x615 },  // ISO8859_6 组合符号 Arabic
	{ 0x64b, 0x65e },
	{ 0x6d6, 0x6ed },
	//{ 0xe31, 0xe31 }, // ISO8859_11 组合符号 Thai  暂不支持泰语
	//{ 0xe34, 0xe3a },
	//{ 0xe47, 0xe4e },
};

const WORD ASCII_Symbol[29] =
{
	32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,    //!, ", #, $, %, &, ', (, ), *, +, ,, -, ., /,
	58, 59, 60, 61, 62, 63, 64,															//:, ;, <, =, >, ?, @, 
	91, 92, 93, 94, 95, 96,																	//[, \, ], ^, _,	 `,
};

#endif



/*
A

FC5E	064C 0651
FC5F		064D 0651
FC60	064E 0651
FC61	064F 0651
FC62	0650 0651
FC63	0651 0670

FCF2		0640 064E 0651
FCF3		0640 064F 0651
FCF4		0640 0650 0651

B
FE70		064B
FE71		0640 064B
FE72		064C
FE74		064D
FE76		064E
FE77		0640 064E
FE78		064F
FE79		0640 064F
FE7A		0650
FE7B		0640 0650
FE7C	0651
FE7D	0640 0651
FE7E		0652
FE7F		0640 0652




*/
