// System headers
#include <unistd.h>

// External component headers
#include "common/ErrorReporting.hpp"

// Module header
#include "Handle.hpp"

namespace robocom {
namespace client
{
	const SysHandleType BAD_HANDLE = -1;

	Handle::Handle () throw ()
		: m_handle( BAD_HANDLE )
	{ }


	Handle::Handle (SysHandleType handle) throw ()
		: m_handle( handle )
	{ }


	Handle::~Handle () throw ()
	{
		close();
	}


	bool
	Handle::isOpen () const throw ()
	{
		return BAD_HANDLE != m_handle;
	}


	SysHandleType
	Handle::getNative () const throw ()
	{
		return m_handle;
	}


	void
	Handle::assign (SysHandleType handle) throw ()
	{
		close();
		m_handle = handle;
	}
	

	void
	Handle::close () throw ()
	{
		if ( isOpen() && 0 != ::close( m_handle ) ) {
			NCR_UNEXPECTED( "failed to close a handle" );
		}

		m_handle = BAD_HANDLE;
	}

} }
