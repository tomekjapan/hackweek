#include <Arduino.h>
#include "../Motor.hpp"

Motor::Motor (int dir, int spp)
	: _dirPin( dir )
	, _speedPin( spp )
	, _dir(0)
	, _speed(0)
{
}

void Motor::setup ()
{
	pinMode( _dirPin, OUTPUT );
	pinMode( _speedPin, OUTPUT );
	setDirection(0);
	setSignal(0);
}

void Motor::setDirection (int dir)
{
	_dir = dir;
	digitalWrite( _dirPin, dir ? LOW : HIGH );
}

void Motor::setSignal (int spd)
{
	_speed = spd;
	analogWrite( _speedPin, spd );
}

