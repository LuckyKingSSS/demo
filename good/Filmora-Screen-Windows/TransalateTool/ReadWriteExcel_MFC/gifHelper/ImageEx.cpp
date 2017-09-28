// GDIPlusHelper.cpp: implementation of the CGDIPlusHelper class. 
// 
////////////////////////////////////////////////////////////////////// 

#include "stdafx.h" 
#include "ImageEx.h" 
#include <process.h> 
#include "Resource.h"

int gIndex = 0;
#ifdef _DEBUG 
#undef THIS_FILE 
static char THIS_FILE[]=__FILE__; 
//#define new DEBUG_NEW 
#endif 

////////////////////////////////////////////////////////////////////// 
// Construction/Destruction 
////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	ImageEx::ImageEx 
//  
// DESCRIPTION:	Constructor for constructing images from a resource  
//  
// RETURNS:	 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
ImageEx::ImageEx(LPCTSTR sResourceType, LPCTSTR sResource):m_PauseTime(1)
{ 
	Initialize(); 

	if (Load(sResourceType, sResource)) 
	{ 

		nativeImage = NULL; 

		lastResult = DllExports::GdipLoadImageFromStreamICM(m_pStream, &nativeImage); 

		TestForAnimatedGIF();

		m_bPause = false;
	} 
} 



//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	ImageEx::ImageEx 
//  
// DESCRIPTION:	Constructor for constructing images from a file 
//  
// RETURNS:	 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
ImageEx::ImageEx(const WCHAR* filename, BOOL useEmbeddedColorManagement) : Image(filename, useEmbeddedColorManagement), m_PauseTime(1)
{ 
	Initialize(); 

	m_bIsInitialized = true; 

	TestForAnimatedGIF(); 
} 

ImageEx::ImageEx(const void * pBuf, const int nBufSize): m_PauseTime(1)
{
	Initialize();
	if (LoadFromBuffer((BYTE *)pBuf, nBufSize))
	{
		m_bIsInitialized = true;

		nativeImage = NULL; 

		lastResult = DllExports::GdipLoadImageFromStreamICM(m_pStream, &nativeImage); 

		TestForAnimatedGIF();
	}
}

//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	ImageEx::~ImageEx 
//  
// DESCRIPTION:	Free up fresources 
//  
// RETURNS:	 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
ImageEx::~ImageEx() 
{ 
	Destroy(); 
} 



//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	LoadFromBuffer 
//  
// DESCRIPTION:	Helper function to copy phyical memory from buffer a IStream 
//  
// RETURNS:	 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
bool ImageEx::LoadFromBuffer(BYTE* pBuff, int nSize) 
{ 
	bool bResult = false; 

	hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize); 
	if (hGlobal) 
	{ 
		void* pData = GlobalLock(hGlobal); 
		if (pData) 
			memcpy(pData, pBuff, nSize); 

		GlobalUnlock(hGlobal); 

		if (CreateStreamOnHGlobal(hGlobal, FALSE, &m_pStream) == S_OK) 
			bResult = true; 

	} 


	return bResult; 
} 

//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	GetResource 
//  
// DESCRIPTION:	Helper function to lock down resource 
//  
// RETURNS:	 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
bool ImageEx::GetResource(LPCTSTR lpName, LPCTSTR lpType, void* pResource, int& nBufSize) 
{  
	HRSRC		hResInfo; 
	HANDLE		hRes; 
	LPSTR		lpRes	= NULL;  
	int			nLen	= 0; 
	bool		bResult	= FALSE; 

	// Find the resource 

	hResInfo = FindResource(m_hInst , lpName, lpType); 
	if (hResInfo == NULL)  
	{ 
		DWORD dwErr = GetLastError(); 
		return false; 
	} 

	// Load the resource 
	hRes = LoadResource(m_hInst , hResInfo); 

	if (hRes == NULL)  
		return false; 

	// Lock the resource 
	lpRes = (char*)LockResource(hRes); 

	if (lpRes != NULL) 
	{  
		if (pResource == NULL) 
		{ 
			nBufSize = SizeofResource(m_hInst , hResInfo); 
			bResult = true; 
		} 
		else 
		{ 
			if (nBufSize >= (int)SizeofResource(m_hInst , hResInfo)) 
			{ 
				memcpy(pResource, lpRes, nBufSize); 
				bResult = true; 
			} 
		}  

		UnlockResource(hRes);   
	} 

	// Free the resource 
	FreeResource(hRes); 

	return bResult; 
} 


