#ifndef ROBOCOM_SHARED_MSG_ENCODER_READING_NOTICE_HPP
#define ROBOCOM_SHARED_MSG_ENCODER_READING_NOTICE_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class represents a notice about an encoder reading
	 *
	 * If the client subscribed to readings from the encoder,
	 * an instance of this class are added to the output queue for each
	 * reading. If the client did not subscribe, no output messages will be
	 * generated.
	 */
	class EncoderReadingNotice
	{
	public:

		/// The message type for instances of this class
		enum { MSGID = RobocomMessageTypes::MSGID_ENCODER_READING };

		/**
		 * Constructor
		 *
		 * @param task_id
		 * @param current_millis
		 * @param encoder_id the ID of the target encoder; either 0 or 1
		 * @param total the index of the encoder tick
		 * @param measurement_us micros at the time of measurement
		 */
		EncoderReadingNotice (
			UInt16 task_id,
			UInt32 current_millis,
			UInt8 encoder_id,
			UInt32 tick_index,
			UInt32 measurement_us
		) throw ();

		/**
		 * Constructs a EncoderReadingNotice object from the given message
		 */
		explicit EncoderReadingNotice (const Message& msg) throw ();

		/**
		 * Copies state from the given message into this object
		 */
		EncoderReadingNotice& operator= (const Message& msg) throw ();

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
		 * Returns the index of the encoder tick associated with this
		 * measurement
		 *
		 * Encoders register "ticks" every 1/10th of the full wheel rotation,
		 * at which point they will write down the micros when the tick
		 * happened.
		 * 
		 * @return the tick index
		 */
		UInt32 getTickIndex () const throw ();

		/**
		 * Returns the micros at the time of this measurement
		 *
		 * @return the micros at the time of this measurement
		 */
		UInt32 getMeasurementMicros () const throw ();

	private:

		enum
		{
			OFFSET_ENCODER_ID = 0,
			OFFSET_TICK_INDEX = 1,
			OFFSET_MICROS = 5,
			DATA_SIZE = 9
		};

		Message m_msg;
	};

} } }

#endif

