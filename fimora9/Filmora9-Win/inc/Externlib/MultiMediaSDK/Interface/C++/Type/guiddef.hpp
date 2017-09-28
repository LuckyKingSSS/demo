/*
*******************************************************************************

    guiddef.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_GUID_DEF_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_GUID_DEF_HPP

#ifndef	_WIN32

#ifndef	GUID_DEFINED
#define	GUID_DEFINED

typedef struct _GUID
{
	unsigned int	Data1;
	unsigned short	Data2;
	unsigned short	Data3;
	unsigned char	Data4[ 8 ];
} GUID;

#endif	/* GUID_DEFINED */

#ifndef	IID_DEFINED
#define	IID_DEFINED

typedef GUID		IID;
typedef IID *		LPIID;
typedef GUID		CLSID;
typedef CLSID *		LPCLSID;

#ifndef	REFGUID_DEFINED
#define	REFGUID_DEFINED
#ifdef	__cplusplus
#define REFGUID		const GUID &
#else
#define REFGUID		const GUID *
#endif
#endif

#ifndef	REFIID_DEFINED
#define	REFIID_DEFINED
#ifdef	__cplusplus
#define REFIID		const IID &
#else
#define REFIID		const IID *
#endif
#endif

#ifndef	REFCLSID_DEFINED
#define	REFCLSID_DEFINED
#ifdef	__cplusplus
#define	REFCLSID	const CLSID &
#else
#define	REFCLSID	const CLSID *
#endif
#endif

#endif	/* IID_DEFINED */

#ifndef	__cplusplus

#define	IsEqualGUID ( gid1, gid2 )	( !  memcmp( gid1, gid2, sizeof( GUID ) ) )
#define	IsEqualIID  ( iid1, iid2 )	IsEqualGUID( iid1, iid2 )
#define IsEqualCLSID( cid1, cid2 )	IsEqualGUID( cid1, cid2 )

#else

inline int IsEqualGUID( REFGUID gid1, REFGUID gid2 )
{
	return ! memcmp( &gid1, &gid2, sizeof( GUID ) );
}

inline int IsEqualIID( REFIID iid1, REFIID iid2 )
{
	return IsEqualGUID( iid1, iid2 );
}

inline int IsEqualCLSID( REFCLSID cid1, REFCLSID cid2 )
{
	return IsEqualGUID( cid1, cid2 );
}

inline bool operator == ( REFGUID gid1, REFGUID gid2 )
{
	return IsEqualGUID( gid1, gid2 ) != 0;
}

inline bool operator != ( REFGUID gid1, REFGUID gid2 )
{
	return IsEqualGUID( gid1, gid2 ) == 0;
}

#endif	/* __cplusplus */

#endif	/* _WIN32 */

#endif	/* _MACRO_WONDERSHARE_LIBRARY_GUID_DEF_HPP */
