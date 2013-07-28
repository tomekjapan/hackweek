#ifndef MOTOR_HPP
#define MOTOR_HPP

class Motor
{
public:
	Motor (int dir, int spp);
  
	void setup ();

	int getDirection () const { return _dir; }
	int getSignal () const { return _speed; }

	void setDirection (int dir);
	void setSignal (int spd);
  
private:
	int _dirPin;
	int _speedPin;
	int _dir;
	int _speed;
};

#endif
