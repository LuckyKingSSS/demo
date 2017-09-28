#pragma once

#ifdef DEFINE_KEY
#undef DEFINE_KEY
#endif

#ifdef INITKEY
#define DEFINE_KEY(name, str) \
        EXTERN_C const WCHAR*  name = str
#else
#define DEFINE_KEY(name,str) \
    EXTERN_C const WCHAR* FAR name

#endif // INITKEY

