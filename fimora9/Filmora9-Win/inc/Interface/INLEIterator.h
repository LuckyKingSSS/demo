/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEIterator.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    5/24/2016 15:33
 */

#ifndef _H_INLEITERATOR_H_
#define _H_INLEITERATOR_H_


#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                        
#include "NLEComPtr.h"
                                                                                
//{36B8225A-B5BA-9098-8969-9A9AAE4D329B}
DEFINE_GUID(CLSID_INLEIterator, 0x36B8225A, 0xB5BA, 0x9098, 0x89, 0x69, 0x9A, 0x9A, 0xAE, 0x4D, 0x32, 0x9B);                   
//{870911F8-2DF6-D1D2-19BD-1809E67EC8D8}
DEFINE_GUID(IID_INLEIterator, 0x870911F8, 0x2DF6, 0xD1D2, 0x19, 0xBD, 0x18, 0x09, 0xE6, 0x7E, 0xC8, 0xD8);                         
class INLEIterator :public IUnknown
{
public:

	virtual HRESULT  __stdcall MoveToBegin() = 0;
	virtual HRESULT  __stdcall MoveToNext() = 0;	
	virtual NLEComPtr<IUnknown> __stdcall Current() = 0;
};                                                                              

#endif //_H_INLEITERATOR_H_