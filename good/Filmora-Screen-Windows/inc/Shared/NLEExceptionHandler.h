/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  NLEExceptionHandler.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    6/23/2016 10:55
 */

#ifndef _H_NLEEXCEPTIONHANDLER_H_
#define _H_NLEEXCEPTIONHANDLER_H_

#ifdef WIN32

#include <excpt.h>

#define __TRAY __try
#define __CATCH __except(EXCEPTION_EXECUTE_HANDLER)

#else 

#define __TRAY try
#define __CATCH catch(...)

#endif



#endif //_H_NLEEXCEPTIONHANDLER_H_


