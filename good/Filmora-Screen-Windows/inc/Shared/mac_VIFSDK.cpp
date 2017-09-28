//
//  mac_VIFSDK.cpp
//  NLEPlatformPro
//
//  Created by admin on 16/6/7.
//  Copyright (c) 2016年 wondershare. All rights reserved.
//

#include <stdio.h>
#include "NLEMacroDefine.h"
#include "VIFSDK.h"

#include "scombase.h"

//mac版暂未实现，先给一个空实现
namespace VIFSDK {
    
    const char VIFSDK_API *kEffectCurrentTime = "kEffectCurrentTime";
    const char VIFSDK_API *kEffectTotalTime = "kEffectTotalTime";
    const char VIFSDK_API *kEffectFrameRate = "kEffectFrameRate";
    
    
    class CEffectBaseParam
    {
    public:
        CEffectBaseParam(){}
        ~CEffectBaseParam(){}
    };
    
    
    
    class CEffectImage : public scombase::CUnknown
    {
    public:
        DECLARE_IUNKNOWN
        
        CEffectImage(int width, int height, VIFormat format)
        {
            m_width = width;
            m_height = height;
            m_nSize = width * height * 4;
            
            m_pData = new uint8_t[m_nSize];
        }
        
        ~CEffectImage()
        {
            delete m_pData;
        }
        
        VIRESULT GetWidth(unsigned int *pWidth)
        {
            *pWidth = m_width;
            
            return VIRT_OK;
        }
        
        VIRESULT GetHeight(unsigned int *pHeight)
        {
            *pHeight = m_height;
            return VIRT_OK;
        }
        
        VIRESULT LockData(uint8_t **pData)
        {
            *pData = m_pData;
            
            return VIRT_OK;
        }
        
        VIRESULT UnlockData()
        {
            return VIRT_OK;
        }
        
    public:
        unsigned int m_width;
        unsigned int m_height;
        uint8_t *m_pData;
        unsigned int m_nSize;
    };
    
    
    class CSimpleEffect : public scombase::CUnknown
    {
    public:
        DECLARE_IUNKNOWN
        
        CSimpleEffect(const char *filterId, const int nWidth, const int nHeight)
        {
            m_pOutImage = new IImage(nWidth, nHeight, 0);
            m_pOutImage->AddRef();
            
            m_pInImage = NULL;
        }
        ~CSimpleEffect()
        {
            SAFE_RELEASE(m_pInImage);
            SAFE_RELEASE(m_pOutImage);

        }
        
        VIRESULT SetInputImage(IImage *pImage, unsigned int nOrder = 0)
        {
            m_pInImage = pImage;
            
            return VIRT_OK;
        }
        
        VIRESULT DoFilter(IImage **ppImage)
        {
            memcpy(m_pOutImage->m_pData, m_pInImage->m_pData, m_pInImage->m_nSize);
            
            *ppImage = m_pOutImage;
            
            return VIRT_OK;
        }
        
        VIRESULT SetInputFilter(IFilter *pFrom, int order)
        {
            NLE_ASSERT(0);
            return VIRT_OK;
        }
        
    private:
        IImage *m_pOutImage;
        IImage *m_pInImage;
    };
    
    
    
    VIRESULT VIFSDK_API InitializeEngine(const char* pszResPath)
    {
        return VIRT_OK;
    }
    
    VIRESULT VIFSDK_API UninitializeEngine()
    {
        return VIRT_OK;
    }
    
    
    VIRESULT VIFSDK_API CreateImage(const VICreateArgs args, IImage **ppImage)
    {
        *ppImage = new IImage(args.nWidth, args.nHeight, args.nFormat);
        (*ppImage)->AddRef();
        
        return VIRT_OK;
    }
    
    
    VIRESULT VIFSDK_API CreateImageFromFile(const char* imageFile, const VICreateArgs args, IImage **ppImage)
    {
        NLE_ASSERT(0);
        return VIRT_OK;
    }
    
    
    VIRESULT VIFSDK_API GetImageWidth(IImage *pImage, unsigned int *pWidth)
    {
        return pImage->GetWidth(pWidth);
    }
    
    
    VIRESULT VIFSDK_API GetImageHeight(IImage *pImage, unsigned int *pHeight)
    {
        return pImage->GetHeight(pHeight);
    }
    
    
    VIRESULT VIFSDK_API LockImageData(IImage *pImage, unsigned char **ppData)
    {
        return pImage->LockData(ppData);
    }
    
    
    VIRESULT VIFSDK_API UnlockImageData(IImage *pImage)
    {
        return pImage->UnlockData();
    }
    
    
    VIRESULT VIFSDK_API ReleaseImage(IImage *pImage)
    {
        pImage->Release();
        
        return VIRT_OK;
    }
    
    VIRESULT VIFSDK_API SaveImage(IImage *pImage, const char* savePath)
    {
        NLE_ASSERT(0);
        return VIRT_OK;
    }
    
 
    VIRESULT VIFSDK_API GetFilter(const char *filterId, const int nWidth, const int nHeight, IFilter **ppFilter)
    {
        IFilter *p = new IFilter(filterId, nWidth, nHeight);
        p->AddRef();
        
        *ppFilter = p;
        
        return VIRT_OK;
    }
    
    VIRESULT VIFSDK_API GetFilterParam(IFilter *pFilter, IParam **ppParam)
    {
        NLE_ASSERT(0);
        return VIRT_OK;
    }
    
    
    VIRESULT VIFSDK_API setParamIntValue(IParam *pParam, const char* name, const int value)
    {
        NLE_ASSERT(0);
        return VIRT_OK;
    }
    
    
    VIRESULT VIFSDK_API setParamFloatValue(IParam *pParam, const char* name, const float value)
    {
        NLE_ASSERT(0);
        return VIRT_OK;
    }
    
    
    VIRESULT VIFSDK_API setParamIntArrayValue(IParam *pParam, const char* name, const int *pValue, const int count)
    {
        NLE_ASSERT(0);
        return VIRT_OK;
    }
    
    
    VIRESULT VIFSDK_API setParamFloatArrayValue(IParam *pParam, const char* name, const float *pValue, const int count)
    {
        NLE_ASSERT(0);
        return VIRT_OK;
    }
    
    
    VIRESULT VIFSDK_API SetInputImage(IFilter *pFilter, IImage *pImage, unsigned int nOrder)
    {
        return pFilter->SetInputImage(pImage, nOrder);
    }
    
    VIRESULT VIFSDK_API ConnectFilter(IFilter *pFrom, IFilter *pTo, unsigned int nOrder)
    {
        return pTo->SetInputFilter(pFrom, nOrder);
    }
    
    VIRESULT VIFSDK_API DoFilter(IFilter *pFilter, IImage **ppImage)
    {
        return pFilter->DoFilter(ppImage);
    }
    
    
    VIRESULT VIFSDK_API ReleaseFilter(IFilter *pFilter)
    {
        pFilter->Release();
        
        return VIRT_OK;
    }
    
}