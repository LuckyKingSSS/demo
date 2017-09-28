#pragma once

#include <vector>
#include <algorithm>
#include "objbase.hpp"

// Filter Setup data structures no defined in axextend.idl

/* Version of IUnknown that is renamed to allow a class to support both
   non delegating and delegating IUnknowns in the same COM object */

#define CheckPointer(p,ret) {if((p)==NULL) return (ret);}
#define ValidateReadPtr(p,cb) 0
#define ValidateWritePtr(p,cb) 0
#define ValidateReadWritePtr(p,cb) 0
#define ValidateStringPtr(p) 0
#define ValidateStringPtrA(p) 0
#define ValidateStringPtrW(p) 0
#ifndef ASSERT
#define ASSERT(_x_) ((void)0)
#endif

#ifndef SAFE_ADDREF
#define SAFE_ADDREF(p) {if(p) { (p)->AddRef();}} 
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef SAFE_CLOSE
#define SAFE_CLOSE(p) { if(p) { CloseHandle((p)); (p)=NULL; } }
#endif

#ifndef SAFE_CLOSE_FILE
#define SAFE_CLOSE_FILE(p) { if(p) { fclose((p)); (p)=NULL; } }
#endif

#ifndef SAFE_ASSIGN
#define SAFE_ASSIGN
__inline void SafeAssign(IUnknown **ppLeft, IUnknown *pRight)
{
	if (ppLeft)
	{
		SAFE_RELEASE((*ppLeft)); 
		*ppLeft = pRight; 
		SAFE_ADDREF((*ppLeft));
	}
}
#endif

#ifndef JIF
#define JIF(x) \
	if (FAILED(hr=(x))) \
{ \
	return FALSE; \
}
#endif

#ifdef  _MSC_VER
#if _MSC_VER>=1100
#define AM_NOVTABLE __declspec(novtable)
#else
#define AM_NOVTABLE
#endif
#endif  // MSC_VER

namespace scombase
{
#ifndef DbgLog
#define DbgLog
#endif

enum {  LOG_TIMING = 0x01,    // Timing and performance measurements
        LOG_TRACE = 0x02,     // General step point call tracing
        LOG_MEMORY =  0x04,   // Memory and object allocation/destruction
        LOG_LOCKING = 0x08,   // Locking/unlocking of critical sections
        LOG_ERROR = 0x10,     // Debug error notification
        LOG_CUSTOM1 = 0x20,
        LOG_CUSTOM2 = 0x40,
        LOG_CUSTOM3 = 0x80,
        LOG_CUSTOM4 = 0x100,
        LOG_CUSTOM5 = 0x200,
};

#ifndef DECLARE_IUNKNOWN
#define DECLARE_IUNKNOWN                                        \
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv) {      \
        return GetOwner()->QueryInterface(riid,ppv);            \
    };                                                          \
    STDMETHODIMP_(ULONG) AddRef() {                             \
        return GetOwner()->AddRef();                            \
    };                                                          \
    STDMETHODIMP_(ULONG) Release() {                            \
        return GetOwner()->Release();                           \
    };
#endif

#ifndef INONDELEGATINGUNKNOWN_DEFINED
DECLARE_INTERFACE(INonDelegatingUnknown)
{
    STDMETHOD(NonDelegatingQueryInterface)( REFIID, LPVOID *) = 0;
    STDMETHOD_(ULONG, NonDelegatingAddRef)( void ) = 0;
    STDMETHOD_(ULONG, NonDelegatingRelease)( void ) = 0;
};
#define INONDELEGATINGUNKNOWN_DEFINED
#endif

typedef INonDelegatingUnknown *PNDUNKNOWN;


/* This is the base object class that supports active object counting. As
   part of the debug facilities we trace every time a C++ object is created
   or destroyed. The name of the object has to be passed up through the class
   derivation list during construction as you cannot call virtual functions
   in the constructor. The downside of all this is that every single object
   constructor has to take an object name parameter that describes it */

/* An object that supports one or more COM interfaces will be based on
   this class. It supports counting of total objects for DLLCanUnloadNow
   support, and an implementation of the core non delegating IUnknown */

class CUnknown : public INonDelegatingUnknown                 
{
private:
    const LPUNKNOWN m_pUnknown; /* Owner of this object */

protected:                      /* So we can override NonDelegatingRelease() */
    volatile LONG m_cRef;       /* Number of reference counts */

public:
    CUnknown()
		:m_cRef(0)
		,m_pUnknown(reinterpret_cast<LPUNKNOWN>( static_cast<PNDUNKNOWN>(this) ))
	{
		
	};
    CUnknown(LPUNKNOWN pUnk);
    virtual ~CUnknown() {};

    // This is redundant, just use the other constructor
    //   as we never touch the HRESULT in this anyway
    CUnknown( LPUNKNOWN pUnk, HRESULT *phr);
    /* Return the owner of this object */

    LPUNKNOWN GetOwner() const {
        return m_pUnknown;
    };

    /* Called from the class factory to create a new instance, it is
       pure virtual so it must be overriden in your derived class */

    /* static CUnknown *CreateInstance(LPUNKNOWN, HRESULT *) */

    /* Non delegating unknown implementation */

    STDMETHODIMP NonDelegatingQueryInterface(REFIID, void **);
    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
};

/* Return an interface pointer to a requesting client
   performing a thread safe AddRef as necessary */

HRESULT __stdcall GetInterface(LPUNKNOWN pUnk, void **ppv);

/* A function that can create a new COM object */

