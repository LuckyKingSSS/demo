#pragma once

typedef char				NLEchar;
typedef unsigned char		NLEbyte;
typedef short				NLEshort;
typedef unsigned short		NLEushort;
typedef int					NLEint;
typedef unsigned int		NLEuint;
typedef long long			NLElonglong;
typedef unsigned long long	NLEulonglong;

typedef wchar_t				NLEwchar;
typedef float				NLEREAL;

typedef struct _tagNLERational
{	
	NLElonglong num;	/**@brief 分子*/
	NLElonglong den;	/**@brief 分母*/
}tagNLERational;

typedef struct _tagNLESize
{
	INT Width;
	INT Height;
}tagNLESize;

typedef struct _tagNLESizeF
{
	NLEREAL Width;
	NLEREAL Height;
}tagNLESizeF;

typedef struct  _tagNLEPoint
{
	INT X;
	INT Y;
}tagNLEPoint;

typedef struct _tagNLEPointF
{
	NLEREAL X;
	NLEREAL Y;
}tagNLEPointF;

typedef struct _tagNLERect
{
	INT X;
	INT Y;
	INT Width;
	INT Height;
}tagNLERect;

typedef struct _tagNLERectF
{
	NLEREAL X;
	NLEREAL Y;
	NLEREAL Width;
	NLEREAL Height;
}tagNLERectF;

typedef struct _tagMask
{
	BOOL bEnable;
	int nAlpha;
	float fPosX;
	float fPosY;
	float fSizeX;
	float fSizeY;
}MASKPARAM, *PMASKPARAM;

typedef enum _tagKeyType
{
	KT_CHROMAKEY = 0,
}KEYTYPE;

typedef struct _tagKeying
{
	BOOL bEnable;
	KEYTYPE uType;
	DWORD dwValue;
	int nLevel;
}KEYINGPARAM, *PKEYINGPARAM;

typedef enum _tagDirection
{
	D_TOP = 0,
	D_LEFT = D_TOP + 1,
	D_DOWN = D_LEFT + 1,
	D_RIGHT = D_DOWN + 1,
	D_LEFT_TOP = D_RIGHT + 1,
	D_RIGHT_TOP = D_LEFT_TOP + 1,
	D_LEFT_DOWN = D_RIGHT_TOP + 1,
	D_RIGHT_DOWN = D_LEFT_DOWN + 1,
}DIRECTION;

typedef struct _tagShadow
{
	BOOL bEnable;
	int nDistance; 
	int nBlur;
	int nAlpha;
	DWORD dwValue;
	DIRECTION uDirect; 
}SHADOWSPARAM, *PSHADOWSPARAM;

typedef struct _tagBorder
{
	BOOL bEnable;
	int nSize;
	int nBlur;
	int nAlpha;
	DWORD dwBegin;
	DWORD dwEnd;
	DIRECTION uDirect;
}BORDERPARAM, *PBORDERPARAM;

struct NLERGB
{
	int r;
	int g;
	int b;
	int nReserve;

};

typedef enum 
{
	RepositoryType_VideoFilter,
	RepositoryType_AudioFilter,
	RepositoryType_Mask,
	RepositoryType_MotionTemplate,
}RepositoryType;