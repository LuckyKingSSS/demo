/*
******************************************************************************

    strconv.hpp

    Programmer:		Hetao

    Remark:

******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_POSIX_LIBRARY_STRCONV_HPP
#define	_MACRO_PRIVATE_POSIX_LIBRARY_STRCONV_HPP

#ifndef	_WIN32

#include "CIconv.hpp"

namespace libPrvPosix
{

inline bool UTF8toUCS4LE( wchar_t * buffer, size_t maxlen, char * source )
{
	CIconv cd( "UTF-8", "UCS-4LE" );

	if ( ! cd ) return false;

	return ! cd.conv( source, buffer, maxlen );
}

inline bool UCS4LEtoUTF8( char * buffer, size_t maxlen, wchar_t * source )
{
	CIconv cd( "UCS-4LE", "UTF-8" );

	if ( ! cd ) return false;

	return ! cd.conv( source, buffer, maxlen );
}

}

#endif	/* _WIN32 */

#endif	/* _MACRO_PRIVATE_POSIX_LIBRARY_STRCONV_HPP */
