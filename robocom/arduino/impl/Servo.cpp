#include <Arduino.h>
#include "../Servo.hpp"

Servo::Servo (int pin, int base_angle) throw ()
	: m_pin( pin )
	, m_angle( -1 )
	, m_base_angle( base_angle )
{
}


void
Servo::setup () throw ()
{
	pinMode( m_pin, OUTPUT );
	setBase();
}


void
Servo::setAngle (int angle) throw ()
{
	if ( angle < 0 ) {
		angle = 0;
	}

	if ( angle > 255 ) {
		angle = 255;
	}

	m_angle = angle;
	analogWrite( m_pin, m_angle );
}
