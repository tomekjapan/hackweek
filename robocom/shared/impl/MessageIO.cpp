#include "../Message.hpp"
#include "../StreamIO.hpp"

#include "../MessageIO.hpp"

namespace robocom {
namespace shared
{

	MessageIO::MessageIO (StreamIO& stream) throw ()
		: m_stream( stream )
		, m_state( IOS_NEED_MESSAGE_START )
	{ }


	bool
	MessageIO::read (Message& msg)
	{
		if ( IOS_NEED_MESSAGE_START == m_state )
		{
			if ( ! _readMessageStart() ) {
				return false;
			}
		}

		if ( IOS_NEED_DATA == m_state )
		{
			if ( ! _waitForDataAvailable() ) {
				return false;
			}
		}

		// IOS_HAVE_DATA == m_state
		return _readData( msg );
	}


	void
	MessageIO::write (const Message& msg)
	{
		m_stream.write( MC_MESSAGE_START );
		msg.serializeTo( m_stream );
		m_stream.write( MC_MESSAGE_END );
	}


	bool 
	MessageIO::_readMessageStart ()
	{
		while ( m_stream.available() > 0 )
		{
			if ( MC_MESSAGE_START == m_stream.read() )
			{
				m_state = IOS_NEED_DATA;
				return true;
			}
		}

		return false;
	}


	bool
	MessageIO::_waitForDataAvailable ()
	{
		if ( 0 == m_stream.available() ) {
			return false;
		}

		// The first byte is the message size. Make sure that the
		// message size falls within a valid range. If it doesn't,
		// reset the state and look for the next message start marker.

		const int message_size = m_stream.peek();

		if ( message_size > Message::HEADER_SIZE + Message::MAX_DATA_SIZE
			 	||
			 message_size < Message::HEADER_SIZE )
		{
			m_state = IOS_NEED_MESSAGE_START;
			return false;
		}

		// Message size does not include the message start and end marker bytes,
		// so we need at least one byte more than the message size
		if ( m_stream.available() <= message_size ) {
			return false;
		}

		m_state = IOS_HAVE_DATA;
		return true;
	}


	bool
	MessageIO::_readData (Message& msg)
	{
		m_state = IOS_NEED_MESSAGE_START;

		msg.deserializeFrom( m_stream );

		return MC_MESSAGE_END == m_stream.read();
	}

} }
