// Component includes
#include "../Gyro.hpp"
#include "../Motor.hpp"

// Module include
#include "../LogoCommands.hpp"

LogoTurn::LogoTurn (
	const Gyro& gyro,
	Motor& motor_1,
	Motor& motor_2
) throw ()
	: m_p_gyro( &gyro )
	, m_p_motor_1( &motor_1 )
	, m_p_motor_2( &motor_2 )
	, m_start_yaw(0.0f)
	, m_target_angle(0.0f)
	, m_motor_speed( 60 )	// TODO: choose something reasonable
	, m_is_active( false )
{}


bool
LogoTurn::isDone () const throw ()
{
	return (m_target_angle >= 0)
		? (getAngle() >= m_target_angle)
		: (getAngle() <= m_target_angle);
}


float
LogoTurn::getAngle() const throw ()
{
	float angle = m_p_gyro->getLatestReading().ypr[0] - m_start_yaw;
	if (angle > _maxAngle()) {
		angle -= 2*M_PI;
	} else if (angle < _minAngle()) {
		angle += 2*M_PI;
	}
	return angle;
}


bool
LogoTurn::start (UInt16 task_id, float target_angle) throw ()
{
	if ( m_is_active || fabs(target_angle) < 1.0 ) {
		return false;
	}

	m_is_active = true;
	m_start_yaw = m_p_gyro->getLatestReading().ypr[0];
	m_target_angle = target_angle;
	setTaskId( task_id );
	_turnMotorsOn( target_angle > 0 ? 0 : 1 );
	
	return true;
}


bool
LogoTurn::update () throw ()
{
	if ( m_is_active && isDone() )
	{
		_turnMotorsOff();
		m_is_active = false;
	}

	return ! m_is_active;
}


float
LogoTurn::_maxAngle() const throw ()
{
	return m_target_angle >= 0 ? (3.0f*M_PI/2.0f) : (M_PI/2.0f);
}


float
LogoTurn::_minAngle() const throw ()
{
	return m_target_angle >= 0 ? (-M_PI/2.0f) : (-3.0f*M_PI/2.0f);
}


void
LogoTurn::_turnMotorsOn (UInt8 direction) throw ()
{
	m_p_motor_1->setDirection(direction);
	m_p_motor_2->setDirection(1 - direction);

	m_p_motor_1->setSignal(m_motor_speed);
	m_p_motor_2->setSignal(m_motor_speed);
}


void
LogoTurn::_turnMotorsOff () throw ()
{
	m_p_motor_1->setSignal(0);
	m_p_motor_2->setSignal(0);
}


