#include "../GyroReadingNotice.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	GyroReadingNotice::GyroReadingNotice (
		UInt16 task_id,
		UInt32 current_millis,
		float yaw_degrees,
		float pitch_degrees,
		float roll_degrees,
		UInt32 measurement_us
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setMillis( current_millis );
		m_msg.setUInt16( OFFSET_YAW_DEGREES, _encodeAngle(yaw_degrees) );
		m_msg.setUInt16( OFFSET_PITCH_DEGREES, _encodeAngle(pitch_degrees) );
		m_msg.setUInt16( OFFSET_ROLL_DEGREES, _encodeAngle(roll_degrees) );
		m_msg.setUInt32( OFFSET_MICROS, measurement_us );
	}


	GyroReadingNotice::GyroReadingNotice (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	GyroReadingNotice&
	GyroReadingNotice::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	GyroReadingNotice::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	GyroReadingNotice::validate () const throw ()
	{
		if ( m_msg.getMessageType() != MSGID ) {
			return STATUS_E_MESSAGE_TYPE;
		}

		if ( m_msg.getDataSize() != DATA_SIZE ) {
			return STATUS_E_DATA_SIZE;
		}

		if ( getYawDegrees() > 180 || getYawDegrees() < -180) {
			return STATUS_E_ANGLE_RANGE;
		}
		if ( getPitchDegrees() > 180 || getPitchDegrees() < -180) {
			return STATUS_E_ANGLE_RANGE;
		}
		if ( getRollDegrees() > 180 || getRollDegrees() < -180) {
			return STATUS_E_ANGLE_RANGE;
		}

		return STATUS_OK;
	}


	float
	GyroReadingNotice::getYawDegrees () const throw ()
	{
		return _decodeAngle(m_msg.getUInt16( OFFSET_YAW_DEGREES ));
	}


	float
	GyroReadingNotice::getPitchDegrees () const throw ()
	{
		return _decodeAngle(m_msg.getUInt16( OFFSET_PITCH_DEGREES ));
	}


	float
	GyroReadingNotice::getRollDegrees () const throw ()
	{
		return _decodeAngle(m_msg.getUInt16( OFFSET_ROLL_DEGREES ));
	}


	UInt32
	GyroReadingNotice::getMeasurementMicros () const throw ()
	{
		return m_msg.getUInt32( OFFSET_MICROS );
	}

} } }

