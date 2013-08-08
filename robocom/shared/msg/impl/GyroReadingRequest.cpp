#include "../GyroReadingRequest.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	GyroReadingRequest::GyroReadingRequest (
		UInt16 task_id,
		UInt32 current_millis,
		UInt32 min_delay_millis,
		bool is_subscribe
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setMillis( current_millis );
		m_msg.setUInt8( OFFSET_IS_SUBSCRIBE, is_subscribe ? 1 : 0 );
		m_msg.setUInt32( OFFSET_MIN_DELAY_MILLIS, min_delay_millis );
	}


	GyroReadingRequest::GyroReadingRequest (
		UInt16 task_id,
		UInt32 min_delay_millis,
		bool is_subscribe
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setImmediate();
		m_msg.setUInt8( OFFSET_IS_SUBSCRIBE, is_subscribe ? 1 : 0 );
		m_msg.setUInt32( OFFSET_MIN_DELAY_MILLIS, min_delay_millis );
	}


	GyroReadingRequest::GyroReadingRequest (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	GyroReadingRequest&
	GyroReadingRequest::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	GyroReadingRequest::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	GyroReadingRequest::validate () const throw ()
	{
		if ( m_msg.getMessageType() != MSGID ) {
			return STATUS_E_MESSAGE_TYPE;
		}

		if ( m_msg.getDataSize() != DATA_SIZE ) {
			return STATUS_E_DATA_SIZE;
		}

		const UInt8 is_subscribe = m_msg.getUInt8( OFFSET_IS_SUBSCRIBE );
		if ( is_subscribe != 0 && is_subscribe != 1 ) {
			return STATUS_E_IS_SUBSCRIBE;
		}

		return STATUS_OK;
	}


	UInt32
	GyroReadingRequest::getMinDelayMillis () const throw ()
	{
		return m_msg.getUInt32( OFFSET_MIN_DELAY_MILLIS );
	}


	bool
	GyroReadingRequest::getIsSubscribe () const throw ()
	{
		return 0 != m_msg.getUInt8( OFFSET_IS_SUBSCRIBE );
	}

} } }

