// math_utility.h
//

#pragma once

#ifndef DEFAULT_PRECISION
	#define DEFAULT_PRECISION 0.001
#endif

#ifndef WIN32
#	define _abs64 abs
#endif


const double DefaultPrecision = DEFAULT_PRECISION;

// 比较两浮点数 
// 返回0表示两数相等，返回值小于0表示dLeft小于dRight，返回值大于0表示dLeft大于dRight 
// Precision 为默认精度
__inline  int FloatCompare(double dLeft, double dRight, double dPreci = DefaultPrecision)
{
	double dDiff = dLeft - dRight;
	if (dDiff >= dPreci)
		return 1;
	else if (dDiff <= (0 - dPreci))
		return -1;

	return 0;
}



