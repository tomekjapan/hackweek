#include "../EncoderReadingRequest.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	EncoderReadingRequest::EncoderReadingRequest (
		UInt16 task_id,
		UInt32 current_millis,
		UInt8 encoder_id,
		bool is_subscribe
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setMillis( current_millis );
		m_msg.setUInt8( OFFSET_ENCODER_ID, encoder_id );
		m_msg.setUInt8( OFFSET_IS_SUBSCRIBE, is_subscribe ? 1 : 0 );
	}


	EncoderReadingRequest::EncoderReadingRequest (
		UInt16 task_id,
		UInt8 encoder_id,
		bool is_subscribe
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setImmediate();
		m_msg.setUInt8( OFFSET_ENCODER_ID, encoder_id );
		m_msg.setUInt8( OFFSET_IS_SUBSCRIBE, is_subscribe ? 1 : 0 );
	}


	EncoderReadingRequest::EncoderReadingRequest (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	EncoderReadingRequest&
	EncoderReadingRequest::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	EncoderReadingRequest::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	EncoderReadingRequest::validate () const throw ()
	{
		if ( m_msg.getMessageType() != MSGID ) {
			return STATUS_E_MESSAGE_TYPE;
		}

		if ( m_msg.getDataSize() != DATA_SIZE ) {
			return STATUS_E_DATA_SIZE;
		}

		if ( getEncoderId() != 0 && getEncoderId() != 1 ) {
			return STATUS_E_ENCODER_ID;
		}

		const UInt8 is_subscribe = m_msg.getUInt8( OFFSET_IS_SUBSCRIBE );
		if ( is_subscribe != 0 && is_subscribe != 1 ) {
			return STATUS_E_IS_SUBSCRIBE;
		}

		return STATUS_OK;
	}


	UInt8
	EncoderReadingRequest::getEncoderId () const throw ()
	{
		return m_msg.getUInt8( OFFSET_ENCODER_ID );
	}


	bool
	EncoderReadingRequest::getIsSubscribe () const throw ()
	{
		return 0 != m_msg.getUInt8( OFFSET_IS_SUBSCRIBE );
	}

} } }

