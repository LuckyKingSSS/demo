#include "stdafx.h"

#ifdef _DEBUG
#pragma comment(lib,"CommonLibD.lib")
#pragma comment(lib, "FCommonLibD.lib")
#pragma comment(lib, "FProjectD.lib")

#else
#pragma comment(lib,"CommonLib.lib")
#pragma comment(lib, "FCommonLibD.lib")
#pragma comment(lib, "FProject.lib")
#endif // _DEBUG
