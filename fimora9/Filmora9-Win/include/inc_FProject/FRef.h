#ifndef FREF_H
#define FREF_H

#include "inc_FProject/fproject_global.h"
#include "INLETimelineBase.h"


class FPROJECT_EXPORT FRef
{
public:
    virtual STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
    virtual STDMETHODIMP_(ULONG) AddRef();
    virtual STDMETHODIMP_(ULONG) Release();

    void PrintRef();
    int GetRef();

public:
    FRef();
    virtual ~FRef();
private:
    int m_ref;

};


#endif // FNLETIMELINE_H
