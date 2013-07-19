#include "../MessagePool.hpp"

namespace robocom {
namespace shared
{

	MessagePool::MessagePool () throw ()
		: m_p_free ( m_buffer )
		, m_free_count( SLOT_COUNT )
		, m_min_free_count( SLOT_COUNT )
	{
		for ( int i = 1; i < SLOT_COUNT; i++ ) {
			m_buffer[i-1].setNext( m_buffer + i );
		}
		m_buffer[SLOT_COUNT-1].setNext( 0 );
	}


	MessageListNode*
	MessagePool::alloc () throw ()
	{
		// No more free slots... have to ignore
		if ( 0 == m_p_free ) {
			return 0;
		}

		if ( -- m_free_count < m_min_free_count ) {
			m_min_free_count = m_free_count;
		}

		MessageListNode* const p_node = m_p_free;
		m_p_free = m_p_free->getNext();

		p_node->setNext( 0 );
		return p_node;
	}


	void
	MessagePool::free (MessageListNode* p_node) throw ()
	{
		p_node->setNext( m_p_free );
		m_p_free = p_node;
		m_free_count++;
	}

} }
