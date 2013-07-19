#include "../FlushResponse.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	FlushResponse::FlushResponse (
		UInt32 current_millis,
		UInt8 min_free_slots,
		UInt8 free_slots,
		UInt8 input_queue_max_size,
		UInt8 input_queue_size,
		UInt8 output_queue_max_size,
		UInt8 output_queue_size
	) throw ()
		: m_msg( )
	{
		m_msg.clear();
		m_msg.setMessageType( MSGID );
		m_msg.setDataSize( DATA_SIZE );
		m_msg.setMillis( current_millis );
		m_msg.setUInt8( OFFSET_MIN_FREE_SLOTS, min_free_slots );
		m_msg.setUInt8( OFFSET_FREE_SLOTS, free_slots );
		m_msg.setUInt8( OFFSET_INPUT_QUEUE_MAX_SIZE, input_queue_max_size );
		m_msg.setUInt8( OFFSET_INPUT_QUEUE_SIZE, input_queue_size );
		m_msg.setUInt8( OFFSET_OUTPUT_QUEUE_MAX_SIZE, output_queue_max_size );
		m_msg.setUInt8( OFFSET_OUTPUT_QUEUE_SIZE, output_queue_size );
	}


	FlushResponse::FlushResponse (
		const Message& msg
	) throw ()
		: m_msg( msg )
	{
	}


	FlushResponse&
	FlushResponse::operator= (const Message& msg) throw ()
	{
		m_msg = msg;
		return *this;
	}


	const Message&
	FlushResponse::asMessage () const throw ()
	{
		return m_msg;
	}


	MessageStatus
	FlushResponse::validate () const throw ()
	{
		if ( m_msg.getMessageType() != MSGID ) {
			return STATUS_E_MESSAGE_TYPE;
		}

		if ( m_msg.getDataSize() != DATA_SIZE ) {
			return STATUS_E_DATA_SIZE;
		}

		if ( m_msg.isImmediate() ) {
			return STATUS_E_IMMEDIATE;
		}

		if ( getMinFreeSlots() > getFreeSlots() ) {
			return STATUS_E_FREE_SLOTS;
		}

		if ( getInputQueueSize() > getInputQueueMaxSize() ) {
			return STATUS_E_INPUT_QUEUE_SIZE;
		}

		if ( getOutputQueueSize() > getOutputQueueMaxSize() ) {
			return STATUS_E_OUTPUT_QUEUE_SIZE;
		}

		return STATUS_OK;
	}


	UInt8
	FlushResponse::getMinFreeSlots () const throw ()
	{
		return m_msg.getUInt8( OFFSET_MIN_FREE_SLOTS );
	}


	UInt8
	FlushResponse::getFreeSlots () const throw ()
	{
		return m_msg.getUInt8( OFFSET_FREE_SLOTS );
	}


	UInt8
	FlushResponse::getInputQueueMaxSize () const throw ()
	{
		return m_msg.getUInt8( OFFSET_INPUT_QUEUE_MAX_SIZE );
	}


	UInt8
	FlushResponse::getInputQueueSize () const throw ()
	{
		return m_msg.getUInt8( OFFSET_INPUT_QUEUE_SIZE );
	}


	UInt8
	FlushResponse::getOutputQueueMaxSize () const throw ()
	{
		return m_msg.getUInt8( OFFSET_OUTPUT_QUEUE_MAX_SIZE );
	}


	UInt8
	FlushResponse::getOutputQueueSize () const throw ()
	{
		return m_msg.getUInt8( OFFSET_OUTPUT_QUEUE_SIZE );
	}

} } }

