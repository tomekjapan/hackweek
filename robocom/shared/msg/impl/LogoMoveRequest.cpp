#include "../LogoMoveRequest.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	LogoMoveRequest::LogoMoveRequest (
		UInt16 task_id,
		UInt32 current_millis,
		UInt8 direction,
		UInt16 distance
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setMillis( current_millis );
		m_msg.setUInt8( OFFSET_DIRECTION, direction );
		m_msg.setUInt16( OFFSET_DISTANCE, distance );
	}


	LogoMoveRequest::LogoMoveRequest (
		UInt16 task_id,
		UInt8 direction,
		UInt16 distance
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setImmediate();
		m_msg.setUInt8( OFFSET_DIRECTION, direction );
		m_msg.setUInt16( OFFSET_DISTANCE, distance );
	}


	LogoMoveRequest::LogoMoveRequest (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	LogoMoveRequest&
	LogoMoveRequest::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	LogoMoveRequest::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	LogoMoveRequest::validate () const throw ()
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
	LogoMoveRequest::getDirection () const throw ()
	{
		return m_msg.getUInt8( OFFSET_DIRECTION );
	}


	UInt16
	LogoMoveRequest::getDistance () const throw ()
	{
		return m_msg.getUInt16( OFFSET_DISTANCE );
	}

} } }

