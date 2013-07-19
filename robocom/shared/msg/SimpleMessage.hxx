#ifndef ROBOCOM_SHARED_MSG_NOOP_REQUEST_HPP
#define ROBOCOM_SHARED_MSG_NOOP_REQUEST_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class template implements immediate messages that carry no data
	 */
	template <int ID> class SimpleMessage;

	/**
	 * This class encapsulates the do-nothing message
	 */
	typedef SimpleMessage<CommonMessageTypes::MSGID_NOOP> NoopRequest;

	/**
	 * This class represents the request to send the same message back
	 */
	typedef SimpleMessage<CommonMessageTypes::MSGID_ECHO> EchoRequest;

	/**
	 * This class represents the responce to the echo request
	 */
	typedef SimpleMessage<CommonMessageTypes::MSGID_ECHO> EchoResponse;

	/**
	 * This class represents the request to flush pending out
	 * messages from arduino to the client.
	 */
	typedef SimpleMessage<CommonMessageTypes::MSGID_FLUSH> FlushRequest;

	/**
	 * This class represents the request to discard all pending tasks
	 */
	typedef SimpleMessage<CommonMessageTypes::MSGID_RESET> ResetRequest;


	template <int ID>
	class SimpleMessage
	{
	public:

		/**
		 * The message type ID
		 */
		enum { MSGID = ID };

		/**
		 * Constructor
		 */
		SimpleMessage () throw ()
			: m_msg( )
		{
			m_msg.clear();
			m_msg.setMessageType( MSGID );
			m_msg.setDataSize( DATA_SIZE );
			m_msg.setImmediate();
		}

		/**
		 * Constructs a SimpleMessage object from the given message
		 */
		explicit SimpleMessage (const Message& msg) throw ()
			: m_msg( msg )
		{ }

		/**
		 * Copies state from the given message into this object
		 */
		SimpleMessage& operator= (const Message& msg) throw ()
		{
			m_msg = msg;
			return *this;
		}

		/**
		 * Returns the representation of this object state as a Message
		 * instance
		 */
		const Message& asMessage () const throw ()
		{
			return m_msg;
		}

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
		 *   STATUS_E_NOT_IMMEDIATE if the message is not marked as immediate
		 */
		MessageStatus validate () const throw ()
		{
			if ( m_msg.getMessageType() != MSGID ) {
				return STATUS_E_MESSAGE_TYPE;
			}

			if ( m_msg.getDataSize() != DATA_SIZE ) {
				return STATUS_E_DATA_SIZE;
			}

			if ( ! m_msg.isImmediate() ) {
				return STATUS_E_NOT_IMMEDIATE;
			}

			return STATUS_OK;
		}

	private:

		enum
		{
			DATA_SIZE = 0
		};

		Message m_msg;
	};

} } }

#endif

