#include "stdafx.h"
#include <QtPlugin>
#ifdef _DEBUG
#pragma comment(lib, "FSCommonLibD.lib")
#pragma comment(lib, "CommonLibD.lib")
#pragma comment(lib, "MediaPlayerViewD.lib")
#pragma comment(lib, "FSTimelineViewD.lib")
#else
#pragma comment(lib, "FSCommonLib.lib")
#pragma comment(lib, "CommonLib.lib")
#pragma comment(lib, "MediaPlayerView.lib")
#pragma comment(lib, "FSTimelineView.lib")
#endif
#pragma comment(lib, "NLEKey.lib")
//#pragma comment(lib, "NLEGUID.lib")
#pragma comment(lib, "NLECommon.lib")
