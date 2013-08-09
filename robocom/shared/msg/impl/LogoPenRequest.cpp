#include "../LogoPenRequest.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	LogoPenRequest::LogoPenRequest (
		UInt16 task_id,
		UInt32 current_millis,
		UInt8 direction
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setMillis( current_millis );
		m_msg.setUInt8( OFFSET_DIRECTION, direction );
	}


	LogoPenRequest::LogoPenRequest (
		UInt16 task_id,
		UInt8 direction
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setImmediate();
		m_msg.setUInt8( OFFSET_DIRECTION, direction );
	}


	LogoPenRequest::LogoPenRequest (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	LogoPenRequest&
	LogoPenRequest::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	LogoPenRequest::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	LogoPenRequest::validate () const throw ()
	{
		if ( m_msg.getMessageType() != MSGID ) {
			return STATUS_E_MESSAGE_TYPE;
		}

		if ( m_msg.getDataSize() != DATA_SIZE ) {
			return STATUS_E_DATA_SIZE;
		}

		if ( getDirection() != 0 && getDirection() != 1 ) {
			return STATUS_E_DIRECTION;
		}

		return STATUS_OK;
	}


	UInt8
	LogoPenRequest::getDirection () const throw ()
	{
		return m_msg.getUInt8( OFFSET_DIRECTION );
	}

} } }

