
#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>   
                                                                                
//{9F7646F9-1252-5852-B500-0C9BE2B0A1E1}
DEFINE_GUID(CLSID_NLEAudioFilter, 0x9F7646F9, 0x1252, 0x5852, 0xB5, 0x00, 0x0C, 0x9B, 0xE2, 0xB0, 0xA1, 0xE1);                   
//{0F3B768A-C0A9-C77D-61B2-1F9CDC9854E1}
DEFINE_GUID(IID_INLEAudioFilter, 0x0F3B768A, 0xC0A9, 0xC77D, 0x61, 0xB2, 0x1F, 0x9C, 0xDC, 0x98, 0x54, 0xE1);     
/**@ingroup GroupTimeline
 *@brief 音频Timeline 
 *@{
 */

/**
 *@brief 音频Timeline
 */
class INLEAudioFilter :public IUnknown
{
public:
	virtual INT __stdcall GetFilterPropertyCount() = 0;
	virtual HRESULT __stdcall GetFilterProperty(int nIndex, INLEProperties **pProperties) = 0;
};                                                                             

/**
 *@}
 */