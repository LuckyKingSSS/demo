// GDIPlusHelper.h: interface for the CGDIPlusHelper class. 
// 
////////////////////////////////////////////////////////////////////// 

#if !defined(AFX_GDIPLUSHELPER_H__BD5F6266_5686_43E2_B146_5EA1217A56FE__INCLUDED_) 
#define AFX_GDIPLUSHELPER_H__BD5F6266_5686_43E2_B146_5EA1217A56FE__INCLUDED_ 

#if _MSC_VER > 1000 
#pragma once 
#endif // _MSC_VER > 1000 

#pragma warning(disable:4786)
#include <map>
#include "Regulator.h"

struct UserInfo 
{
	CString		strName;
};

class ImageEx;
class ImageHandler
{
public:
	ImageHandler() {}
	virtual ~ImageHandler() {}
	virtual void OnImageHandle() = NULL;
};

class ImageManager;
class ImageEx : public Image 
{ 
public: 
	ImageEx(IN LPCTSTR  sResourceType, IN LPCTSTR sResource); 
	ImageEx(const WCHAR* filename, BOOL useEmbeddedColorManagement = false); 
	ImageEx(const void * pBuf, const int nBufSize);

	~ImageEx(); 
public: 
	CSize	GetSize(); 

	bool	IsAnimatedGIF() { return m_nFrameCount > 1; } 
	void	SetPause(bool bPause); 
	bool	IsPaused() { return m_bPause; }  
	void	Destroy(); 
	void	LoadGIF(CString path);

	void	Initialize(); //protected

protected: 
	friend ImageManager;
	bool				TestForAnimatedGIF(); 	 
	bool				DrawFrameGIF(); 
	IStream*			m_pStream; 

	bool LoadFromBuffer(BYTE* pBuff, int nSize); 
	bool GetResource(LPCTSTR lpName, LPCTSTR lpType, void* pResource, int& nBufSize); 
	bool Load(LPCTSTR sResourceType, LPCTSTR sResource); 

	HINSTANCE		m_hInst; 
	UINT			m_nFrameCount; 
	UINT			m_nFramePosition; 
	bool			m_bIsInitialized; 
	bool			m_bPause; 
	PropertyItem*	m_pPropertyItem;
	Regulator<long> m_PauseTime;
	HGLOBAL hGlobal;
	int				m_Index;
}; 


typedef std::map<int, ImageEx*>	ImageMap;
class ImageManager
{
public:
	ImageManager();
	~ImageManager();
	void SetImageHandler(ImageHandler *pHandler) { m_pHandler = pHandler;}
	ImageEx * AddGifImage(int nKey, int nResouceID);
	ImageEx * GetImage(int nKey);
	void ResumeThread();
protected:
	static UINT WINAPI _ThreadAnimationProc(LPVOID pParam);
	void ThreadAnimation();
protected:
	ImageMap			m_ImageList;
	ImageMap			m_GifList;
	ImageHandler		*m_pHandler;
	HANDLE				m_hThread; 
	bool				m_bStop;
	Regulator<long>		m_PauseTime;
	CRITICAL_SECTION	m_Critical_Section;
};



#endif // !defined(AFX_GDIPLUSHELPER_H__BD5F6266_5686_43E2_B146_5EA1217A56FE__INCLUDED_)