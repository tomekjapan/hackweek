#include "../Encoder.hpp"

volatile unsigned long Encoder::_s_total[2] = { 0 };
volatile unsigned long Encoder::_s_millis[2] = { 0 };
volatile unsigned long Encoder::_s_micros[2] = { 0 };


void Encoder::_intrHandler0 ()
{
	_s_millis[0] = millis();
	_s_micros[0] = micros();
	_s_total[0]++;
}


void Encoder::_intrHandler1 ()
{
	_s_millis[1] = millis();
	_s_micros[1] = micros();
	_s_total[1]++;
}


void Encoder::setup ()
{
	_total = 0;
	_millis = 0;
	_micros = 0;
	_serial = 0;
	_has_serial = 0;
	
	pinMode( _pin, INPUT );
	digitalWrite( _pin, HIGH );
	attachInterrupt(
		_intr,
		0 == _intr ? & _intrHandler0 : & _intrHandler1,
		RISING
	);
}


bool Encoder::update ()
{
	// If no one is waiting for updates, don't do any
	if ( ! _has_serial ) {
		return false;
	}

	bool changed = false;

	noInterrupts();

	if ( _total != _s_total[_intr] )
	{
		_total = _s_total[_intr];
		_millis = _s_millis[_intr];
		_micros = _s_micros[_intr];
		changed = true;
	}

	interrupts();

	return changed;
}

