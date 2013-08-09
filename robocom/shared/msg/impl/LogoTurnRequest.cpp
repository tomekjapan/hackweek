#include "../LogoTurnRequest.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	LogoTurnRequest::LogoTurnRequest (
		UInt16 task_id,
		UInt32 current_millis,
		UInt8 direction,
		UInt16 angle
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setMillis( current_millis );
		m_msg.setUInt8( OFFSET_DIRECTION, direction );
		m_msg.setUInt16( OFFSET_ANGLE, angle );
	}


	LogoTurnRequest::LogoTurnRequest (
		UInt16 task_id,
		UInt8 direction,
		UInt16 angle
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setImmediate();
		m_msg.setUInt8( OFFSET_DIRECTION, direction );
		m_msg.setUInt16( OFFSET_ANGLE, angle );
	}


	LogoTurnRequest::LogoTurnRequest (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	LogoTurnRequest&
	LogoTurnRequest::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	LogoTurnRequest::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	LogoTurnRequest::validate () const throw ()
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
	LogoTurnRequest::getDirection () const throw ()
	{
		return m_msg.getUInt8( OFFSET_DIRECTION );
	}


	UInt16
	LogoTurnRequest::getAngle () const throw ()
	{
		return m_msg.getUInt16( OFFSET_ANGLE );
	}

} } }

