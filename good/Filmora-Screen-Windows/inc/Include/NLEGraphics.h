#pragma once

#ifndef _H_NLEGRAPHICS_H_
#define _H_NLEGRAPHICS_H_

#include <stdio.h>

#include "NLEType.h"
#include "NLEMacroDefine.h"
#include <math.h>
#include <string>
class NLESize;
class NLESizeF;
class NLEPoint;
class NLEPointF;
class NLERect;
class NLERectF;



#ifndef REAL_EPSILON
#define REAL_EPSILON        1.192092896e-07F        /* FLT_EPSILON */
#endif


#ifndef _MSC_VER
#define swscanf_s swscanf
#endif

class NLESizeF:public tagNLESizeF
{
public:
	NLESizeF()
	{
		Width = Height = 0.0f;
	}

	NLESizeF(  const NLESizeF& size)
	{
		Width = size.Width;
		Height = size.Height;
	}

	NLESizeF(  NLEREAL width,
		  NLEREAL height)
	{
		Width = width;
		Height = height;
	}

	NLESizeF operator+(  const NLESizeF& sz) const
	{
		return NLESizeF(Width + sz.Width,
			Height + sz.Height);
	}

	NLESizeF operator-(  const NLESizeF& sz) const
	{
		return NLESizeF(Width - sz.Width,
			Height - sz.Height);
	}

	BOOL Equals(  const NLESizeF& sz) const
	{
		return (Width == sz.Width) && (Height == sz.Height);
	}

	BOOL Empty() const
	{
		return (Width == 0.0f && Height == 0.0f);
	}

	const WCHAR* toString()
	{
		static WCHAR pStr[64];
		swprintf(pStr, 64, L"Width:%f, cy:%f", this->Width, this->Height);
		return pStr;
	}
	int fromString(WCHAR* pStr)
	{
		return swscanf_s(pStr, L"Width:%f, cy:%f", &this->Width, &this->Height);
	}
public:

	
};

//--------------------------------------------------------------------------
// Represents a dimension in a 2D coordinate system (integer coordinates)
//--------------------------------------------------------------------------

class NLESize:public tagNLESize
{
public:
	NLESize()
	{
		Width = Height = 0;
	}

	NLESize(  const NLESize& size)
	{
		Width = size.Width;
		Height = size.Height;
	}

	NLESize(  INT width,
		  INT height)
	{
		Width = width;
		Height = height;
	}

	NLESize operator+(  const NLESize& sz) const
	{
		return NLESize(Width + sz.Width,
			Height + sz.Height);
	}

	NLESize operator-(  const NLESize& sz) const
	{
		return NLESize(Width - sz.Width,
			Height - sz.Height);
	}

	BOOL Equals(  const NLESize& sz) const
	{
		return (Width == sz.Width) && (Height == sz.Height);
	}

	BOOL Empty() const
	{
		return (Width == 0 && Height == 0);
	}

	const WCHAR* toString()
	{
		static WCHAR pStr[64];
		swprintf(pStr, 64, L"Width:%d, cy:%d", this->Width, this->Height);
		return pStr;
	}
	int fromString(WCHAR* pStr)
	{
		return swscanf_s(pStr, L"Width:%d, cy:%d", &this->Width, &this->Height);
	}
public:

	
};

//--------------------------------------------------------------------------
// Represents a location in a 2D coordinate system (floating-point coordinates)
//--------------------------------------------------------------------------

class NLEPointF:public tagNLEPointF
{
public:
	NLEPointF()
	{
		X = Y = 0.0f;
	}

	NLEPointF(  const NLEPointF &point)
	{
		X = point.X;
		Y = point.Y;
	}

	NLEPointF(  const NLESizeF &size)
	{
		X = size.Width;
		Y = size.Height;
	}

	NLEPointF(  NLEREAL x,
		  NLEREAL y)
	{
		X = x;
		Y = y;
	}

	NLEPointF operator+(  const NLEPointF& point) const
	{
		return NLEPointF(X + point.X,
			Y + point.Y);
	}

	NLEPointF operator-(  const NLEPointF& point) const
	{
		return NLEPointF(X - point.X,
			Y - point.Y);
	}

	BOOL Equals(  const NLEPointF& point) const
	{
		return (X == point.X) && (Y == point.Y);
	}

	const WCHAR* toString()
	{
#define STR_LEN_1 50
		static WCHAR pStr[STR_LEN_1];
		swprintf(pStr, STR_LEN_1, L"x:%f, y:%f", this->X, this->Y);
		return pStr;
	}
	int fromString(WCHAR* pStr)
	{
		return swscanf_s(pStr, L"x:%f, y:%f", &this->X, &this->Y);
	}
public:

	
};

