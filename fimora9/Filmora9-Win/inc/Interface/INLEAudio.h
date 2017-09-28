#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                                                
//{90FDFBF8-261F-6570-7019-7AA34625FF9C}
DEFINE_GUID(CLSID_NLEAudio, 0x90FDFBF8, 0x261F, 0x6570, 0x70, 0x19, 0x7A, 0xA3, 0x46, 0x25, 0xFF, 0x9C);                   
//{BFB70CEF-EDCE-3D3A-1D83-05603D72F8D9}
DEFINE_GUID(IID_INLEAudio, 0xBFB70CEF, 0xEDCE, 0x3D3A, 0x1D, 0x83, 0x05, 0x60, 0x3D, 0x72, 0xF8, 0xD9);           
/**@ingroup GroupTimeline
 *@brief 音频Timeline，目前定义保留
 */
class INLEAudio :public IUnknown
{
public:
	/**
	*@brief function
	*@param[in] x input 1
	*@param[out] py output 1
	*@return result
	*/
	virtual HRESULT STDMETHODCALLTYPE InterfaceExample(int x, int *py) = 0;
};                                                                              
