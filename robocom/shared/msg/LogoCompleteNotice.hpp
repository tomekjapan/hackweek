#ifndef ROBOCOM_SHARED_MSG_LOGO_COMPLETE_NOTICE_HPP
#define ROBOCOM_SHARED_MSG_LOGO_COMPLETE_NOTICE_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class represents a noticed sent to the controlling computer
	 * that a logo command finished execution and the turtle is ready
	 * to accept the next logo command
	 */
	class LogoCompleteNotice
	{
	public:

		/// The message type for instances of this class
		enum { MSGID = RobocomMessageTypes::MSGID_LOGO_COMPLETE };

		/**
		 * Constructor for a delayed-execution message
		 *
		 * @param status 0 if execution was successful, non-zero in case of
		 *   an error
		 */
		LogoCompleteNotice (
			UInt16 task_id,
			UInt32 current_millis,
			UInt8 status
		) throw ();

		/**
		 * Constructor for an immediate-execution message
		 *
		 * @param status 0 if execution was successful, non-zero in case of
		 *   an error
		 */
		LogoCompleteNotice (
			UInt16 task_id,
			UInt8 status
		) throw ();

		/**
		 * Constructs a LogoCompleteNotice object from the given message
		 */
		explicit LogoCompleteNotice (const Message& msg) throw ();

		/**
		 * Copies state from the given message into this object
		 */
		LogoCompleteNotice& operator= (const Message& msg) throw ();

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
		 * Returns the status of the logo command execution
		 *
		 * @return 0 if command executed successfully, non-zero if
		 *   there was an error
		 */
		UInt8 getStatus () const throw ();

	private:

		enum
		{
			OFFSET_STATUS = 0,
			DATA_SIZE = 1
		};

		Message m_msg;
	};

} } }

#endif

