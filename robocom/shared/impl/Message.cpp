#include "../StreamIO.hpp"

#include "../Message.hpp"

namespace robocom {
namespace shared
{

	using namespace common;


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


	UInt8
	Message::getMessageType () const throw ()
	{
		return m_message_type & ~IMMEDIATE_BIT;
	}


	bool
	Message::isImmediate () const throw ()
	{
		return 0 != ( m_message_type & IMMEDIATE_BIT );
	}


	UInt16
	Message::getTaskId () const throw ()
	{
		return ntoh_UInt16( m_task_id );
	}


	UInt32
	Message::getMillis () const throw ()
	{
		if ( isImmediate() ) {
			return 0;
		}

		return ntoh_UInt32( m_data );
	}


	UInt8
	Message::getUInt8 (unsigned offset) const throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 1u );

		if ( ! isImmediate() ) {
			offset += 4u;
		}

		return m_data[ offset ];
	}


	UInt16
	Message::getUInt16 (unsigned offset) const throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 2u );

		if ( ! isImmediate() ) {
			offset += 4u;
		}

		return ntoh_UInt16( m_data + offset );
	}
	

	UInt32
	Message::getUInt32 (unsigned offset) const throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 4u );

		if ( ! isImmediate() ) {
			offset += 4u;
		}

		return ntoh_UInt32( m_data + offset );
	}


	void
	Message::clear () throw ()
	{
		m_data_size = 0;
		m_message_type = 0;
		m_task_id[0] = 0;
		m_task_id[1] = 0;

		for ( unsigned i = 0; i < sizeof(m_data); i++ ) {
			m_data[i] = 0;
		}
	}


	void
	Message::setDataSize (UInt8 size) throw ()
	{
		USE_CONTRACT_CHECK( size <= getMaxDataSize() );

		m_data_size = size;
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
			for ( int i = 0; i < size; i++ ) {
				m_data[i] = m_data[i+4];
			}
		}
	}


	void
	Message::setTaskId (UInt16 task_id) throw ()
	{
		hton_UInt16( m_task_id, task_id );
	}


	void
	Message::setMillis (UInt32 ms) throw ()
	{
		USE_CONTRACT_CHECK( getDataSize() <= MAX_DATA_SIZE - 4u );

		if ( isImmediate() )
		{
			m_message_type &= ~IMMEDIATE_BIT;

			for ( int i = m_data_size - 1; i >= 0; i-- ) {
				m_data[i+4] = m_data[i];
			}
		}

		hton_UInt32( m_data, ms );
	}


	void
	Message::setUInt8 (unsigned offset, UInt8 value) throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 1u );

		if ( ! isImmediate() ) {
			offset += 4u;
		}

		m_data[offset] = value;
	}


	void
	Message::setUInt16 (unsigned offset, UInt16 value) throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 2u );

		if ( ! isImmediate() ) {
			offset += 4u;
		}

		hton_UInt16( m_data + offset, value );
	}


	void
	Message::setUInt32 (unsigned offset, UInt32 value) throw ()
	{
		USE_CONTRACT_CHECK( offset <= getDataSize() - 4u );

		if ( ! isImmediate() ) {
			offset += 4u;
		}

		hton_UInt32( m_data + offset, value );
	}


	int
	Message::comparePriority (const Message& other) const throw ()
	{
		if ( isImmediate() )
		{
			if ( other.isImmediate() ) {
				return 0;
			}
			else {
				return -1;
			}
		}
		else
		{
			if ( other.isImmediate() ) {
				return 1;
			}
			else if ( getMillis() < other.getMillis() ) {
				return -1;
			}
			else if ( getMillis() > other.getMillis() ) {
				return 1;
			}
		}

		return 0;
	}


	void
	Message::deserializeFrom (StreamIO& stream)
	{
		// The caller (MessageIO) has verified that the data size
		// is in the valid range, and that there are enough data in the
		// output buffer to read the whole message.

		const UInt8 data_size = stream.read() - HEADER_SIZE;
		m_message_type = stream.read();
		stream.readBytes( (char*) m_task_id, sizeof(m_task_id) );

		if ( data_size > 0 ) {
			stream.readBytes( (char*) m_data, data_size );
		}

		m_data_size = data_size;
		if ( ! isImmediate() ) {
			m_data_size -= 4u;
		}
	}


	void
	Message::serializeTo (StreamIO& stream) const
	{
		UInt8 data_size = m_data_size;
		if ( ! isImmediate() ) {
			data_size += 4u;
		}

		stream.write( data_size + HEADER_SIZE );
		stream.write( m_message_type );
		stream.write( m_task_id, sizeof(m_task_id) );
		stream.write( m_data, data_size );
	}

} }
