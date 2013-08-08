#ifndef ROBOT_SERVER_HPP
#define ROBOT_SERVER_HPP

#include "robocom/shared/Server.hpp"
#include "Motor.hpp"
#include "Encoder.hpp"
#include "Gyro.hpp"
#include "LogoCommands.hpp"

/**
 * This class defines a robocom server for a particular 2WD mobile platform
 * equipped with two motors, each having an encoder
 *
 * The server allows application of driving signal to both wheels separately
 * and also supports subscription to the encoder measurements.
 */
class RobotServer
	: public robocom::shared::Server
{
public:

	/// @name Exported constants
	///@{

	/**
	 * Defines pins assumed to be connected to specific peripherals.
	 */
	enum PinDefinitions
	{
		MOTOR_1_DIR_PIN = 4,
		MOTOR_1_SIGNAL_PIN = 5,
		MOTOR_2_DIR_PIN = 7,
		MOTOR_2_SIGNAL_PIN = 6,
		ENCODER_1_PIN = 2,
		ENCODER_2_PIN = 3
	};

	///@}


	/// @name Lifetime management
	///@{

	/**
	 * Creates a new instance
	 *
	 * @param stream the StreamIO instance to use for the communication
	 *  channel
	 */
	RobotServer (robocom::shared::StreamIO& stream) throw ();

	/**
	 * Destroys the state of this object
	 */
	virtual ~RobotServer () throw ()
	{ }
	
	///@}


	/// @name Methods
	///@{

	/**
	 * Sets up motors and encoders
	 */
	virtual void setup () throw ();

	///@}

protected:

	/// @name Protected methods
	///@{

	/**
	 * Sets both wheel speeds to zero and removes subscriptions
	 * from both encoders
	 *
	 * A reset request will result in a notice response about the
	 * new wheel drive signals.
	 *
	 * @param req the request message that caused this function to
	 *   be called
	 */
	virtual void handleReset (
		const robocom::shared::msg::ResetRequest& req
	) throw ();

	/**
	 * Dispatches the application-specific requests:
	 * - request to set wheel drive signals
	 * - request to subscribe to encoder measurements
	 *
	 * @param msg the message to handle
	 */
	virtual void handleMessage (
		const robocom::shared::Message& msg
	) throw ();

	/**
	 * Adds responses for encoder measurements if any and if the
	 * client subscribed for them
	 */
	virtual void handleStateUpdate () throw ();

	///@}

private:

	RobotServer (const RobotServer&);
	void operator= (const RobotServer&);

	void _processMessage (
		const robocom::shared::msg::SetWheelDriveRequest& req
	) throw ();

	void _processMessage (
		const robocom::shared::msg::EncoderReadingRequest& req
	) throw ();

	void _notifyWheelDriveChanged (
		const robocom::shared::Message& msg
	) throw ();

	void _notifyEncoderReading (
		const Encoder& encoder
	) throw ();

	void _setWheelDrive (
		UInt8 motor_1_direction,
		UInt8 motor_1_signal,
		UInt8 motor_2_direction,
		UInt8 motor_2_signal
	) throw ();

	Motor m_motor_1;
	Motor m_motor_2;
	Encoder m_encoder_1;
	Encoder m_encoder_2;
    Gyro m_gyro;

    LogoTurn m_turn;
};


#endif
