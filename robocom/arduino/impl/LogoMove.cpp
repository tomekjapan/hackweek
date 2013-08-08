// Component includes
#include "../Encoder.hpp"
#include "../Gyro.hpp"
#include "../Motor.hpp"

// Module include
#include "../LogoCommands.hpp"

LogoMove::LogoMove (
	const Gyro& gyro,
	Motor& motor_1,
	Motor& motor_2,
	Encoder& encoder_1,
	Encoder& encoder_2
) throw ()
	: m_p_gyro( &gyro )
	, m_p_motor_1( &motor_1 )
	, m_p_motor_2( &motor_2 )
	, m_p_encoder_1( &encoder_1 )
	, m_p_encoder_2( &encoder_2 )
	, m_start_angle(0.0f)
	, m_start_tick_1(0)
	, m_start_tick_2(0)
	, m_target_distance(0)
	, m_motor_speed( 80 )	// TODO: choose something reasonable
	, m_is_active( false )
{}


bool
LogoMove::isDone () const throw ()
{
	return getDistance() >= (2 * m_target_distance);
}


UInt32
LogoMove::getDistance() const throw ()
{
	return
		(m_p_encoder_1->getTotal() - m_start_tick_1) +
		(m_p_encoder_2->getTotal() - m_start_tick_2);
}


bool
LogoMove::start (
	UInt16 task_id, 
	UInt8 direction,
	UInt32 distance_ticks
) throw ()
{
	if ( m_is_active || distance_ticks == 0 ) {
		return false;
	}

	m_is_active = true;
	m_start_angle = m_p_gyro->getLatestReading().ypr[0];
	m_start_tick_1 = m_p_encoder_1->getTotal();
	m_start_tick_2 = m_p_encoder_2->getTotal();
	m_target_distance = distance_ticks;

	setTaskId( task_id );
	_turnMotorsOn( direction );
	
	return true;
}


bool
LogoMove::update () throw ()
{
	if ( m_is_active && isDone() )
	{
		_turnMotorsOff();
		m_is_active = false;
	}

	return ! m_is_active;
}


void
LogoMove::_turnMotorsOn (UInt8 direction) throw ()
{
	m_p_motor_1->setDirection(direction);
	m_p_motor_2->setDirection(direction);

	m_p_motor_1->setSignal(m_motor_speed);
	m_p_motor_2->setSignal(m_motor_speed);
}


void
LogoMove::_turnMotorsOff () throw ()
{
	m_p_motor_1->setSignal(0);
	m_p_motor_2->setSignal(0);
}


