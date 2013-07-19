#include "../SetWheelDriveRequest.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	SetWheelDriveRequest::SetWheelDriveRequest (
		UInt32 current_millis,
		UInt8 motor_1_direction,
		UInt8 motor_1_signal,
		UInt8 motor_2_direction,
		UInt8 motor_2_signal
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setMillis( current_millis );
		m_msg.setUInt8( OFFSET_MOTOR_1_DIRECTION, motor_1_direction );
		m_msg.setUInt8( OFFSET_MOTOR_1_SIGNAL, motor_1_signal );
		m_msg.setUInt8( OFFSET_MOTOR_2_DIRECTION, motor_2_direction );
		m_msg.setUInt8( OFFSET_MOTOR_2_SIGNAL, motor_2_signal );
	}


	SetWheelDriveRequest::SetWheelDriveRequest (
		UInt8 motor_1_direction,
		UInt8 motor_1_signal,
		UInt8 motor_2_direction,
		UInt8 motor_2_signal
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setImmediate();
		m_msg.setUInt8( OFFSET_MOTOR_1_DIRECTION, motor_1_direction );
		m_msg.setUInt8( OFFSET_MOTOR_1_SIGNAL, motor_1_signal );
		m_msg.setUInt8( OFFSET_MOTOR_2_DIRECTION, motor_2_direction );
		m_msg.setUInt8( OFFSET_MOTOR_2_SIGNAL, motor_2_signal );
	}


	SetWheelDriveRequest::SetWheelDriveRequest (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	SetWheelDriveRequest&
	SetWheelDriveRequest::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	SetWheelDriveRequest::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	SetWheelDriveRequest::validate () const throw ()
	{
		if ( m_msg.getMessageType() != MSGID ) {
			return STATUS_E_MESSAGE_TYPE;
		}

		if ( m_msg.getDataSize() != DATA_SIZE ) {
			return STATUS_E_DATA_SIZE;
		}

		if ( getMotor1Direction() != 0 && getMotor1Direction() != 1 ) {
			return STATUS_E_MOTOR_1_DIRECTION;
		}

		if ( getMotor2Direction() != 0 && getMotor2Direction() != 1 ) {
			return STATUS_E_MOTOR_2_DIRECTION;
		}

		return STATUS_OK;
	}


	UInt8
	SetWheelDriveRequest::getMotor1Direction () const throw ()
	{
		return m_msg.getUInt8( OFFSET_MOTOR_1_DIRECTION );
	}


	UInt8
	SetWheelDriveRequest::getMotor1Signal () const throw ()
	{
		return m_msg.getUInt8( OFFSET_MOTOR_1_SIGNAL );
	}


	UInt8
	SetWheelDriveRequest::getMotor2Direction () const throw ()
	{
		return m_msg.getUInt8( OFFSET_MOTOR_2_DIRECTION );
	}


	UInt8
	SetWheelDriveRequest::getMotor2Signal () const throw ()
	{
		return m_msg.getUInt8( OFFSET_MOTOR_2_SIGNAL );
	}

} } }

