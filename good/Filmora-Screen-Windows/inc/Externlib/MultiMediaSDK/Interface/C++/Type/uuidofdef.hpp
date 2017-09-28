/*
*******************************************************************************

    uuidofdef.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_UUIDOF_DEF_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_UUIDOF_DEF_HPP

#include "typedef.hpp"
#include "guidprc.hpp"

template< class _Tp > struct uuidof_class
{
	static const GUID & get_guid()
	{
		return GUID_NULL;
	}
};

#define	type_bind_guid( tp, gd )				\
template<> struct uuidof_class< tp >				\
{								\
	static GUID make_guid( LPOLESTR psz )			\
	{							\
		GUID result;					\
								\
		CLSIDFromString( psz, &result );		\
								\
		return result;					\
	}							\
								\
	static const GUID get_guid()				\
	{							\
		static GUID _S_gd = make_guid( L##gd );		\
								\
		return _S_gd;					\
	}							\
}

#ifndef	_MSC_VER

#define	__uuidof( tp )	uuidof_class< tp >::get_guid()

#endif	/* _MSC_VER */

#endif	/* _MACRO_WONDERSHARE_LIBRARY_UUIDOF_DEF_HPP */
