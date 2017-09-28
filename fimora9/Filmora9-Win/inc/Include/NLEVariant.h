#pragma once

#ifndef _NLE_VARIANT_TYPE_DEFINE_
#define _NLE_VARIANT_TYPE_DEFINE_

//#include <Unknwn.h>
#include "NLEType.h"
#include "NLEMacroDefine.h"

typedef enum _NLE_VARIANT_TYPE
{
	NLEVariantInvalid = 0,
	NLEVariantInt = 1,	//32bit
	NLEVariantLongLong = 2,	//64bit
	NLEVariantDouble = 3,	//double
	NLEVariantString = 4,	//wchar*
	NLEVariantBlob = 5,	//BYTE*
	NLEVariantInterface = 6,	//IUnknown
	NLEVariantRect = 7,  //NLERect
	NLEVariantRectF = 8,  //NLERectF
	NLEVariantPoint = 9,	//NLEPoint
	NLEVariantPointF = 10,	//NLEPointF
	NLEVariantGUID = 11,		//GUID
	NLEVariantRational = 12, //NLERational
	NLEVariantValue = 13, //CNLEValue
} NLEVariantType;

typedef struct NLEVariant
{
	NLEVariantType type;
	union
	{
		INT			nValue;  //NLEVariantInt
		NLElonglong	llValue; //NLEVariantInt64
		DOUBLE		dValue; //NLEVariantDouble
		WCHAR*		pStr; //NLEVariantString
		IUnknown	*pUnk;  //NLEVariantInterface
		GUID		oGuid;
	};
	NLEBlob		oBlob;
	NLERect		oRect;
	NLERectF	oRectF;
	NLEPoint	oPoint;
	NLEPointF	oPointF;
	NLERational oRational;	
}NLEVariant;
#endif //_NLE_VARIANT_TYPE_DEFINE

