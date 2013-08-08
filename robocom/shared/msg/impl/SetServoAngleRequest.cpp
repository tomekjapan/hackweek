#include "../SetServoAngleRequest.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	SetServoAngleRequest::SetServoAngleRequest (
		UInt16 task_id,
		UInt32 current_millis,
		UInt8 servo_id,
		UInt8 angle
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setMillis( current_millis );
		m_msg.setUInt8( OFFSET_SERVO_ID, servo_id );
		m_msg.setUInt8( OFFSET_ANGLE, angle );
	}


	SetServoAngleRequest::SetServoAngleRequest (
		UInt16 task_id,
		UInt8 servo_id,
		UInt8 angle
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setImmediate();
		m_msg.setUInt8( OFFSET_SERVO_ID, servo_id );
		m_msg.setUInt8( OFFSET_ANGLE, angle );
	}


	SetServoAngleRequest::SetServoAngleRequest (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	SetServoAngleRequest&
	SetServoAngleRequest::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	SetServoAngleRequest::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	SetServoAngleRequest::validate () const throw ()
	{
		if ( m_msg.getMessageType() != MSGID ) {
			return STATUS_E_MESSAGE_TYPE;
		}

		if ( m_msg.getDataSize() != DATA_SIZE ) {
			return STATUS_E_DATA_SIZE;
		}

		return STATUS_OK;
	}


	UInt8
	SetServoAngleRequest::getServoId () const throw ()
	{
		return m_msg.getUInt8( OFFSET_SERVO_ID );
	}


	UInt8
	SetServoAngleRequest::getAngle () const throw ()
	{
		return m_msg.getUInt8( OFFSET_ANGLE );
	}

} } }

