#ifndef FMATTERVIEW_GLOBAL_H
#define FMATTERVIEW_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef FMATTERVIEW_LIB
# define FMATTERVIEW_EXPORT Q_DECL_EXPORT
#else
# define FMATTERVIEW_EXPORT Q_DECL_IMPORT
#endif

#endif // FMATTERVIEW_GLOBAL_H