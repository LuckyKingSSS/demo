/*
*******************************************************************************

    ws_auto_ptr.hpp

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_WONDERSHARE_LIBRARY_AUTO_COM_PTR_HPP
#define	_MACRO_WONDERSHARE_LIBRARY_AUTO_COM_PTR_HPP

namespace libWondershare
{

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  ws_auto_ptr                                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

template< class _Tp > class ws_auto_ptr
{
public:

	typedef _Tp			value_type;
	typedef value_type *		pointer;
	typedef value_type &		reference;
	typedef const value_type *	const_pointer;
	typedef const value_type &	const_reference;

public:

	explicit ws_auto_ptr( pointer ptr = 0 ): _M_ptr( ptr )
	{
	}

	~ws_auto_ptr()
	{
		if_release();
	}

private:

	ws_auto_ptr( const ws_auto_ptr & );

	ws_auto_ptr & operator = ( const ws_auto_ptr & );

public:

	pointer get() const
	{
		return _M_ptr;
	}

	void release()
	{
		if_release();

		_M_ptr = 0;
	}

	void reset( pointer ptr )
	{
		if_release();

		_M_ptr = ptr;
	}

public:

	operator pointer ()
	{
		return get();
	}

	operator const_pointer () const
	{
		return get();
	}

	bool operator ! () const
	{
		return ! get();
	}

	pointer operator -> ()
	{
		return get();
	}

	const_pointer operator -> () const
	{
		return get();
	}

	reference operator * ()
	{
		return *( get() );
	}

	const_reference operator * () const
	{
		return *( get() );
	}

private:

	void if_release()
	{
		if ( _M_ptr ) delete _M_ptr;
	}

private:

	pointer	_M_ptr;
};

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  ws_auto_com_ptr                                                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

template< class _Tp > class ws_auto_com_ptr
{
public:

	typedef _Tp			value_type;
	typedef value_type *		pointer;
	typedef value_type &		reference;
	typedef const value_type *	const_pointer;
	typedef const value_type &	const_reference;

public:

	explicit ws_auto_com_ptr( pointer ptr = 0 ): _M_ptr( ptr )
	{
	}

	ws_auto_com_ptr( const ws_auto_com_ptr & ohr )
	{
		if ( this != &ohr ) _M_ptr = ohr.clone();
	}

	~ws_auto_com_ptr()
	{
		if_release();
	}

	ws_auto_com_ptr & operator = ( const ws_auto_com_ptr & ohr )
	{
		if ( this != &ohr ) reset( ohr.get() );

		return *this;
	}

public:

	pointer get() const
	{
		return _M_ptr;
	}

	void release()
	{
		if_release();

		_M_ptr = 0;
	}

	void reset( pointer ptr )
	{
		if_release();

		_M_ptr = ptr;

		if_addref();
	}

public:

	operator pointer ()
	{
		return get();
	}

	operator const_pointer () const
	{
		return get();
	}

	bool operator ! () const
	{
		return ! get();
	}

	pointer operator -> ()
	{
		return get();
	}

	const_pointer operator -> () const
	{
		return get();
	}

	reference operator * ()
	{
		return *( get() );
	}

	const_reference operator * () const
	{
		return *( get() );
	}

private:

	pointer clone()
	{
		return if_addref(), _M_ptr;
	}

	void if_addref()
	{
		if ( _M_ptr ) _M_ptr->AddRef();
	}

	void if_release()
	{
		if ( _M_ptr ) _M_ptr->Release();
	}

private:

	pointer	_M_ptr;
};

}

#endif	/* _MACRO_WONDERSHARE_LIBRARY_AUTO_COM_PTR_HPP */
