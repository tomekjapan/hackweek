#ifndef ROBOCOM_SHARED_MSG_LOGO_MOVE_REQUEST_HPP
#define ROBOCOM_SHARED_MSG_LOGO_MOVE_REQUEST_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class represents the request to move the robot by
	 * the desired number of ticks (1 tick is equal to the 1/10th of the
	 * wheel circumference).
	 */
	class LogoMoveRequest
	{
	public:

		/// The message type for instances of this class
		enum { MSGID = RobocomMessageTypes::MSGID_LOGO_MOVE };

		/**
		 * Constructor for a delayed-execution message
		 *
		 * @param direction 0 for forward move, 1 for backward move
		 * @param distance the amount to move in ticks
		 */
		LogoMoveRequest (
			UInt16 task_id,
			UInt32 current_millis,
			UInt8 direction,
			UInt16 distance
		) throw ();

		/**
		 * Constructor for an immediate-execution message
		 *
		 * @param direction 0 for forward move, 1 for backward move
		 * @param distance the amount to move in ticks
		 */
		LogoMoveRequest (
			UInt16 task_id,
			UInt8 direction,
			UInt16 distance
		) throw ();

		/**
		 * Constructs a LogoMoveRequest object from the given message
		 */
		explicit LogoMoveRequest (const Message& msg) throw ();

		/**
		 * Copies state from the given message into this object
		 */
		LogoMoveRequest& operator= (const Message& msg) throw ();

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
		 *   STATUS_E_DIRECTION if the move direction is invalid
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
		 * Returns the direction to move
		 *
		 * @return 0 for forward, 1 for backward
		 */
		UInt8 getDirection () const throw ();

		/**
		 * Returns the distance by which to move
		 * 
		 * @return the amount to move in ticks
		 */
		UInt16 getDistance () const throw ();

	private:

		enum
		{
			OFFSET_DIRECTION = 0,
			OFFSET_DISTANCE = 1,
			DATA_SIZE = 3
		};

		Message m_msg;
	};

} } }

#endif

