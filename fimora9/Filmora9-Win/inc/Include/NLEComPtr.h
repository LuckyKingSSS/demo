#pragma  once

#ifndef _ASSERT

#ifdef _DEBUG
#include <assert.h>
#ifdef WIN32
#include "crtdbg.h"
#else
#define _ASSERT(x) assert(x)
#endif

#else //release

#define _ASSERT(x) ((void)0)

#endif //end #ifdef _DEBUG

#endif //end #ifndef NLE_ASSERT


#include <objbase.h>
#include "NLEType.h"
#include "NLEMacroDefine.h"
/*
	源自atlcomcli.h
*/

template <class T>
class NLEComPtr;

template <class T>
class NLEComPtrBase
{
protected:
	NLEComPtrBase() 
	{
		p = NULL;
	}
	NLEComPtrBase(_Inout_opt_ T* lp) 
	{
		p = lp;
		if (p != NULL)
			p->AddRef();
	}
	void Swap(NLEComPtrBase& other)
	{
		T* pTemp = p;
		p = other.p;
		other.p = pTemp;
	}
public:
	typedef T _PtrClass;
	~NLEComPtrBase() throw()
	{
		if (p)
			p->Release();
	}
	operator T*() const throw()
	{
		return p;
	}
	T& operator*() const
	{
		NLE_ASSERT(p != NULL);
		return *p;
	}
	//The _ASSERT on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&() throw()
	{
		NLE_ASSERT(p == NULL);
		return &p;
	}

	T* operator->() const throw()
	{
		NLE_ASSERT(p != NULL);
		return (T*)p;
	}

	bool operator!() const throw()
	{
		return (p == NULL);
	}
	bool operator<(_In_opt_ T* pT) const throw()
	{
		return p < pT;
	}
	bool operator!=(_In_opt_ T* pT) const
	{
		return !operator==(pT);
	}
	bool operator==(_In_opt_ T* pT) const throw()
	{
		return p == pT;
	}

	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}
	// Compare two objects for equivalence
	bool IsEqualObject(_Inout_opt_ IUnknown* pOther) throw()
	{
		if (p == NULL && pOther == NULL)
			return true;	// They are both NULL objects

		if (p == NULL || pOther == NULL)
			return false;	// One is NULL the other is not

		IUnknown *punk1 = NULL;
		IUnknown *punk2 = NULL;
		p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		bool result = punk1 == punk2;
        SAFE_RELEASE(punk1);
        SAFE_RELEASE(punk2);
        
        return result;
	}
	// Attach to an existing interface (does not AddRef)
	void Attach(_In_opt_ T* p2) throw()
	{
		if (p)
		{
			ULONG ref = p->Release();
			_ASSERT(ref != 0 || p2 != p);
		}
		p = p2;
	}
	// Detach the interface (does not Release)
	T* Detach() throw()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
	HRESULT CopyTo( T** ppT) throw()
	{
		_ASSERT(ppT != NULL);
		if (ppT == NULL)
			return E_POINTER;
		*ppT = p;
		if (p)
			p->AddRef();
		return S_OK;
	}
		
	HRESULT QueryInterface(REFIID iid,void** pp) const throw()
	{
		NLE_ASSERT(pp != NULL);
		return p->QueryInterface(iid, (void**)pp);
	}
	T* p;
};

template <class T>
class NLEComPtr :
	public NLEComPtrBase<T>
{
public:
	NLEComPtr() throw()
	{
	}
	NLEComPtr(_Inout_opt_ T* lp) throw() :
		NLEComPtrBase<T>(lp)
	{
	}
	NLEComPtr(_Inout_ const NLEComPtr<T>& lp) throw() :
		NLEComPtrBase<T>(lp.p)
	{
	}
	T* operator=(_Inout_opt_ T* lp) throw()
	{
		if (*this != lp)
		{
			NLEComPtr(lp).Swap(*this);
		}
		return *this;
	}
	template <typename Q>
	T* operator=(_Inout_ const NLEComPtr<Q>& lp) throw()
	{
		if (!IsEqualObject(lp))
		{
			NLE_ASSERT(FALSE);
			//return static_cast<T*>(AtlComQIPtrAssign((IUnknown**)&p, lp, __uuidof(T)));
		}
		return *this;
	}
	T* operator=(_Inout_ const NLEComPtr<T>& lp) throw()
	{
		if (*this != lp)
		{
			NLEComPtr(lp).Swap(*this);
		}
		return *this;
	}
	NLEComPtr(_Inout_ NLEComPtr<T>&& lp) throw() :
		NLEComPtrBase<T>()
	{
			lp.Swap(*this);
		}
	T* operator=(_Inout_ NLEComPtr<T>&& lp) throw()
	{
		if (*this != lp)
		{
			NLEComPtr(static_cast<NLEComPtr&&>(lp)).Swap(*this);
		}
		return *this;
	}
};