#ifndef ROBOCOM_SHARED_MSG_ENCODER_READING_REQUEST_HPP
#define ROBOCOM_SHARED_MSG_ENCODER_READING_REQUEST_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class represents a request to subscribe to or unsubscribe from
	 * notifications about encoder readings
	 *
	 * After subscribing, each time the encoder registers a reading, a new
	 * message of type EncoderReadingNotice will be placed in the output queue.
	 * The message will have the task ID the same as the message that subscribed
	 * to the readings.
	 *
	 * Encoder readings will be reported until the client sends an unsubscribe
	 * EncoderReadingRequest.
	 */
	class EncoderReadingRequest
	{
	public:

		/// The message type for instances of this class
		enum { MSGID = RobocomMessageTypes::MSGID_ENCODER_READING };

		/**
		 * Constructor for a delayed-execution message
		 *
		 * @param task_id
		 * @param current_millis
		 * @param encoder_id the ID of the target encoder; either 0 or 1
		 * @param is_subscribe true for a request to subscribe to encoder
		 *   readings, false to unsubscribe
		 */
		EncoderReadingRequest (
			UInt16 task_id,
			UInt32 current_millis,
			UInt8 encoder_id,
			bool is_subscribe
		) throw ();

		/**
		 * Constructor for an immediate-execution message
		 *
		 * @param task_id
		 * @param encoder_id the ID of the target encoder; either 0 or 1
		 * @param is_subscribe true for a request to subscribe to encoder
		 *   readings, false to unsubscribe
		 */
		EncoderReadingRequest (
			UInt16 task_id,
			UInt8 encoder_id,
			bool is_subscribe
		) throw ();

		/**
		 * Constructs a EncoderReadingRequest object from the given message
		 */
		explicit EncoderReadingRequest (const Message& msg) throw ();

		/**
		 * Copies state from the given message into this object
		 */
		EncoderReadingRequest& operator= (const Message& msg) throw ();

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
		 *   STATUS_E_ENCODER_ID if the encoder ID is neither 0 nor 1
		 *   STATUS_E_IS_SUBSCRIBE if the subscribe flag is neither 0 nor 1
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
		 * Returns the ID of the target encoder
		 *
		 * @return the ID of the target encoder; either 0 or 1
		 */
		UInt8 getEncoderId () const throw ();

		/**
		 * Returns whether this object is a request to subscribe to or to
		 * unsubscribe from encoder readings
		 * 
		 * @return true to subscribe, false to unsubscribe
		 */
		bool getIsSubscribe () const throw ();

	private:

		enum
		{
			OFFSET_ENCODER_ID = 0,
			OFFSET_IS_SUBSCRIBE = 1,
			DATA_SIZE = 2
		};

		Message m_msg;
	};

} } }

#endif

