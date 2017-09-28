#include "NLECommon.h"

namespace NLECommon
{
	namespace Math
	{			
		NLECOMMON_API NLElonglong GetGCD(NLElonglong m, NLElonglong n)
		{
			NLElonglong t;
            NLElonglong a = NLE_MAX(m, n);
            NLElonglong b = NLE_MIN(m, n);

			if (a < b)
			{
				t = a; a = b, b = t;
			}
			if (b == 0)
			{
				return a;
			}
			t = a % b;
			while (t != 0)
			{
				a = b;
				b = t;
				t = a % b;
			}
			return b;

		}

		NLECOMMON_API void ReduceRational(const NLERational& ralIn, NLERational* pRalOut)
		{
			NLElonglong xgcd = GetGCD(ralIn.num, ralIn.den);
			pRalOut->den = ralIn.den / xgcd;
			pRalOut->num = ralIn.num / xgcd;
		}

		NLECOMMON_API void CutRational(const NLERational &ralIn, NLERational *pRalOut, unsigned int nBits /*= 2*/)
		{
			LONGLONG nCut = 1;
			for (int i = 0;i < nBits; i++)
			{
				nCut *= 10;
			}
			pRalOut->num = (LONGLONG)(nCut * (ralIn.num * 1.0 / ralIn.den));
			pRalOut->den = nCut;
			ReduceRational(*pRalOut, pRalOut);
		}
	}
}

