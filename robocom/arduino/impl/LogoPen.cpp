// Component includes
#include "../Servo.hpp"

// Module include
#include "../LogoCommands.hpp"

LogoPen::LogoPen (
	Servo& servo
) throw ()
	: m_p_servo( &servo )
	, m_up_angle( 45 )
	, m_down_angle( 135 )
	, m_is_active( false )
{}


bool
LogoPen::start (
	UInt16 task_id, 
	UInt8 direction
) throw ()
{
	if ( m_is_active ) {
		return false;
	}

	m_is_active = true;
	m_p_servo->setAngle( 0 == direction ? m_up_angle : m_down_angle );
	setTaskId( task_id );
	
	return true;
}


bool
LogoPen::update () throw ()
{
	if ( m_is_active ) {
		m_is_active = false;
	}

	return ! m_is_active;
}

