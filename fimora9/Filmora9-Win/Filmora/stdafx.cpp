#include "stdafx.h"



#ifdef _DEBUG
#pragma comment(lib,"FCommonLibD.lib")
#pragma comment(lib,"FPropertyPanelD.lib")
#pragma comment(lib, "FProjectD.lib")
#pragma comment(lib, "FTimelineViewD.lib")

#else
#pragma comment(lib,"FCommonLibD.lib")
#pragma comment(lib,"FPropertyPanel.lib")
#pragma comment(lib, "FProject.lib")
#pragma comment(lib, "FTimelineView.lib")

#endif // _DEBUG

