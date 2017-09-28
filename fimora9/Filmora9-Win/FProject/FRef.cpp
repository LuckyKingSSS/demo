#include "inc_FProject/FRef.h"

#include <QDebug>

void FRef::PrintRef()
{
    qDebug() << "Ref:" << m_ref;
}

int FRef::GetRef()
{
    return m_ref;
}

FRef::FRef()
{
    m_ref = 0;
}

FRef::~FRef()
{

}


STDMETHODIMP FRef::QueryInterface(REFIID riid, void **ppv)
{
    (*ppv) = this;
    return S_OK;
}


STDMETHODIMP_(ULONG) FRef::Release()
{
    if (--m_ref == 0)
    {
        delete this;
        return 0;
    }
    return m_ref;
}

STDMETHODIMP_(ULONG) FRef::AddRef()
{
    return ++m_ref;

}