//--------------------------------------------------------------------------
// Represents a location in a 2D coordinate system (integer coordinates)
//--------------------------------------------------------------------------

class NLEPoint:public tagNLEPoint
{
public:
	NLEPoint()
	{
		X = Y = 0;
	}

	NLEPoint(  const NLEPoint &point)
	{
		X = point.X;
		Y = point.Y;
	}

	NLEPoint(  const NLESize &size)
	{
		X = size.Width;
		Y = size.Height;
	}

	NLEPoint(  INT x,
		  INT y)
	{
		X = x;
		Y = y;
	}

	NLEPoint operator+(  const NLEPoint& point) const
	{
		return NLEPoint(X + point.X,
			Y + point.Y);
	}

	NLEPoint operator-(  const NLEPoint& point) const
	{
		return NLEPoint(X - point.X,
			Y - point.Y);
	}

	BOOL Equals(  const NLEPoint& point)
	{
		return (X == point.X) && (Y == point.Y);
	}

	
	const WCHAR* toString()
	{
		static WCHAR pStr[20];
		swprintf(pStr, 20, L"x:%d, y:%d", this->X, this->Y);
		return pStr;
	}
	int fromString(WCHAR* pStr)
	{
		return swscanf_s(pStr, L"x:%d, y:%d", &this->X, &this->Y);
	}
public:
	
	
};

class NLERange
{
public:
	NLERange()
	{
		Start = 0;
		End = 0;
	}
	NLERange(INT nStart, INT nEnd) 
	{
		Start = nStart;
		End = nEnd;
	}
	NLERange(const NLERange &rng)
	{
		Start = rng.Start;
		End = rng.End;
	}

	BOOL Equals(const NLERange& rng)
	{
		return (Start == rng.Start) && (End == rng.End);
	}

	BOOL InRange(INT nCheck)
	{
		return ( Start<= nCheck && nCheck <= End);
	}

	const WCHAR* toString()
	{
		static WCHAR pStr[64];
		swprintf(pStr, 64, L"Start:%d, End:%d", this->Start, this->End);
		return pStr;
	}
	int fromString(WCHAR* pStr)
	{
		return swscanf_s(pStr, L"Start:%d, End:%d", &this->Start, &this->End);
	}
public:
	BOOL IsEmplty() const
	{
		return (this->End < this->Start);
	}

	INT Count() const
	{
		if (this->IsEmplty())
		{
			return 0;
		}
		else
		{
			return (this->End - this->Start + 1);
		}
	}

public:
	NLERange& operator = (const NLERange& rng)
	{
		this->Start = rng.Start;
		this->End = rng.End;
		return *this;
	}
public:

	INT Start;
	INT End;
};


//--------------------------------------------------------------------------
// Represents a rectangle in a 2D coordinate system (integer coordinates)
//--------------------------------------------------------------------------

class NLERect:public tagNLERect
{
public:

	NLERect()
	{
		X = Y = Width = Height = 0;
	}