//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	Load 
//  
// DESCRIPTION:	Helper function to load resource from memory 
//  
// RETURNS:	 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
bool ImageEx::Load(LPCTSTR sResourceType, LPCTSTR sResource) 
{ 
	bool bResult = false; 


	BYTE*	pBuff = NULL; 
	int		nSize = 0; 
	if (GetResource(sResource/*sResource.GetBuffer(0)*/, sResourceType/*sResourceType.GetBuffer(0)*/, pBuff, nSize)) 
	{ 
		if (nSize > 0) 
		{ 
			pBuff = new BYTE[nSize]; 

			if (GetResource(sResource, sResourceType/*.GetBuffer(0)*/, pBuff, nSize)) 
			{ 
				if (LoadFromBuffer(pBuff, nSize)) 
				{ 

					bResult = true; 
				} 
			} 

			delete [] pBuff; 
		} 
	} 


	m_bIsInitialized = bResult; 

	return bResult; 
} 


//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	GetSize 
//  
// DESCRIPTION:	Returns Width and Height object 
//  
// RETURNS:	 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
CSize ImageEx::GetSize() 
{ 
	return CSize(GetWidth(), GetHeight()); 
} 


//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	TestForAnimatedGIF 
//  
// DESCRIPTION:	Check GIF/Image for avialability of animation 
//  
// RETURNS:	 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
bool ImageEx::TestForAnimatedGIF() 
{ 
	UINT count = 0; 
	count = GetFrameDimensionsCount(); 
	GUID* pDimensionIDs = new GUID[count]; 

	// Get the list of frame dimensions from the Image object. 
	GetFrameDimensionsList(pDimensionIDs, count); 

	// Get the number of frames in the first dimension. 
	m_nFrameCount = GetFrameCount(&pDimensionIDs[0]); 

	// Assume that the image has a property item of type PropertyItemEquipMake. 
	// Get the size of that property item. 
	int nSize = GetPropertyItemSize(PropertyTagFrameDelay); 

	// Allocate a buffer to receive the property item. 
	m_pPropertyItem = (PropertyItem*) malloc(nSize); 

	GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem); 


	delete[]  pDimensionIDs; 

	return m_nFrameCount > 1; 
} 


//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	ImageEx::Initialize 
//  
// DESCRIPTION:	Common function called from Constructors 
//  
// RETURNS:	 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
void ImageEx::Initialize() 
{ 
	m_pStream = NULL; 
	m_nFramePosition = 0; 
	m_nFrameCount = 0; 
	m_pStream = NULL; 
	lastResult = InvalidParameter; 
	m_bIsInitialized = false; 
	m_pPropertyItem = NULL; 
	m_nFramePosition = 0; 
	m_bPause = true;
	m_Index = gIndex++;

#ifdef INDIGO_CTRL_PROJECT 
	m_hInst = _Module.GetResourceInstance(); 
#else 
	m_hInst = AfxGetResourceHandle(); 
#endif 
} 

////////////////////////////////////////////////////////////////////////////////
//FUNCTION:    ImageEx::LoadGIF
//
//DESCRIPTION:  Load GIF
//
//RETURNS: void
//
//NOTES:
//
//MODIFICATIONS:
//
//Name				Date		Version		Comments
//ChangZiwen		20100422	1.0			Origin	
////////////////////////////////////////////////////////////////////////////////
void ImageEx::LoadGIF(CString path)
{
	SetPause(true);
	if(m_pStream)
		m_pStream->Release();

	if(m_pPropertyItem)
	{
		free(m_pPropertyItem);
	}

	GlobalFree(hGlobal);
	BYTE * pBuf = NULL;
	int nBufSize = 0;
	if (pBuf)
	{
		ImageEx((void *)pBuf, nBufSize);

		Initialize();
		if (LoadFromBuffer((BYTE *)pBuf, nBufSize))
		{
			m_bIsInitialized = true;

			delete nativeImage;
			nativeImage = NULL; 

			lastResult = DllExports::GdipLoadImageFromStreamICM(m_pStream, &nativeImage); 

			TestForAnimatedGIF();
		}
	}
	delete[] pBuf;
	SetPause(false);
}


