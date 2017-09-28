#pragma once
#include "NLEConfig.h"

#ifdef NLE_BASE_CLASS_EXPORTS
#define BASE_CLASS_EXPORT NLE_DECLARE_EXPORT
#else
#define BASE_CLASS_EXPORT NLE_DECLARE_IMPORT
#endif

