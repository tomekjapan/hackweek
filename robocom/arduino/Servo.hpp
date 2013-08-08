#ifndef SERVO_HPP
#define SERVO_HPP

/**
 * Helper class for the servo control
 */
class Servo
{
public:

	/**
	 * Creates a servo controller instance
	 *
	 * The base angle is the initial position of the servo.
	 */
	Servo (int pin, int base_angle) throw ();

	/**
	 * Sets up the servo's pin
	 */
	void setup () throw ();

	/**
	 * Returns the current angle of rotation
	 */
	int getAngle () const throw ()
	{
		return m_angle;
	}

	/**
	 * Returns the base angle, which is the initial position
	 */
	int getBaseAngle () const throw ()
	{
		return m_base_angle;
	}

	/**
	 * Moves the servo to the prescribed location
	 */
	void setAngle (int angle) throw ();

	/**
	 * Moves the servo to the base position
	 */
	void setBase () throw ()
	{
		setAngle( m_base_angle );
	}

private:

	int m_pin;
	int m_base_angle;
	int m_angle;
};

#endif
