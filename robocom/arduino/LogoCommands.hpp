/* Helper routines for logo turtle command execution.
 *
 * Dropbox Hack Week, August 2013.
 */
#ifndef LOGO_COMMANDS_HPP
#define LOGO_COMMANDS_HPP

#include "arduino_base.hpp"

/**
 * An abstract class implementing the LOGO command control protocol
 */
class LogoCommand
{
public:

	/**
	 * Creates an inactive Logo command object with no associated task ID
	 */
	LogoCommand ()
		: m_task_id(-1)
	{ }

	/**
	 * Returns the ID of the task associated with this command
	 */
	UInt16 getTaskId () const throw ()
	{
		return m_task_id;
	}

	/**
	 * Updates the state associated with the command
	 *
	 * @return true if the command completed execution, false otherwise
	 */
	virtual bool update () throw () = 0;

protected:

	/**
	 * Sets the task ID associated with this command
	 */
	void setTaskId (UInt16 task_id) throw ()
	{
		m_task_id = task_id;
	}

private:

	UInt16 m_task_id;
};


/**
 * LOGO command for turning by a certain angle
 */
class LogoTurn
	: public LogoCommand
{
	const Gyro * m_p_gyro;
	Motor* m_p_motor_1;
	Motor* m_p_motor_2;
	float m_start_yaw;
	float m_target_angle;
	int m_motor_speed;
	bool m_is_active;

public:

	/**
	 * Creates an object using the specified peripherals
	 */
	LogoTurn(
		const Gyro& gyro,
		Motor& motor_1,
		Motor& motor_2
	) throw ();

	/**
	 * Returns whether the turn is finished.
	 */  
	bool isDone() const throw ();
  
	/**
	 * Returns the current turn angle.
	 */
	float getAngle() const throw ();

	/**
	 * Starts a turn by the given angle.
	 *
	 * @return true if the turn was started, false otherwise
	 */
	bool start (UInt16 task_id, float target_angle) throw ();

	/**
	 * Updates the state of this command
	 *
	 * @returns true if this command has completed execution, false
	 *   if the command is still executing
	 */
	virtual bool update () throw ();

private:

	float _maxAngle() const throw ();
	float _minAngle() const throw ();

	void _turnMotorsOn (UInt8 direction) throw ();
	void _turnMotorsOff () throw ();
};



/**
 * LOGO command for moving by a certain distance
 */
class LogoMove
	: public LogoCommand
{
	const Gyro * m_p_gyro;
	Motor* m_p_motor_1;
	Motor* m_p_motor_2;
	Encoder* m_p_encoder_1;
	Encoder* m_p_encoder_2;
	float m_start_angle;
	UInt32 m_start_tick_1;
	UInt32 m_start_tick_2;
	UInt32 m_target_distance;
	int m_motor_speed;
	bool m_is_active;

public:

	/**
	 * Creates an object using the specified peripherals
	 */
	LogoMove(
		const Gyro& gyro,
		Motor& motor_1,
		Motor& motor_2,
		Encoder& encoder_1,
		Encoder& encoder_2
	) throw ();

	/**
	 * Returns whether the drive is finished.
	 */  
	bool isDone() const throw ();
  
	/**
	 * Returns the cumulative distance traveled so far (in ticks) by
	 * both wheels
	 */
	UInt32 getDistance () const throw ();

	/**
	 * Starts a turn by the given angle.
	 *
	 * @param task_id the ID of the task associated with this command
	 * @param direction 0 to move forward, 1 to move backward
	 * @param distance_ticks the number of ticks (1 tick equals 1/10th of wheel
	 *   diameter) to move
	 *
	 * @return true if the move was started, false otherwise
	 */
	bool start (
		UInt16 task_id,
		UInt8 direction,
		UInt32 distance_ticks
	) throw ();

	/**
	 * Updates the state of this command
	 *
	 * @returns true if this command has completed execution, false
	 *   if the command is still executing
	 */
	virtual bool update () throw ();

private:

	void _turnMotorsOn (UInt8 direction) throw ();
	void _turnMotorsOff () throw ();
};


/**
 * LOGO command for moving by a certain distance
 */
class LogoPen
	: public LogoCommand
{
	Servo * m_p_servo;
	UInt8 m_up_angle;
	UInt8 m_down_angle;
	bool m_is_active;

public:

	/**
	 * Creates an object using the specified peripherals
	 */
	LogoPen(Servo& servo) throw ();

	/**
	 * Starts the move of the pen
	 *
	 * @param task_id the ID of the task associated with this command
	 * @param direction 0 to move the pen up, 1 to move the pen down
	 *
	 * @return true if the move was started, false otherwise
	 */
	bool start (UInt16 task_id, UInt8 direction) throw ();

	/**
	 * Updates the state of this command
	 *
	 * @returns true if this command has completed execution, false
	 *   if the command is still executing
	 */
	virtual bool update () throw ();
};


#endif // LOGO_COMMANDS_HPP
