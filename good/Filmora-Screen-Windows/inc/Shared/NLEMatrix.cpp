#include "stdafx.h"
#include "NLEMatrix.h"
#include <math.h>

static const double PI = 3.1415926535;

#define DegreeToRad(angle) (angle * PI / 180.0)
#define RadToDegree(rad) (rad * 180.0f/PI)


namespace NLEMatrix
{
	typedef NLETransformMatrix NLETransformMatrix;

	void MatrixAssign(const NLETransformMatrix &mFrom, NLETransformMatrix &mTo)
	{
		memcpy(mTo, mFrom, sizeof(NLETransformMatrix));
	}

	void MatrixMultiplay(const NLETransformMatrix &mA, const NLETransformMatrix &mB, NLETransformMatrix &mC)
	{
		NLETransformMatrix _mA, _mB;
		MatrixAssign(mA, _mA);
		MatrixAssign(mB, _mB);

		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				mC[r][c] = 0.0;
				for (int i = 0; i < 3; i++)
				{
					mC[r][c] += _mA[r][i] * _mB[i][c];
				}
			}
		}
	}

	void GetUnitAffineMatrix(NLETransformMatrix &mA)
	{
		NLETransformMatrix mU =
		{
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 },
		};
		MatrixAssign(mU, mA);
	}

	const NLETransformMatrix& GetUnitAffineMatrix()
	{
		static NLETransformMatrix mU =
		{
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 },
		};
		return mU;
	}

	void GetRotateAffineMatrix(NLETransformMatrix &mA, float fAngle)
	{
		NLETransformMatrix mE;
		GetUnitAffineMatrix(mE);

		double rad = DegreeToRad(fAngle);
		double fcos = cos(rad);
		double fsin = sin(rad);
		NLETransformMatrix mR =
		{
			{ fcos, -fsin, 0 },
			{ fsin, fcos, 0 },
			{ 0, 0, 1 },
		};

		MatrixMultiplay(mE, mR, mA);
	}

	void GetScaleMatrix(NLETransformMatrix &mA, float fScaleX, float fScaleY)
	{
		NLETransformMatrix mE;
		GetUnitAffineMatrix(mE);

		NLETransformMatrix mS =
		{
			{ fScaleX, 0, 0 },
			{ 0, fScaleY, 0 },
			{ 0, 0, 1 },
		};

		MatrixMultiplay(mE, mS, mA);
	}

	void GetTranslateMatrix(NLETransformMatrix &mA, float fTransX, float fTransY)
	{
		NLETransformMatrix mE;
		GetUnitAffineMatrix(mE);

		NLETransformMatrix mT =
		{
			{ 1, 0, fTransX },
			{ 0, 1, fTransY },
			{ 0, 0, 1 },
		};

		MatrixMultiplay(mE, mT, mA);
	}

	void ApplyScale(NLETransformMatrix &mA, float fScaleX, float fScaleY)
	{
		NLETransformMatrix mScale;
		GetScaleMatrix(mScale, fScaleX, fScaleY);
		MatrixMultiplay(mA, mScale, mA);
	}

	void ApplyRotate(NLETransformMatrix &mA, float fAngle)
	{
		NLETransformMatrix mRotate;
		GetRotateAffineMatrix(mRotate, fAngle);
		MatrixMultiplay(mA, mRotate, mA);
	}

	void ApplyTranslate(NLETransformMatrix &mA, float fTransX, float fTransY)
	{
		NLETransformMatrix mTranslate;
		GetTranslateMatrix(mTranslate, fTransX, fTransY);
		MatrixMultiplay(mA, mTranslate, mA);
	}
};

namespace NLEMatrix
{
	void CNLEAffineMatrix::operator=(const CNLEAffineMatrix &from)
	{
		MatrixAssign(from.matrix, this->matrix);
	}

	CNLEAffineMatrix::CNLEAffineMatrix(const NLETransformMatrix &oMatrix)
	{
		MatrixAssign(oMatrix, this->matrix);
	}

	CNLEAffineMatrix::CNLEAffineMatrix()
	{

	}

	void CNLEAffineMatrix::Multiply(const CNLEAffineMatrix &oThat)
	{
		MatrixMultiplay(this->matrix, oThat.matrix, this->matrix);
	}

	void CNLEAffineMatrix::MakeUnit()
	{
		GetUnitAffineMatrix(this->matrix);
	}

	void CNLEAffineMatrix::ApplyScale(float fx, float fy)
	{
		NLETransformMatrix matxScale;
		GetScaleMatrix(matxScale, fx, fy);
		MatrixMultiplay(this->matrix, matxScale, this->matrix);
	}

	void CNLEAffineMatrix::ApplyRotate(float fAnlge)
	{
		NLETransformMatrix matxRotate;
		GetRotateAffineMatrix(matxRotate, fAnlge);
		MatrixMultiplay(this->matrix, matxRotate, this->matrix);
	}

	void CNLEAffineMatrix::ApplyTranslate(float xOffset, float yOffset)
	{
		NLETransformMatrix matxTranslate;
		GetTranslateMatrix(matxTranslate, xOffset, yOffset);
		this->matrix[0][2] += xOffset;
		this->matrix[1][2] += yOffset;
		//MatrixMultiplay(this->matrix, matxTranslate, this->matrix);
	}

	
	void CNLEAffineMatrix::GetTranslate(float *pXoffset, float *pYoffset)
	{
		//NLE_ASSERT(pXoffset && pYoffset);
		*pXoffset = (float)this->matrix[0][2];
		*pYoffset = (float)this->matrix[1][2];
	}

	bool CNLEAffineMatrix::IsScaleOrRotate()
	{
		return (this->matrix[0][0] != 1.0 || this->matrix[1][1] != 1.0)
			|| (this->matrix[0][1] != 0 || this->matrix[1][0] != 0);
	}

	const NLEMatrix::NLETransformMatrix & CNLEAffineMatrix::GetAffineMatrix()
	{
		return this->matrix; 
	}

};
