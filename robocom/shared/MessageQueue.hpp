#ifndef ROBOCOM_SHARED_MESSAGE_QUEUE_HPP
#define ROBOCOM_SHARED_MESSAGE_QUEUE_HPP

#include "shared_base.hpp"

namespace robocom {
namespace shared
{

	/**
	 * This class implements a queue of messages
	 *
	 * Messages are ordered by their millis, with the ones marked for
	 * immediate execution coming before other messages.
	 */
	class MessageQueue
	{
	public:

		/// @name Lifetime management
		///@{

		/**
		 * Creates a new Message queue that will use the specified
		 * pool for message allocation
		 */
		MessageQueue (MessagePool& pool) throw ();

		///@}


		/// @name Accessors
		///@{

		/**
		 * Returns the number of messages in this object
		 */
		UInt8 getSize () const throw ()
		{
			return m_size;
		}

		/**
		 * Returns the maximum size of this object since its creation
		 */
		UInt8 getMaxSize () const throw ()
		{
			return m_max_size;
		}

		///@}


		/// @name Methods
		///@{

		/**
		 * Discards all messages from this queue
		 */
		void clear () throw ();

		/**
		 * Adds the given message to this queue
		 *
		 * @param msg the message to add
		 *
		 * @return true if the message was added, false if there is no more
		 *   free space in the message pool and the message was not added
		 */
		bool push (const Message& msg) throw ();

		/**
		 * Gets the message from the head of this queue and removes it
		 *
		 * Messages marked for immediate execution are retrieved before the
		 * others, then messages not marked for immediate execution are
		 * prioritized by their millis value.
		 *
		 * @param msg on output stores the retrieved message
		 * @param current_millis the current millis value
		 *
		 * @return true if the message was retrieved, false if the message
		 *   was not removed because the queue was empty
		 */
		bool pop (Message& msg, UInt32 current_millis) throw ();

		///@}

	private:

		MessagePool* m_p_pool;
		MessageListNode* m_p_head;
		UInt8 m_size;
		UInt8 m_max_size;
	};

} }

#endif