	NLERect(  INT x,
		  INT y,
		  INT width,
		  INT height)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}

	NLERect(  const NLEPoint& location,
		  const NLESize& size)
	{
		X = location.X;
		Y = location.Y;
		Width = size.Width;
		Height = size.Height;
	}

	NLERect* Clone() const
	{
		return new NLERect(X, Y, Width, Height);
	}

	VOID GetLocation(NLEPoint* point) const
	{
		point->X = X;
		point->Y = Y;
	}

	VOID GetSize(NLESize* size) const
	{
		size->Width = Width;
		size->Height = Height;
	}

	VOID GetBounds(NLERect* rect) const
	{
		rect->X = X;
		rect->Y = Y;
		rect->Width = Width;
		rect->Height = Height;
	}

	INT GetLeft() const
	{
		return X;
	}

	INT GetTop() const
	{
		return Y;
	}

	INT GetRight() const
	{
		return X + Width;
	}

	INT GetBottom() const
	{
		return Y + Height;
	}

	BOOL IsEmptyArea() const
	{
		return (Width <= 0) || (Height <= 0);
	}

	BOOL Equals(  const NLERect & rect) const
	{
		return X == rect.X &&
			Y == rect.Y &&
			Width == rect.Width &&
			Height == rect.Height;
	}

	BOOL Contains(  INT x,
		  INT y) const
	{
		return x >= X && x < X + Width &&
			y >= Y && y < Y + Height;
	}

	BOOL Contains(  const NLEPoint& pt) const
	{
		return Contains(pt.X, pt.Y);
	}

	BOOL Contains(  NLERect& rect) const
	{
		return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
			(Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
	}

	VOID Inflate(  INT dx,
		  INT dy)
	{
		X -= dx;
		Y -= dy;
		Width += 2 * dx;
		Height += 2 * dy;
	}

	VOID Inflate(  const NLEPoint& point)
	{
		Inflate(point.X, point.Y);
	}

	BOOL Intersect(  const NLERect& rect)
	{
		return Intersect(*this, *this, rect);
	}

	static BOOL Intersect(NLERect& c,
		  const NLERect& a,
		  const NLERect& b)
	{
		INT right = NLE_MIN(a.GetRight(), b.GetRight());
		INT bottom = NLE_MIN(a.GetBottom(), b.GetBottom());
		INT left = NLE_MAX(a.GetLeft(), b.GetLeft());
		INT top = NLE_MAX(a.GetTop(), b.GetTop());

		c.X = left;
		c.Y = top;
		c.Width = right - left;
		c.Height = bottom - top;
		return !c.IsEmptyArea();
	}

	BOOL IntersectsWith(  const NLERect& rect) const
	{
		return (GetLeft() < rect.GetRight() &&
			GetTop() < rect.GetBottom() &&
			GetRight() > rect.GetLeft() &&
			GetBottom() > rect.GetTop());
	}

	static BOOL Union(NLERect& c,
		  const NLERect& a,
		  const NLERect& b)
	{
		INT right = NLE_MAX(a.GetRight(), b.GetRight());
		INT bottom = NLE_MAX(a.GetBottom(), b.GetBottom());
		INT left = NLE_MIN(a.GetLeft(), b.GetLeft());
		INT top = NLE_MIN(a.GetTop(), b.GetTop());

		c.X = left;
		c.Y = top;
		c.Width = right - left;
		c.Height = bottom - top;
		return !c.IsEmptyArea();
	}

	VOID Offset(  const NLEPoint& point)
	{
		Offset(point.X, point.Y);
	}

	VOID Offset(  INT dx,
		  INT dy)
	{
		X += dx;
		Y += dy;
	}

	const std::wstring toString()
	{
		WCHAR pStr[50];
		swprintf(pStr, 50, L"left:%d, top:%d, width:%d, height:%d", this->X, this->Y, this->Width, this->Height);

		std::wstring str = pStr;
		return str;
	}
	int fromString(WCHAR* pStr)
	{		
		int nret = swscanf_s(pStr, L"left:%d, top:%d, width:%d, height:%d", &this->X, &this->Y, &this->Width, &this->Height);
		return nret;
	}
public:

	
};

//--------------------------------------------------------------------------
// Represents a rectangle in a 2D coordinate system (floating-point coordinates)
//--------------------------------------------------------------------------

class NLERectF :public tagNLERectF
{
public:

	NLERectF()
	{
		X = Y = Width = Height = 0.0f;
	}

