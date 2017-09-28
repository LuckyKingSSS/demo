/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  NLECyclebuffer.h
 *@brief                                
 *@version 1.0                          
 *@author  yaoyj                       
 *@date    3/29/2016 16:45
 */
#ifndef _H_NLECYCLEBUFFER_H_
#define _H_NLECYCLEBUFFER_H_

#include <stdint.h>
#include "NLESingleLock.h"
#include "NLEMacroDefine.h"

class CNLECycleBuffer
{
public:
    CNLECycleBuffer()
    {
        pBuffer = NULL;
        nBufferSize = 0;
        pRead = NULL;
        pWrite = NULL;
        nDataSize = 0;
    }
    virtual ~CNLECycleBuffer()
    {
        clear();

        CNLESingleLock lock(&mutex);
        SAFE_DELETE(pBuffer);
    }
    void clear()
    {
        CNLESingleLock lock(&mutex);
        pRead = pBuffer;
        pWrite = pBuffer;
        nDataSize = 0;
    }
	
    int Init(int nSize)
    {   
        CNLESingleLock lock(&mutex);
        if (nSize > nBufferSize)
        {
            SAFE_DELETE(pBuffer);
        }
        pBuffer = new uint8_t[nSize];
        NLE_ASSERT(pBuffer);

        nBufferSize = nSize;
        pRead = pBuffer;
        pWrite = pBuffer;
        nDataSize = 0;

        return S_OK;
    }

    BOOL Empty()
    {
        CNLESingleLock lock(&mutex);
        return nDataSize == 0;
    }

    BOOL Full()
    {
        CNLESingleLock lock(&mutex);
        return nDataSize == nBufferSize;
    }

    int LeftSpace()
    {
        CNLESingleLock lock(&mutex);
        return nBufferSize - nDataSize;
    }

    int ValidDataSize()
    {
        CNLESingleLock lock(&mutex);
        return nDataSize;
    }

    //返回>=0表示成功，<0时表示失败
	virtual HRESULT Push(uint8_t *pData, int nSize)
    {
        if (LeftSpace() < nSize)
        {
            return E_FAIL;
        }

        CNLESingleLock lock(&mutex);
        if (nSize > nBufferSize)
            return E_FAIL;

        int nTailSize = pBuffer + nBufferSize - pWrite;
        if (nTailSize >= nSize)
        {
            memcpy(pWrite, pData, nSize);
            pWrite += nSize;
            if (nTailSize == nSize)
            {
                pWrite = pBuffer;
            }
        }
        else
        {
            memcpy(pWrite, pData, nTailSize);

            pData += nTailSize;
            memcpy(pBuffer, pData, nSize - nTailSize);
            pWrite = pBuffer + nSize - nTailSize;
        }
        nDataSize += nSize;

        return S_OK;
    }


    virtual HRESULT Pop(uint8_t *pData, int nSize)
    {
        if (ValidDataSize() < nSize)
        {
            return E_FAIL;
        }

        CNLESingleLock lock(&mutex);
        if (nSize > nBufferSize)
        {
            return E_FAIL;
        }
        int nTailSize = pBuffer + nBufferSize - pRead;
        if (nTailSize >= nSize)
        {
            memcpy(pData, pRead, nSize);
            pRead += nSize;
            if (nTailSize == nSize)
            {
                pRead = pBuffer;
            }
        }
        else
        {
            memcpy(pData, pRead, nTailSize);

            pData += nTailSize;
            memcpy(pData, pBuffer, nSize - nTailSize);
            pRead = pBuffer + nSize - nTailSize;
        }
        nDataSize -= nSize;

        return S_OK;
    }

    int Size()
    {
        return nBufferSize;
    }

protected:
    CNLEMutex mutex;

    int nDataSize;
    uint8_t *pBuffer;
    uint8_t *pRead;
    uint8_t *pWrite;
    int nBufferSize;
};


#endif //_H_NLECYCLEBUFFER_H_
