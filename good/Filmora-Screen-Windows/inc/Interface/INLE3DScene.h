#pragma once

#ifndef _H_INLE3DSCENE_H_
#define _H_INLE3DSCENE_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>   

// {7FA7A2D0-3687-471F-9F7D-DA6204593B54}
DEFINE_GUID(CLSID_NLE3DScene, 0x7fa7a2d0, 0x3687, 0x471f, 0x9f, 0x7d, 0xda, 0x62, 0x4, 0x59, 0x3b, 0x54);
// {9DC91595-B15D-4393-9A13-FA1A90EE9737}
DEFINE_GUID(IID_INLE3DScene, 0x9dc91595, 0xb15d, 0x4393, 0x9a, 0x13, 0xfa, 0x1a, 0x90, 0xee, 0x97, 0x37);

/**@ingroup GroupTimeline
*@brief 3D场景化编辑组件
*@{
*/

/**
*@brief 3D场景化编辑组件
*/
class INLE3DScene :public IUnknown
{
public:
};

/**
*@}
*/

#endif // _H_INLE3DSCENE_H_