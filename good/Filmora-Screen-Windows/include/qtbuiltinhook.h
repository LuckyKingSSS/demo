/**********************************************************************
** Copyright (C) 2005 froglogic GmbH.
**
** This file is part of Squish.
**
** The following licensing terms and conditions constitute an
** exception from the standard Squish License Agreement and are
** incorporated therein by this reference. The purpose of this
** exception is to permit third parties the inclusion and compilation
** of this header file into such parties' proprietary programs.
**
** This software is provided 'as-is', without any express or implied
** warranty. In no event will the authors be held liable for any damages
** arising from the use of this software.
**
** Permission is granted to anyone to use this software for any purpose,
** including commercial applications, and to alter it and redistribute it
** freely, subject to the following restrictions:
**
** 1. The origin of this software must not be misrepresented; you must not
**    claim that you wrote the original software.
** 2. Altered source versions must be plainly marked as such, and must not be
**    misrepresented as being the original software.
** 3. This notice may not be removed or altered from any source distribution.
**
** Contact contact@froglogic.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef QTBUILTINHOOK_H
#define QTBUILTINHOOK_H

#if !defined(SQUISH_QT_AS_STATICLIB)

#include <qdir.h>
#include <qstring.h>
#include <qlibrary.h>
#include <stdlib.h>

#if defined(Q_OS_ANDROID)
#  include <android/log.h>
#endif


#if defined(Q_OS_ANDROID)
#  define SQ_HANDLE_LIB_ERROR( cond, lib ) \
    if ( cond ) { \
        __android_log_write( ANDROID_LOG_WARN, "Squish", qPrintable( lib.errorString() ) ); \
        return 0; \
    }
#elif QT_VERSION >= 0x040200
#  define SQ_HANDLE_LIB_ERROR( cond, lib ) \
    if ( cond ) { \
        qWarning( "%s", qPrintable( lib.errorString() ) ); \
        return 0; \
    }
#else
#  define SQ_HANDLE_LIB_ERROR( cond, lib ) \
    if ( cond ) { \
        return 0; \
    }
#endif


namespace Squish {
    typedef bool (*InitBuiltinHookFunc)( unsigned short int attachPort );

    static QString setupSquishPrefix()
    {
#if defined(SQUISH_INSTALL_PREFIX)
        static const char *fixedPrefix = SQUISH_INSTALL_PREFIX;
        qDebug( "Setting SQUISH_PREFIX to '%s'", fixedPrefix );
        setenv( "SQUISH_PREFIX", fixedPrefix, 0 );
        return QString::fromLatin1( fixedPrefix );
#elif defined(Q_OS_ANDROID)
        // Locate Squish based on the app installation directory
        QDir squishDir( QDir::homePath() );
        squishDir.cdUp();
        const QString squishPath = squishDir.canonicalPath();
        const QByteArray basePath = squishPath.toLocal8Bit();
        __android_log_print( ANDROID_LOG_INFO, "Squish",
                "Setting SQUISH_PREFIX to '%s'", basePath.constData() );
        qputenv( "SQUISH_PREFIX", basePath.constData() );
        return squishPath;
#elif defined(Q_OS_WINCE)
        // If there is no squish subfolder, this function is a noop on WinCE.
        const QString wcePrefix( "squish" );
        return QDir( wcePrefix ).exists()
            ? wcePrefix
            : QString();
#else
        // If the SQUISH_PREFIX variable is not set, this function is a noop.
        // This makes it possible to leave this code in the final AUT.
	return QString::fromLocal8Bit( getenv( "SQUISH_PREFIX" ) );
#endif
    }

    static QString pathForSquishLib( const QString &prefix, const char *libName )
    {
#if defined(Q_OS_WIN)
        static const char *libDirPart = "/bin/";
#else
        static const char *libDirPart = "/lib/";
#endif

        return prefix + QString::fromLatin1( libDirPart ) + QString::fromLatin1( libName );
    }


    static bool loadQtWrapperDependencies( const QString &prefix )
    {
#if defined(Q_OS_ANDROID) || defined(Q_OS_WINCE)
        // Load qtwrapper dependencies explicitly where it is needed.
        // On Android there's lack of a (working) LD_LIBRARY_PATH and on
        // WinCE only dlls from the system and application folder are
        // loaded automatically.
        QLibrary hookLib( pathForSquishLib( prefix, "squishhook" ) );
        SQ_HANDLE_LIB_ERROR( !hookLib.load(), hookLib );
#else
        Q_UNUSED( prefix );
#endif

        return true;
    }


    static bool loadQtWrapperExtensionDependencies( const QString &prefix )
    {
#if (QT_VERSION >= 0x050000) && (defined(Q_OS_ANDROID) || defined(Q_OS_WINCE))
        // Explicitely load dependency of QtQuick and QtQuickWidgets support.
        // Failure to do so is not fatal, the AUT may not be using QtQuick.
        QLibrary qtquickcommonLib( pathForSquishLib( prefix, "squishqtquickcommon" ) );
        SQ_HANDLE_LIB_ERROR( !qtquickcommonLib.load(), qtquickcommonLib );
#else
        Q_UNUSED( prefix );
#endif

        return true;
    }


    static InitBuiltinHookFunc loadBuiltinhookLibAndResolve()
    {
#if defined(Q_OS_AIX)
        static const char *qtwrapperLibName = "libsquishqtwrapper.a";
#else
        static const char *qtwrapperLibName = "squishqtwrapper";
#endif

        const QString prefix = setupSquishPrefix();
        if ( prefix.isEmpty() ) {
            return 0;
        }

        if ( !loadQtWrapperDependencies( prefix ) ) {
            return 0;
        }

        QLibrary qtwrapperLib( pathForSquishLib( prefix, qtwrapperLibName ) );
#if QT_VERSION < 0x040000
        qtwrapperLib.setAutoUnload( false );
#endif
        SQ_HANDLE_LIB_ERROR( !qtwrapperLib.load(), qtwrapperLib );

        InitBuiltinHookFunc funcPtr = (InitBuiltinHookFunc)qtwrapperLib.resolve(
                "qtwrapper_initialize_builtin_hook" );
        SQ_HANDLE_LIB_ERROR( !funcPtr, qtwrapperLib );

        (void)loadQtWrapperExtensionDependencies( prefix );

        return funcPtr;
    }

    bool installBuiltinHook()
    {
        InitBuiltinHookFunc initFunc = loadBuiltinhookLibAndResolve();
        return initFunc ? (*initFunc)( 0 ) : false;
    }

    bool allowAttaching( unsigned short port )
    {
        InitBuiltinHookFunc initFunc = loadBuiltinhookLibAndResolve();
        return initFunc ? (*initFunc)( port ) : false;
    }

}

#else

#if !defined(SQUISH_INSTALL_PREFIX) && !defined(Q_OS_IOS)
#  define SQUISH_INSTALL_PREFIX ":/"
#endif

extern "C" bool qtwrapper_initialize_builtin_hook( unsigned short int port );

namespace Squish {
    bool installBuiltinHook()
    {
#ifdef SQUISH_INSTALL_PREFIX
        qputenv("SQUISH_PREFIX", SQUISH_INSTALL_PREFIX);
#endif
        return qtwrapper_initialize_builtin_hook( 0 );
    }

    bool allowAttaching( unsigned short port )
    {
#ifdef SQUISH_INSTALL_PREFIX
        qputenv("SQUISH_PREFIX", SQUISH_INSTALL_PREFIX);
#endif
        return qtwrapper_initialize_builtin_hook( port );
    }
}

#endif

#endif // QTBUILTINHOOK_H
