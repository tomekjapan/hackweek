#include "common/ErrorReporting.hpp"

#include "../Message.hpp"

namespace robocom {
namespace shared
{

	using namespace common;

	UInt8
	Message::getDataSize () const throw ()
	{
		return m_data_size;
	}

	
	UInt8
	Message::getMaxDataSize () const throw ()
	{
		if ( isImmediate() ) {
			return MAX_DATA_SIZE;
		}
		else {
			return MAX_DATA_SIZE - 4;
		}
	}


	UInt16
	Message::getTaskId () const throw ()
	{
		return m_task_id;
	}


	bool
	Message::isImmediate () const throw ()
	{
		return 0 != ( m_message_type & IMMEDIATE_BIT );
	}


	UInt8
	Message::getMessageType () const throw ()
	{
		return m_message_type & ~IMMEDIATE_BIT;
	}


	UInt32
	Message::getMillis () const throw ()
	{
		if ( isImmediate() ) {
			return 0;
		}

		return getUInt32(0);
	}


	UInt8
	Message::getUInt8 (unsigned offset) const throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 1u );

		return m_data[ offset ];
	}


	UInt16
	Message::getUInt16 (unsigned offset) const throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 2u );

		return ntoh( reinterpret_cast<const UInt16&>( m_data[ offset ] ) );
	}
	

	UInt32
	Message::getUInt32 (unsigned offset) const throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 4u );

		return ntoh( reinterpret_cast<const UInt32&>( m_data[ offset ] ) );
	}


	void
	Message::setDataSize (UInt8 size) throw ()
	{
		USE_CONTRACT_CHECK( size <= getMaxDataSize() );

		m_data_size = size;
	}


	void
	Message::setTaskId (UInt16 task_id) throw ()
	{
		m_task_id = task_id;
	}


	void
	Message::setMessageType (UInt8 message_type) throw ()
	{
		USE_CONTRACT_CHECK( message_type <= MAX_MESSAGE_TYPE );

		m_message_type = message_type | ( m_message_type & IMMEDIATE_BIT );
	}


	void
	Message::setImmediate () throw ()
	{
		if ( ! isImmediate() )
		{
			m_message_type |= IMMEDIATE_BIT;

			const int size = m_data_size;
			for ( int i = 4; i < size; i++ ) {
				m_data[i-4] = m_data[i];
			}
		}
	}


	void
	Message::setMillis (UInt32 ms) throw ()
	{
		USE_CONTRACT_CHECK( getDataSize() <= MAX_DATA_SIZE - 4u );

		if ( isImmediate() )
		{
			m_message_type &= ~IMMEDIATE_BIT;

			for ( int i = m_data_size - 1; i >= 4; i-- ) {
				m_data[i] = m_data[i-4];
			}
		}
	
		setUInt32( 0, ms );
	}


	void
	Message::setUInt8 (unsigned offset, UInt8 value) throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 1u );

		m_data[offset] = value;
	}


	void
	Message::setUInt16 (unsigned offset, UInt16 value) throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 2u );

		reinterpret_cast<UInt16&>( m_data[offset] ) = hton( value );
	}


	void
	Message::setUInt32 (unsigned offset, UInt32 value) throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 4u );

		reinterpret_cast<UInt32&>( m_data[offset] ) = hton( value );
	}

} }
