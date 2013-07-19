#include "../MessagePool.hpp"

#include "../MessageQueue.hpp"

namespace robocom {
namespace shared
{

	MessageQueue::MessageQueue (MessagePool& pool) throw ()
		: m_p_pool( & pool )
		, m_p_head( 0 )
		, m_size( 0 )
		, m_max_size( 0 )
	{ }


	bool
	MessageQueue::push (const Message& msg) throw ()
	{
		// Allocate a slot for the new mesage
		MessageListNode* p_node = m_p_pool->alloc();

		// No more free slots... have to ignore the message
		if ( 0 == p_node ) {
			return false;
		}
		// Link the new command at the right position according
		// to their ordering

		MessageListNode* p1 = 0;
		MessageListNode* p2 = m_p_head;
		while ( p2 != 0 &&
				p2->getMessage().comparePriority( msg ) <= 0 )
		{
			p1 = p2;
			p2 = p2->getNext();
		}

		if ( 0 == p1 ) {
			m_p_head = p_node;
		}
		else {
			p1->setNext( p_node );
		}

		p_node->setNext( p2 );
		p_node->setMessage( msg );

		if ( ++ m_size > m_max_size ) {
			m_max_size = m_size;
		}

		return true;
	}


	bool
	MessageQueue::pop (Message& msg, UInt32 current_millis) throw ()
	{
		// No commands in the queue...
		if ( 0 == m_p_head ) {
			return false;
		}

		// Check if the time has come for the head command to execute
		const Message& head = m_p_head->getMessage();
		if ( ! head.isImmediate() && head.getMillis() > current_millis ) {
			return false;
		}

		// Unlink the command from the queue
		MessageListNode* const p_node = m_p_head;
		m_p_head = m_p_head->getNext();
		m_size--;

		// Copy the command to the user supplied memory
		msg = p_node->getMessage();

		// Return the list node back to the pool
		m_p_pool->free( p_node );

		return true;
	}


} }
