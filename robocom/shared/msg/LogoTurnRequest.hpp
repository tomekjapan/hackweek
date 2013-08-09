#ifndef ROBOCOM_SHARED_MSG_LOGO_TURN_REQUEST_HPP
#define ROBOCOM_SHARED_MSG_LOGO_TURN_REQUEST_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class represents the request to rotate the robot in
	 * place by the specified angle
	 */
	class LogoTurnRequest
	{
	public:

		/// The message type for instances of this class
		enum { MSGID = RobocomMessageTypes::MSGID_LOGO_TURN };

		/**
		 * Constructor for a delayed-execution message
		 *
		 * @param direction 0 for right turn, 1 for left turn
		 * @param angle the amount to turn in degrees
		 */
		LogoTurnRequest (
			UInt16 task_id,
			UInt32 current_millis,
			UInt8 direction,
			UInt16 angle
		) throw ();

		/**
		 * Constructor for an immediate-execution message
		 *
		 * @param direction 0 for right turn, 1 for left turn
		 * @param angle the amount to turn in degrees
		 */
		LogoTurnRequest (
			UInt16 task_id,
			UInt8 direction,
			UInt16 angle
		) throw ();

		/**
		 * Constructs a LogoTurnRequest object from the given message
		 */
		explicit LogoTurnRequest (const Message& msg) throw ();

		/**
		 * Copies state from the given message into this object
		 */
		LogoTurnRequest& operator= (const Message& msg) throw ();

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
		 *   STATUS_E_DIRECTION if the rotation direction is invalid
		 *     (must be 0 or 1)
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
		 * Returns the direction to turn
		 *
		 * @return 0 for turning right, 1 for turning left
		 */
		UInt8 getDirection () const throw ();

		/**
		 * Returns the angle by which to turn
		 * 
		 * @return the amount to rotate in degrees
		 */
		UInt16 getAngle () const throw ();

	private:

		enum
		{
			OFFSET_DIRECTION = 0,
			OFFSET_ANGLE = 1,
			DATA_SIZE = 3
		};

		Message m_msg;
	};

} } }

#endif

