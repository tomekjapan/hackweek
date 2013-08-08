#include "../LogoCompleteNotice.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	LogoCompleteNotice::LogoCompleteNotice (
		UInt16 task_id,
		UInt32 current_millis,
		UInt8 status
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setMillis( current_millis );
		m_msg.setUInt8( OFFSET_STATUS, status );
	}


	LogoCompleteNotice::LogoCompleteNotice (
		UInt16 task_id,
		UInt8 status
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setTaskId( task_id );
		m_msg.setImmediate();
		m_msg.setUInt8( OFFSET_STATUS, status );
	}


	LogoCompleteNotice::LogoCompleteNotice (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	LogoCompleteNotice&
	LogoCompleteNotice::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	LogoCompleteNotice::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	LogoCompleteNotice::validate () const throw ()
	{
		if ( m_msg.getMessageType() != MSGID ) {
			return STATUS_E_MESSAGE_TYPE;
		}

		if ( m_msg.getDataSize() != DATA_SIZE ) {
			return STATUS_E_DATA_SIZE;
		}

		return STATUS_OK;
	}


	UInt8
	LogoCompleteNotice::getStatus () const throw ()
	{
		return m_msg.getUInt8( OFFSET_STATUS );
	}

} } }

