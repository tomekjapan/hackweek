#include "../EncoderReadingNotice.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	EncoderReadingNotice::EncoderReadingNotice (
		UInt16 task_id,
		UInt32 current_millis,
		UInt8 encoder_id,
		UInt32 tick_index,
		UInt32 measurement_us
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setMillis( current_millis );
		m_msg.setUInt8( OFFSET_ENCODER_ID, encoder_id );
		m_msg.setUInt32( OFFSET_TICK_INDEX, tick_index );
		m_msg.setUInt32( OFFSET_MICROS, measurement_us );
	}


	EncoderReadingNotice::EncoderReadingNotice (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	EncoderReadingNotice&
	EncoderReadingNotice::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	EncoderReadingNotice::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	EncoderReadingNotice::validate () const throw ()
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

		return STATUS_OK;
	}


	UInt8
	EncoderReadingNotice::getEncoderId () const throw ()
	{
		return m_msg.getUInt8( OFFSET_ENCODER_ID );
	}


	UInt32
	EncoderReadingNotice::getTickIndex () const throw ()
	{
		return m_msg.getUInt32( OFFSET_TICK_INDEX );
	}


	UInt32
	EncoderReadingNotice::getMeasurementMicros () const throw ()
	{
		return m_msg.getUInt32( OFFSET_MICROS );
	}

} } }

