#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <Arduino.h>

class Encoder
{
public:

	Encoder (int pin)
		: _pin(pin)
		, _intr(pin-2)
		, _total(0)
		, _millis(0)
		, _micros(0)
		, _serial(0)
		, _has_serial(false)
	{ }
  
	void setup ();
	bool update ();
	int getId () const { return _intr; }
	unsigned long getTotal () const { return _total; }
	unsigned long getMillis () const { return _millis; }
	unsigned long getMicros () const { return _micros; }

	word getSubscriber () const { return _serial; }
	void setSubscriber (word s) { _serial = s; _has_serial = true; }
	void clearSubscriber () { _has_serial = false; }

private:
	int _pin;
	int _intr;
	unsigned long _total;
	unsigned long _millis;
	unsigned long _micros;
	word _serial;
	bool _has_serial;

	static void _intrHandler0 ();
	static void _intrHandler1 ();

	static volatile unsigned long _s_total[2];
	static volatile unsigned long _s_millis[2];
	static volatile unsigned long _s_micros[2];
};

#endif
