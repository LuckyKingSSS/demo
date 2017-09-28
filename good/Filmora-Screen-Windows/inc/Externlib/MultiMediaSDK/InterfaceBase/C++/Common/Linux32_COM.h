#pragma once

typedef int HRESULT;

#define _HRESULT_TYPEDEF_(_sc)			((HRESULT)_sc)

#define S_OK							0
#define S_FALSE							1

#define E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80000001L)

//
// MessageId: E_OUTOFMEMORY
//
// MessageText:
//
// Ran out of memory
//
#define E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x80000002L)

//
// MessageId: E_INVALIDARG
//
// MessageText:
//
// One or more arguments are invalid
//
#define E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80000003L)

//
// MessageId: E_NOINTERFACE
//
// MessageText:
//
// No such interface supported
//
#define E_NOINTERFACE                    _HRESULT_TYPEDEF_(0x80000004L)

//
// MessageId: E_POINTER
//
// MessageText:
//
// Invalid pointer
//
#define E_POINTER                        _HRESULT_TYPEDEF_(0x80000005L)

//
// MessageId: E_HANDLE
//
// MessageText:
//
// Invalid handle
//
#define E_HANDLE                         _HRESULT_TYPEDEF_(0x80000006L)

//
// MessageId: E_ABORT
//
// MessageText:
//
// Operation aborted
//
#define E_ABORT                          _HRESULT_TYPEDEF_(0x80000007L)

//
// MessageId: E_FAIL
//
// MessageText:
//
// Unspecified error
//
#define E_FAIL                           _HRESULT_TYPEDEF_(0x80000008L)

//
// MessageId: E_ACCESSDENIED
//
// MessageText:
//
// General access denied error
//
#define E_ACCESSDENIED                   _HRESULT_TYPEDEF_(0x80000009L)

//
// Generic test for success on any status value (non-negative numbers
// indicate success).
//		
#define CLASS_E_NOAGGREGATION            _HRESULT_TYPEDEF_(0x80040110L)

#define CLASS_E_CLASSNOTAVAILABLE        _HRESULT_TYPEDEF_(0x80040111L)

#define   NOERROR   0


#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)

//
// and the inverse
//

#define FAILED(hr) (((HRESULT)(hr)) < 0)