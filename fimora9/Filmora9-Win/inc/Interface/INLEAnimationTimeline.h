#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

// {2B28A956-520A-4C67-87B8-EC91DAB0E4EA}
DEFINE_GUID(CLSID_NLEAnimationTimeline, 0x2b28a956, 0x520a, 0x4c67, 0x87, 0xb8, 0xec, 0x91, 0xda, 0xb0, 0xe4, 0xea);
// {9D7127FF-DA82-4E4F-B546-E98E79265474}
DEFINE_GUID(IID_INLEAnimationTimeline, 0x9d7127ff, 0xda82, 0x4e4f, 0xb5, 0x46, 0xe9, 0x8e, 0x79, 0x26, 0x54, 0x74);

class INLEAnimationTimeline :public IUnknown
{
public:
	
};                                                                              