typedef CUnknown *(CALLBACK *LPFNNewCOMObject)( LPUNKNOWN pUnkOuter, HRESULT *phr);

/*  A function (can be NULL) which is called from the DLL entrypoint
    routine for each factory template:

    bLoading - TRUE on DLL load, FALSE on DLL unload
    rclsid   - the m_ClsID of the entry
*/
typedef void (CALLBACK *LPFNInitRoutine)(BOOL bLoading, const CLSID *rclsid);



template <class ItemClass, class ItemIntf, class ListIntf>
class CUnknownList : public CUnknown, public ListIntf
{
public:
	CUnknownList();
	~CUnknownList();

	DECLARE_IUNKNOWN

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv)
	{
		if (riid==__uuidof(ListIntf))
			return scombase::GetInterface((ListIntf *)this, ppv);
		else
			return scombase::CUnknown::NonDelegatingQueryInterface(riid, ppv);
	};

	STDMETHODIMP_(int) GetCount();
	STDMETHODIMP GetItem(int Index, ItemIntf** ppItem);
	STDMETHODIMP Clear();
	STDMETHODIMP Delete(int Index);
	STDMETHODIMP Add(ItemIntf ** ppItem);
	STDMETHODIMP Remove(ItemIntf* ppItem, int * pIndex);
	STDMETHODIMP_(int) Add(ItemClass* pItem);
	STDMETHODIMP_(ItemClass &) Add();
	ItemClass & STDMETHODCALLTYPE GetItem(int Index);
protected:
	typedef std::vector<ItemClass *> T_Vector;
	T_Vector m_list;	
};


template <class ItemClass, class ItemIntf, class ListIntf> 
	CUnknownList<ItemClass, ItemIntf, ListIntf>::CUnknownList()
{
}

template <class ItemClass, class ItemIntf, class ListIntf> 
	CUnknownList<ItemClass, ItemIntf, ListIntf>::~CUnknownList()
{
	Clear();
}

template <class ItemClass, class ItemIntf, class ListIntf> 
	STDMETHODIMP CUnknownList<ItemClass, ItemIntf, ListIntf>::Add(ItemIntf** ppItem)
{
	CheckPointer(ppItem, E_POINTER);
	ItemClass * pNewItem = new ItemClass(); 
	pNewItem->AddRef();
	m_list.insert(m_list.end(), pNewItem);
	pNewItem->AddRef();
	*ppItem = pNewItem;
	return S_OK;
}

template <class ItemClass, class ItemIntf, class ListIntf> 
	STDMETHODIMP CUnknownList<ItemClass, ItemIntf, ListIntf>::Delete(int Index)
{
	if ((Index>=0) && (Index<distance(m_list.begin(), m_list.end())))
	{
		SAFE_RELEASE(m_list[Index]);
		m_list.erase(m_list.begin()+Index);
		return S_OK;
	}
	else 
	{
		return S_FALSE;
	}
}

template <class ItemClass, class ItemIntf, class ListIntf> 
	STDMETHODIMP CUnknownList<ItemClass, ItemIntf, ListIntf>::Remove(ItemIntf* pItem, int * pIndex)
{
	CheckPointer(pIndex, E_POINTER);
	typename T_Vector::iterator pos = find(m_list.begin(), m_list.end(), pItem);
	if (pos!=m_list.end())
	{
		*pIndex = (int)distance(m_list.begin(), pos);
		m_list.erase(pos);
		pItem->Release();
		return S_OK;
	} else return S_FALSE;
}

template <class ItemClass, class ItemIntf, class ListIntf> 
	STDMETHODIMP CUnknownList<ItemClass, ItemIntf, ListIntf>::Clear()
{
	typename T_Vector::reverse_iterator itor;
	for (itor=m_list.rbegin();itor<m_list.rend();itor++)
	{
		ItemClass * pItem=*itor;
		SAFE_RELEASE(pItem);
	}
	m_list.clear();
	return S_OK;
}

template <class ItemClass, class ItemIntf, class ListIntf> 
	STDMETHODIMP_(int) CUnknownList<ItemClass, ItemIntf, ListIntf>::GetCount()
{
	return (int)m_list.size();
}

template <class ItemClass, class ItemIntf, class ListIntf> 
	STDMETHODIMP CUnknownList<ItemClass, ItemIntf, ListIntf>::GetItem(int Index, ItemIntf** ppItem)
{
	CheckPointer(ppItem, E_POINTER);
	*ppItem = m_list[Index];
	(*ppItem)->AddRef();
	return S_OK;
}

template <class ItemClass, class ItemIntf, class ListIntf> 
	ItemClass & STDMETHODCALLTYPE CUnknownList<ItemClass, ItemIntf, ListIntf>::GetItem(int Index)
{
	return *m_list[Index];
}

template <class ItemClass, class ItemIntf, class ListIntf> 
	int STDMETHODCALLTYPE CUnknownList<ItemClass, ItemIntf, ListIntf>::Add(ItemClass* pItem)
{
	int ret = (int)m_list.size();
	m_list.insert(m_list.end(), pItem);
	pItem->AddRef();
	return ret;
}

template <class ItemClass, class ItemIntf, class ListIntf> 
	ItemClass & STDMETHODCALLTYPE CUnknownList<ItemClass, ItemIntf, ListIntf>::Add()
{
	int ret = (int)m_list.size();
	ItemClass * pItem = new ItemClass();
	m_list.insert(m_list.end(), pItem);
	pItem->AddRef();
	return *pItem;
}
}

