#ifndef ROBOCOM_SHARED_MSG_WHEEL_DRIVE_CHANGE_NOTICE_HPP
#define ROBOCOM_SHARED_MSG_WHEEL_DRIVE_CHANGE_NOTICE_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class represents the notification sent from arduino to the client
	 * whenever the wheel drive changes.
	 */
	class WheelDriveChangedNotice
	{
	public:

		/// The message type for instances of this class
		enum { MSGID = RobocomMessageTypes::MSGID_WHEEL_DRIVE };

		/**
		 * Constructor
		 */
		WheelDriveChangedNotice (
			UInt16 task_id,
			UInt32 current_millis,
			UInt8 motor_1_direction,
			UInt8 motor_1_signal,
			UInt8 motor_2_direction,
			UInt8 motor_2_signal
		) throw ();

		/**
		 * Constructs a WheelDriveChangedNotice object from the given message
		 */
		explicit WheelDriveChangedNotice (const Message& msg) throw ();

		/**
		 * Copies state from the given message into this object
		 */
		WheelDriveChangedNotice& operator= (const Message& msg) throw ();

		/**
		 * Returns the representation of this object state as a Message
		 * instance
		 */
		const Message& asMessage () const throw ();

		/**
		 * Returns whether the data stored in this object is valid
		 * and consistent
		 *
		 * Clients should not attempt to interpret the message if
		 * this function returns an error value
		 *
		 * @return STATUS_OK if data is valid
		 *   STATUS_E_MESSAGE_TYPE if the message type does not match
		 *   STATUS_E_DATA_SIZE if the data size is wrong
		 *   STATUS_E_IMMEDIATE if the message is marked as immediate
		 *   STATUS_E_MOTOR_1_DIRECTION if the direction of the motor 1
		 *    is neither 0 nor 1
		 *   STATUS_E_MOTOR_2_DIRECTION if the direction of the motor 2
		 *    is neither 0 nor 1
		 */
		MessageStatus validate () const throw ();

  		/**
		 * Returns the ID of the task associated with this message, or
		 * zero if there is no such task
		 */
		UInt16 getTaskId () const throw ()
		{
			return m_msg.getTaskId();
		}

		/**
		 * Returns the direction in which the motor signal is applied
		 *
		 * @return 0 for backward, 1 for forward
		 */
		UInt8 getMotor1Direction () const throw ();

		/**
		 * Returns the signal strength for the drive of the motor 1
		 * 
		 * This value is what is applied to the PWM pin driving the
		 * base of the transistor which controls the voltage delivered
		 * to the motor.
		 *
		 * @return a value representing the signal strength
		 */
		UInt8 getMotor1Signal () const throw ();

		/**
		 * Returns the direction in which the motor signal is applied
		 *
		 * @return 0 for backward, 1 for forward
		 */
		UInt8 getMotor2Direction () const throw ();

		/**
		 * Returns the signal strength for the drive of the motor 2
		 * 
		 * This value is what is applied to the PWM pin driving the
		 * base of the transistor which controls the voltage delivered
		 * to the motor.
		 *
		 * @return a value representing the signal strength
		 */
		UInt8 getMotor2Signal () const throw ();

	private:

		enum
		{
			OFFSET_MOTOR_1_DIRECTION = 0,
			OFFSET_MOTOR_1_SIGNAL = 1,
			OFFSET_MOTOR_2_DIRECTION = 2,
			OFFSET_MOTOR_2_SIGNAL = 3,
			DATA_SIZE = 4
		};

		Message m_msg;
	};

} } }

#endif

