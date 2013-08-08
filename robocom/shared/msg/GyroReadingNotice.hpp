#ifndef ROBOCOM_SHARED_MSG_GYRO_READING_NOTICE_HPP
#define ROBOCOM_SHARED_MSG_Gyro_READING_NOTICE_HPP

#include "../Message.hpp"

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * This class represents a notice about a gyro reading.
	 *
	 * If the client subscribed to readings from the gyro,
	 * an instance of this class are added to the output queue for each
	 * reading. If the client did not subscribe, no output messages will be
	 * generated.
	 */
	class GyroReadingNotice
	{
	public:

		/// The message type for instances of this class
		enum { MSGID = RobocomMessageTypes::MSGID_GYRO_READING };

		/**
		 * Constructor
		 *
		 * @param task_id
		 * @param current_millis
		 * @param yaw_degrees the measured yaw value, in degrees
		 * @param yaw_degrees the measured pitch value, in degrees
		 * @param yaw_degrees the measured roll value, in degrees
		 * @param measurement_us micros at the time of measurement
		 */
		GyroReadingNotice (
			UInt16 task_id,
			UInt32 current_millis,
			float yaw_degrees,
			float pitch_degrees,
			float roll_degrees,
			UInt32 measurement_us
		) throw ();

		/**
		 * Constructs a EncoderReadingNotice object from the given message
		 */
		explicit GyroReadingNotice (const Message& msg) throw ();

		/**
		 * Copies state from the given message into this object
		 */
		GyroReadingNotice& operator= (const Message& msg) throw ();

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
		 *   STATUS_E_ANGLE_RANGE if one of the angles is out of range
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
		 * Returns the yaw reading, in degrees
		 *
		 * @return the yaw reading, in degrees
		 */
		float getYawDegrees () const throw ();

		/**
		 * Returns the pitch reading, in degrees
		 *
		 * @return the pitch reading, in degrees
		 */
		float getPitchDegrees () const throw ();

		/**
		 * Returns the roll reading, in degrees
		 *
		 * @return the roll reading, in degrees
		 */
		float getRollDegrees () const throw ();

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

		static UInt16 _encodeAngle(float angle) throw () {
			return (UInt16)static_cast<SInt16>(angle * 100.0f);
		}

		static float _decodeAngle(UInt16 angle) throw () {
			return static_cast<float>((SInt16)angle) / 100.0f;
		}

		enum
		{
			OFFSET_YAW_DEGREES = 0,
			OFFSET_PITCH_DEGREES = 2,
			OFFSET_ROLL_DEGREES = 4,
			OFFSET_MICROS = 6,
			DATA_SIZE = 10
		};

		Message m_msg;
	};

} } }

#endif
