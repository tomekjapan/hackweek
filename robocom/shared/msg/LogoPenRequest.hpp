#ifndef ROBOCOM_SHARED_MSG_LOGO_PEN_REQUEST_HPP
#define ROBOCOM_SHARED_MSG_LOGO_PEN_REQUEST_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class represents the request to move the turtle robot's
	 * pen up or down
	 */
	class LogoPenRequest
	{
	public:

		/// The message type for instances of this class
		enum { MSGID = RobocomMessageTypes::MSGID_LOGO_PEN };

		/**
		 * Constructor for a delayed-execution message
		 *
		 * @param direction 0 for pen up, 1 for pen down
		 */
		LogoPenRequest (
			UInt16 task_id,
			UInt32 current_millis,
			UInt8 direction
		) throw ();

		/**
		 * Constructor for an immediate-execution message
		 *
		 * @param direction 0 for pen up, 1 for pen down
		 */
		LogoPenRequest (
			UInt16 task_id,
			UInt8 direction
		) throw ();

		/**
		 * Constructs a LogoPenRequest object from the given message
		 */
		explicit LogoPenRequest (const Message& msg) throw ();

		/**
		 * Copies state from the given message into this object
		 */
		LogoPenRequest& operator= (const Message& msg) throw ();

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
		 *   STATUS_E_DIRECTION if the pen move direction is invalid
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
		 * Returns the direction to move the pen
		 *
		 * @return 0 for pen up, 1 for pen down
		 */
		UInt8 getDirection () const throw ();

	private:

		enum
		{
			OFFSET_DIRECTION = 0,
			DATA_SIZE = 1
		};

		Message m_msg;
	};

} } }

#endif

