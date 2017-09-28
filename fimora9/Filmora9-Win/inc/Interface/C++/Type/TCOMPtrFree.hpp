/*
*******************************************************************************

    TCOMPtrFree.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_TEMPLATE_COM_PTR_FREE_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_TEMPLATE_COM_PTR_FREE_HPP

#define	MACRO_DECLARE_COM_PTR_AUTO_RELEASE( tp, ptr )	libWondershare::TCOMPtrFree< tp > _auto_free_##ptr( ptr )

namespace libWondershare
{

template< class _Tp >
class TCOMPtrFree
{
public:

	typedef	_Tp *	pointer;

	explicit TCOMPtrFree( pointer ptr ): _M_ptr( ptr )
	{
	}

	~TCOMPtrFree()
	{
		_M_ptr->Release();
	}

private:

	pointer	_M_ptr;

	TCOMPtrFree( const TCOMPtrFree & );

	TCOMPtrFree & operator = ( const TCOMPtrFree & );
};

}

#endif	/* _MACRO_WONDERSHARE_LIBRARY_TEMPLATE_COM_PTR_FREE_HPP */
