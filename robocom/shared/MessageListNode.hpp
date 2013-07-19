#ifndef ROBOCOM_SHARED_MESSAGE_LIST_NODE_HPP
#define ROBOCOM_SHARED_MESSAGE_LIST_NODE_HPP

#include "Message.hpp"

namespace robocom {
namespace shared
{

	/**
	 * This class implements a node of a single list that contains
	 * a Message object
	 *
	 * Classes that maintain collections of Message objects, such as
	 * MessagePool and MessageQueue manage those collections by linking
	 * them in a single list.
	 */
	class MessageListNode
	{
	public:

		/// @name Accessors
		///@{

		/**
		 * Returns the pointer to the next node in the list
		 */
		MessageListNode* getNext () const throw ()
		{
			return m_p_next;
		}

		/**
		 * Returns the stored message
		 */
		const Message& getMessage () const throw ()
		{
			return m_msg;
		}

		///@}


		/// @name Mutators
		///@{

		/**
		 * Sets the pointer to the next node in the list
		 */
		void setNext (MessageListNode* p_next) throw ()
		{
			m_p_next = p_next;
		}

		/**
		 * Sets the message stored in this node
		 */
		void setMessage (const Message& msg) throw ()
		{
			m_msg = msg;
		}

		///@}

	private:

		MessageListNode* m_p_next;
		Message m_msg;
	};

} }



#endif
