#pragma once
#include "NLEType.h"


namespace NLEMatrix
{
	///矩阵赋值
	void MatrixAssign(const NLETransformMatrix &mFrom, NLETransformMatrix &mTo);
	///矩阵乘法
	void MatrixMultiplay(const NLETransformMatrix &mA, const NLETransformMatrix &mB, NLETransformMatrix &mC);
	///获取单位矩阵
	void GetUnitAffineMatrix(NLETransformMatrix &mA);
	const NLETransformMatrix& GetUnitAffineMatrix();

	///获取旋转矩阵
	void GetRotateAffineMatrix(NLETransformMatrix &mA, float fAngle);
	///获取绽放矩阵
	void GetScaleMatrix(NLETransformMatrix &mA, float fScaleX, float fScaleY);
	///获取平移矩阵
	void GetTranslateMatrix(NLETransformMatrix &mA, float fTransX, float fTransY);
	
	///应用缩放
	void ApplyScale(NLETransformMatrix &mA, float fScaleX, float fScaleY);
	void ApplyRotate(NLETransformMatrix &mA, float fAngle);
	void ApplyTranslate(NLETransformMatrix &mA, float fTransX, float fTransY);
};
namespace NLEMatrix
{
	class CNLEAffineMatrix
	{
	public:
		CNLEAffineMatrix();
	public:
		CNLEAffineMatrix(const NLETransformMatrix &oMatrix);
		void operator =(const CNLEAffineMatrix &from);
		void Multiply(const CNLEAffineMatrix &oThat);
		void MakeUnit();
		void ApplyScale(float fx, float fy);
		void ApplyRotate(float fAnlge);
		void ApplyTranslate(float xOffset, float yOffset);
	public:
		void GetTranslate(float *pXoffset, float *pYoffset);
		bool IsScaleOrRotate();

	public:
		const NLETransformMatrix &GetAffineMatrix();
	private:
		NLETransformMatrix matrix;
	};
};