//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	ImageEx::DrawFrameGIF 
//  
// DESCRIPTION:	 
//  
// RETURNS:	 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
bool ImageEx::DrawFrameGIF() 
{ 	
	if (m_bPause) return false;
	if (!m_PauseTime.isReady(GetTickCount())) return false;

	GUID pageGuid = FrameDimensionTime; 

	SelectActiveFrame(&pageGuid, m_nFramePosition++);		 


	if (m_nFramePosition >= m_nFrameCount) 
		m_nFramePosition = 0; 


	long lPause = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
	
	m_PauseTime.SetUpdatePeriod(lPause);
	return true; 
} 



//////////////////////////////////////////////////////////////////////////////// 
//  
// FUNCTION:	ImageEx::SetPause 
//  
// DESCRIPTION:	Toggle Pause state of GIF 
//  
// RETURNS:		 
//  
// NOTES:		 
//  
// MODIFICATIONS: 
//  
// Name				Date		Version		Comments 
// N T ALMOND       29012002	1.0			Origin 
//  
//////////////////////////////////////////////////////////////////////////////// 
void ImageEx::SetPause(bool bPause) 
{ 
	if (!IsAnimatedGIF()) 
		return; 

	m_bPause = bPause; 
} 


void ImageEx::Destroy() 
{ 
	free(m_pPropertyItem); 
	GlobalFree(hGlobal);

	m_pPropertyItem = NULL;  
	if (m_pStream) 
		m_pStream->Release();
}


ImageManager::ImageManager():m_pHandler(NULL), m_PauseTime(70)
{
	m_hThread = NULL;
	m_bStop = false;
	InitializeCriticalSection(&m_Critical_Section);
}


ImageManager::~ImageManager()
{
	m_bStop = false;
	EnterCriticalSection(&m_Critical_Section);
	for (ImageMap::iterator iter = m_ImageList.begin(); iter != m_ImageList.end(); )
	{
		if (iter->second != NULL)		
			delete iter->second;
		iter = m_ImageList.erase(iter);
	}
	LeaveCriticalSection(&m_Critical_Section);

	::DeleteCriticalSection(&m_Critical_Section);
}

ImageEx * ImageManager::AddGifImage(int nKey, int nResouceID)
{
	ImageEx *pImage = NULL;
	ImageMap::iterator iter = m_ImageList.find(nResouceID);
	if (iter == m_ImageList.end())
	{
		pImage = new ImageEx("GIF", MAKEINTRESOURCE(nResouceID));
		m_ImageList[nResouceID] = pImage;
	}
	else
		pImage = iter->second;

	if (m_hThread == NULL)
	{	
		m_hThread = (HANDLE) _beginthreadex( NULL, 0, _ThreadAnimationProc, this, 0, NULL);  
	}

	EnterCriticalSection(&m_Critical_Section);
	m_GifList[nKey] = pImage;
	LeaveCriticalSection(&m_Critical_Section);

	return NULL;
}

ImageEx * ImageManager::GetImage(int nKey)
{
	ImageMap::iterator iter = m_GifList.find(nKey);
	if (iter == m_GifList.end())
		return NULL;
	else
		return iter->second;
}

UINT WINAPI ImageManager::_ThreadAnimationProc(LPVOID pParam) 
{ 
	ASSERT(pParam); 
	ImageManager *pImageManager = reinterpret_cast<ImageManager *> (pParam); 
	
	while (pImageManager && !pImageManager->m_bStop)
	{
		pImageManager->ThreadAnimation();
	}
	return 0; 
} 
void ImageManager::ResumeThread()
{
	if (m_hThread != NULL)
	{
		::ResumeThread(m_hThread);
	}

}
void ImageManager::ThreadAnimation()
{
	if (!m_PauseTime.isReady(GetTickCount()))
	{
		Sleep(5);
		return;
	}
	
	EnterCriticalSection(&m_Critical_Section);

	try
	{
		for (ImageMap::iterator iter = m_ImageList.begin(); iter != m_ImageList.end(); iter++)
		{
			if (iter->second != NULL)
				iter->second->DrawFrameGIF();
		}
		
		if (m_pHandler != NULL)
		m_pHandler->OnImageHandle();
	}
	catch(...)
	{

	}


	LeaveCriticalSection(&m_Critical_Section);

	Sleep(5);
}