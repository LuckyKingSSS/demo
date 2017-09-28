#ifndef FPROJECT_GLOBAL_H
#define FPROJECT_GLOBAL_H

#include <QtCore/qglobal.h>
//#include <QtCore/QtCore>


#ifdef FPROJECT_LIB
# define FPROJECT_EXPORT Q_DECL_EXPORT
#else
# define FPROJECT_EXPORT Q_DECL_IMPORT
#endif

#endif // FPROJECT_GLOBAL_H