	NLERectF(NLEREAL x,
		NLEREAL y,
		NLEREAL width,
		NLEREAL height)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}

	NLERectF(const NLEPointF& location,
		const NLESizeF& size)
	{
		X = location.X;
		Y = location.Y;
		Width = size.Width;
		Height = size.Height;
	}

	NLERectF* Clone() const
	{
		return new NLERectF(X, Y, Width, Height);
	}

	VOID GetLocation(NLEPointF* point) const
	{
		point->X = X;
		point->Y = Y;
	}

	VOID GetSize(NLESizeF* size) const
	{
		size->Width = Width;
		size->Height = Height;
	}

	VOID GetBounds(NLERectF* rect) const
	{
		rect->X = X;
		rect->Y = Y;
		rect->Width = Width;
		rect->Height = Height;
	}

	NLEREAL GetLeft() const
	{
		return X;
	}

	NLEREAL GetTop() const
	{
		return Y;
	}

	NLEREAL GetRight() const
	{
		return X + Width;
	}

	NLEREAL GetBottom() const
	{
		return Y + Height;
	}

	BOOL IsEmptyArea() const
	{
		return (Width <= REAL_EPSILON) || (Height <= REAL_EPSILON);
	}

	BOOL Equals(const NLERectF & rect) const
	{
		return (
				(fabs(X - rect.X) < REAL_EPSILON) &&
				(fabs(Y - rect.Y) <REAL_EPSILON) &&
				(fabs(Width - rect.Width) < REAL_EPSILON) &&
				(fabs(Height - rect.Height) < REAL_EPSILON)
				);
	}

	BOOL Contains(NLEREAL x,
		NLEREAL y) const
	{
		return x >= X && x < X + Width &&
			y >= Y && y < Y + Height;
	}

	BOOL Contains(const NLEPointF& pt) const
	{
		return Contains(pt.X, pt.Y);
	}

	BOOL Contains(const NLERectF& rect) const
	{
		return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
			(Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
	}

	VOID Inflate(NLEREAL dx,
		NLEREAL dy)
	{
		X -= dx;
		Y -= dy;
		Width += 2 * dx;
		Height += 2 * dy;
	}

	VOID Inflate(const NLEPointF& point)
	{
		Inflate(point.X, point.Y);
	}

	BOOL Intersect(const NLERectF& rect)
	{
		return Intersect(*this, *this, rect);
	}

	static BOOL Intersect(NLERectF& c,
		const NLERectF& first,
		const NLERectF& b)
	{
		NLEREAL right = NLE_MIN(first.GetRight(), b.GetRight());
		NLEREAL bottom = NLE_MIN(first.GetBottom(), b.GetBottom());
		NLEREAL left = NLE_MAX(first.GetLeft(), b.GetLeft());
		NLEREAL top = NLE_MAX(first.GetTop(), b.GetTop());

		c.X = left;
		c.Y = top;
		c.Width = right - left;
		c.Height = bottom - top;
		return !c.IsEmptyArea();
	}

	BOOL IntersectsWith(const NLERectF& rect) const
	{
		return (GetLeft() < rect.GetRight() &&
			GetTop() < rect.GetBottom() &&
			GetRight() > rect.GetLeft() &&
			GetBottom() > rect.GetTop());
	}

	static BOOL Union(NLERectF& c,
		const NLERectF& a,
		const NLERectF& b)
	{
		NLEREAL right = NLE_MAX(a.GetRight(), b.GetRight());
		NLEREAL bottom = NLE_MAX(a.GetBottom(), b.GetBottom());
		NLEREAL left = NLE_MIN(a.GetLeft(), b.GetLeft());
		NLEREAL top = NLE_MIN(a.GetTop(), b.GetTop());

		c.X = left;
		c.Y = top;
		c.Width = right - left;
		c.Height = bottom - top;
		return !c.IsEmptyArea();
	}

	VOID Offset(const NLEPointF& point)
	{
		Offset(point.X, point.Y);
	}

	VOID Offset(NLEREAL dx,
		NLEREAL dy)
	{
		X += dx;
		Y += dy;
	}
	/**@brief X/Width缩放sx倍，Y/Height缩放sy倍*/
	void Scale(NLEREAL sx, NLEREAL sy)
	{
		X *= sx; Width *= sx;
		Y *= sy; Height *= sy;
	}

	void GetRect(NLERect *pRect)
	{
		pRect->X = (INT)X;
		pRect->Y = (INT)Y;
		pRect->Width = (INT)Width;
		pRect->Height = (INT)Height;
	}
	const WCHAR* toString()
	{
		static WCHAR pStr[100];
		swprintf(pStr, 100, L"left:%f, top:%f, width:%f, height:%f", this->X, this->Y, this->Width, this->Height);
		return pStr;
	}
	int fromString(WCHAR* pStr)
	{
		int nret = swscanf_s(pStr, L"left:%f, top:%f, width:%f, height:%f", &this->X, &this->Y, &this->Width, &this->Height);
		return nret;
	}
public:

};

class NLEPathData
{
public:
	NLEPathData()
	{
		Count = 0;
		Points = NULL;
		Types = NULL;
	}

	~NLEPathData()
	{
		if (Points != NULL)
		{
			delete[] Points;
		}

		if (Types != NULL)
		{
			delete[] Types;
		}
	}

private:
	NLEPathData(const NLEPathData &);
	NLEPathData& operator=(const NLEPathData &);

public:
	INT Count;
	NLEPointF* Points;
	BYTE* Types;
};

class NLEColorF
{
public:
	NLEColorF()
	{
		r = g = b = 0.0f;
	}
	NLEColorF(NLEREAL r, NLEREAL g, NLEREAL b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	const WCHAR* toString()
	{
		static WCHAR pStr[100];
		swprintf(pStr, 100, L"r:%f, g:%f, b:%f", this->r, this->g, this->b);
		return pStr;
	}
	int fromString(WCHAR* pStr)
	{
		int nret = swscanf_s(pStr, L"r:%f, g:%f, b:%f", &this->r, &this->g, &this->b);
		return nret;
	}

public:
	NLEREAL r;
	NLEREAL g;
	NLEREAL b;
};


#endif //_H_NLEGRAPHICS_H_


