#include "stdafx.h"

#ifdef _DEBUG
#pragma comment(lib,"CommonLibD.lib")
#pragma comment(lib,"FSCommonLibD.lib")
#pragma comment(lib,"FSMatterViewD.lib")
#pragma comment(lib,"FSTimelineViewD.lib")
#pragma comment(lib, "FSMediaPlayerViewD.lib")
#pragma comment(lib, "MediaPlayerViewD.lib")
#pragma comment(lib, "FSExportD.lib")
#pragma comment(lib, "UpdateRegisterD.lib")
#else
#pragma comment(lib,"CommonLib.lib")
#pragma comment(lib,"FSCommonLib.lib")
#pragma comment(lib,"FSMatterView.lib")
#pragma comment(lib,"FSTimelineView.lib")
#pragma comment(lib, "FSMediaPlayerView.lib")
#pragma comment(lib, "MediaPlayerView.lib")
#pragma comment(lib, "FSExport.lib")
#pragma comment(lib, "UpdateRegister.lib")
#endif // _DEBUG


#pragma comment(lib, "WS_Log.lib")
