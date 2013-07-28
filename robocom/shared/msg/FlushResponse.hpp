#ifndef ROBOCOM_SHARED_MSG_FLUSH_RESPONSE_HPP
#define ROBOCOM_SHARED_MSG_FLUSH_RESPONSE_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class represents the response that follows messages flushed
	 * from arduino to the client.
	 */
	class FlushResponse
	{
	public:

		/// The message type for instances of this class
		enum { MSGID = CommonMessageTypes::MSGID_FLUSH };

		/**
		 * Constructor
		 */
		FlushResponse (
			UInt16 task_id,
			UInt32 current_millis,
			UInt8 min_free_slots,
			UInt8 free_slots,
			UInt8 input_queue_max_size,
			UInt8 input_queue_size,
			UInt8 output_queue_max_size,
			UInt8 output_queue_size
		) throw ();

		/**
		 * Constructs a FlushResponse object from the given message
		 */
		explicit FlushResponse (const Message& msg) throw ();

		/**
		 * Copies state from the given message into this object
		 */
		FlushResponse& operator= (const Message& msg) throw ();

		/**
		 * Returns the representation of this object state as a Message
		 * instance
		 */
		const Message& asMessage () const throw ();

		/**
		 * Returns whether the data stored in this object is valid
		 * and consistent
		 *
		 * Clients should not attempt to interpret the message if
		 * this function returns an error value
		 *
		 * @return STATUS_OK if data is valid
		 *   STATUS_E_MESSAGE_TYPE if the message type does not match
		 *   STATUS_E_DATA_SIZE if the data size is wrong
		 *   STATUS_E_IMMEDIATE if the message is marked as immediate
		 *   STATUS_E_FREE_SLOTS if the min number of free slots
		 *    is larger than the curent number of free slots
		 *   STATUS_E_INPUT_QUEUE_SIZE if the current size of the input
		 *    queue is larger than the maximum size
		 *   STATUS_E_OUTPUT_QUEUE_SIZE if the current size of the output
		 *    queue is larger than the maximum size
		 */
		MessageStatus validate () const throw ();

  		/**
		 * Returns the ID of the task associated with this message, or
		 * zero if there is no such task
		 */
		UInt16 getTaskId () const throw ()
		{
			return m_msg.getTaskId();
		}

		/**
		 * Returns the minimum number of free message slots since
		 * last reset
		 *
		 * This number denotes how many messages arduino can still
		 * put in the input or output queue. When this number drops to zero
		 * arduino stops accepting new non-immediate messages until
		 * the output queue is drained to free up some slots.
		 */
		UInt8 getMinFreeSlots () const throw ();

		/**
		 * Returns the current number of free message slots
		 *
		 * This number denotes how many messages arduino can still
		 * put in the input or output queue. When this number drops to zero
		 * arduino stops accepting new non-immediate messages until
		 * the output queue is drained to free up some slots.
		 */
		UInt8 getFreeSlots () const throw ();

		/**
		 * Returns the maximum size of the input queue since last reset
		 *
		 * The input queue stores messages which execution has been delayed
		 */
		UInt8 getInputQueueMaxSize () const throw ();

		/**
		 * Returns the current size of the input queue
		 *
		 * The input queue stores messages which execution has been delayed
		 */
		UInt8 getInputQueueSize () const throw ();

		/**
		 * Returns the maximum size of the output queue since last reset
		 *
		 * The output queue stores messages awaiting transfer to the client
		 */
		UInt8 getOutputQueueMaxSize () const throw ();

		/**
		 * Returns the current size of the output queue
		 *
		 * The output queue stores messages awaiting transfer to the client
		 */
		UInt8 getOutputQueueSize () const throw ();

	private:

		enum
		{
			OFFSET_MIN_FREE_SLOTS = 0,
			OFFSET_FREE_SLOTS = 1,
			OFFSET_INPUT_QUEUE_MAX_SIZE = 2,
			OFFSET_INPUT_QUEUE_SIZE = 3,
			OFFSET_OUTPUT_QUEUE_MAX_SIZE = 4,
			OFFSET_OUTPUT_QUEUE_SIZE = 5,
			DATA_SIZE = 6
		};

		Message m_msg;
	};

} } }

#endif

