#ifndef ROBOCOM_SHARED_MSG_SET_SERVO_ANGLE_REQUEST_HPP
#define ROBOCOM_SHARED_MSG_SET_SERVO_ANGLE_REQUEST_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class represents the request to change the rotation angle of a servo
	 */
	class SetServoAngleRequest
	{
	public:

		/// The message type for instances of this class
		enum { MSGID = RobocomMessageTypes::MSGID_SERVO_ANGLE };

		/**
		 * Constructor for a delayed-execution message
		 */
		SetServoAngleRequest (
			UInt16 task_id,
			UInt32 current_millis,
			UInt8 servo_id,
			UInt8 angle
		) throw ();

		/**
		 * Constructor for an immediate-execution message
		 */
		SetServoAngleRequest (
			UInt16 task_id,
			UInt8 servo_id,
			UInt8 angle
		) throw ();

		/**
		 * Constructs a SetServoAngleRequest object from the given message
		 */
		explicit SetServoAngleRequest (const Message& msg) throw ();

		/**
		 * Copies state from the given message into this object
		 */
		SetServoAngleRequest& operator= (const Message& msg) throw ();

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
		 * Returns the ID of the servo
		 *
		 * @return 0-based ID
		 */
		UInt8 getServoId () const throw ();

		/**
		 * Returns the current angle of rotation
		 * 
		 * @return a value between 0 and 180 (inclusive) representing the
		 *   rotation angle
		 */
		UInt8 getAngle () const throw ();

	private:

		enum
		{
			OFFSET_SERVO_ID = 0,
			OFFSET_ANGLE = 1,
			DATA_SIZE = 2
		};

		Message m_msg;
	};

} } }

#endif

