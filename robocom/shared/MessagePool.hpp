#ifndef ROBOCOM_SHARED_MESSAGE_POOL_HPP
#define ROBOCOM_SHARED_MESSAGE_POOL_HPP

#include "shared_base.hpp"

#include "MessageListNode.hpp"


namespace robocom {
namespace shared
{

	/**
	 * This class implements a pool of Message objects
	 * 
	 * It serves as a subsitute for dynamic memory allocation which
	 * does not exist on Arduino. The pool supplies a fixed number
	 * of reusable objects.
	 */
	class MessagePool
	{
	public:

		/// @name Exported Constants
		///@{

		enum
		{
			/**
			 * The maximum number of messages that one pool can
			 * accomodate
			 */
			SLOT_COUNT = 32
		};

		///@}
		

		/// @name Lifetime management
		///@{

		/**
		 * Creates a new pool of messages
		 */
		MessagePool () throw ();

		///@}


		/// @name Accessors
		///@{

		/**
		 * Returns the number of free slots
		 */
		UInt8 getFree () const throw ()
		{
			return m_free_count;
		}

		/**
		 * Returns the minimum number of free slots since creation of
		 * this object.
		 */
		UInt8 getMinFree () const throw ()
		{
			return m_min_free_count;
		}

		///@}


		/// @name Methods
		///@{

		/**
		 * Allocates one Message object.
		 *
		 * @return the pointer to the Message, or NULL if there are no
		 *  free slots left
		 */
		MessageListNode* alloc () throw ();

		/**
		 * Releases one Message object.
		 *
		 * @param p_msg the message to release
		 */
		void free (MessageListNode* p_node) throw ();

		///@}

	private:

		MessageListNode m_buffer[SLOT_COUNT];
		MessageListNode* m_p_free;
		UInt8 m_free_count;
		UInt8 m_min_free_count;

	};

} }

#endif
