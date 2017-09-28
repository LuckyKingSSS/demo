#ifndef COMMONLIB_GLOBAL_H
#define COMMONLIB_GLOBAL_H

//#include <QtCore/QtCore>
//#include <QtCore/qglobal.h>


#ifdef WIN32

#ifdef COMMONLIB_LIB
#define COMMONLIB_EXPORT __declspec(dllexport)
#else
#define COMMONLIB_EXPORT __declspec(dllimport)
#endif

#else
#define COMMONLIB_EXPORT  
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#endif

#endif // COMMONLIB_GLOBAL_H
