#pragma once

#ifndef _H_INLEMEDIA_H_
#define _H_INLEMEDIA_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                                                
//{9B42D098-1E44-6B5F-C3DA-23CB57969A0B}
DEFINE_GUID(CLSID_NLEMedia, 0x9B42D098, 0x1E44, 0x6B5F, 0xC3, 0xDA, 0x23, 0xCB, 0x57, 0x96, 0x9A, 0x0B);                   
//{EF20DC11-73D0-E907-799B-838DFA0DFD39}
DEFINE_GUID(IID_INLEMedia, 0xEF20DC11, 0x73D0, 0xE907, 0x79, 0x9B, 0x83, 0x8D, 0xFA, 0x0D, 0xFD, 0x39);                         
class INLEMedia :public IUnknown
{
public:	
};                                                                         


#endif //_H_INLEMEDIA_H_


