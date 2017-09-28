/*
******************************************************************************

    CIconv.hpp

    Programmer:		Hetao

    Remark:

******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_POSIX_LIBRARY_CLASS_ICONV_HPP
#define	_MACRO_PRIVATE_POSIX_LIBRARY_CLASS_ICONV_HPP

#ifndef	_WIN32

#include <iconv.h>
#include <wchar.h>
#include <string.h>

namespace libPrvPosix
{

class CIconv
{
public:

	CIconv()
	{
		mContext = ( iconv_t )-1;
	}

	CIconv( const char * from, const char * to )
	{
		mContext = iconv_open( to, from );
	}

	~CIconv()
	{
		if_close( check_context() );
	}

private:

	CIconv( const CIconv & );

	CIconv & operator = ( const CIconv & );

public:

	void close()
	{
		if_close( check_context() );

		mContext = ( iconv_t )-1;
	}

	int conv( const char ** src, size_t * srcsz, char ** dst, size_t * dstsz ) const
	{
		return iconv( mContext, (char **)src, srcsz, dst, dstsz );
	}

	int conv( const char * src, size_t srcsz, char * dst, size_t dstsz ) const
	{
		return conv( &src, &srcsz, &dst, &dstsz );
	}

	int conv( char * src, wchar_t * dst, size_t maxsz ) const
	{
		return conv( src, ( strlen( src ) + 1 ) * sizeof( char ), ( char * )dst, maxsz * sizeof( wchar_t ) );
	}

	int conv( wchar_t * src, char * dst, size_t maxsz ) const
	{
		return conv( ( char * )src, ( wcslen( src ) + 1 ) * sizeof( wchar_t ), dst, maxsz * sizeof( char ) );
	}

	iconv_t get() const
	{
		return mContext;
	}

	bool open( const char * from, const char * to )
	{
		if_close( check_context() );

		mContext = iconv_open( to, from );

		return check_context();
	}

public:

	bool operator ! () const
	{
		return ! check_context();
	}

protected:

	bool check_context() const
	{
		return mContext != ( iconv_t )-1;
	}

	void if_close( bool exp )
	{
		if ( exp ) iconv_close( mContext );
	}

private:

	iconv_t	mContext;
};

}

#endif	/* _WIN32 */

#endif	/* _MACRO_PRIVATE_POSIX_LIBRARY_CLASS_ICONV_HPP */
