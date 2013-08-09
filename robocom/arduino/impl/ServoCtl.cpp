#include "../ServoCtl.hpp"


ServoCtl::ServoCtl (int pin, int base_angle) throw ()
#if(0)
	: m_servo()
	, m_pin( pin )
#else
    : m_pin( pin )
#endif
	, m_angle( -1 )
	, m_base_angle( base_angle )
{
}


void
ServoCtl::setup () throw ()
{
#if(0)
	m_servo.attach(m_pin);
	setBase();
#endif
}


void
ServoCtl::setAngle (int angle) throw ()
{
#if(0)
	if ( angle < 0 ) {
		angle = 0;
	}

	if ( angle > 180 ) {
		angle = 180;
	}

	m_angle = angle;
	m_servo.write(angle);
#endif
}
