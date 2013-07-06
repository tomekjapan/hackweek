#ifndef ROBOCOM_SHARED_MESSAGE_HPP
#define ROBOCOM_SHARED_MESSAGE_HPP

#include "common/Base.hpp"

namespace robocom {
namespace shared
{

	/**
	 * This class implements the message layer for the communication
	 * protocol with arduino
	 *
	 * Message exchange is initiated by the client (PC). One message is sent
	 * from the client to arduino and as a result zero or more messages can
	 * be send back from arduino to the client.
	 *
	 * Messages are sent to arduino to request execution of some tasks.
	 * An example task might involve gathering  information about system
	 * information such as available memory, current time, battery state,
	 * and sending back a message containing the data. Another task
	 * could be to periodically check reading of a sensor and send
	 * back a message each time new measurement data are available. Yet
	 * another task could be to just return messages accumulated so far
	 * on arduino (since they can only be returned in response to the client
	 * request).
	 *
	 * Each message has a task ID, a message type and a payload. The task ID
	 * allows the client and arduino to assemble related messages into a
	 * conversation. The message type describes the meaning and content
	 * of a particular message, and the payload just carries the message
	 * data. The payload content is determined by the message type.
	 *
	 * Messages on arduino are placed in a queue and are typically processed in
	 * the order they arrive. Arduino has a fairly short queue - only 32
	 * messages can be cumulatively queued in its input and output queues.
	 * When the limit is reached, arduino wil will discard any further messages
	 * with the exception of immediate messages described below.
	 *
	 * Some of the messages are processed immediately - they are defined for
	 * all applications in order to facilitate the conversation flow. They
	 * are: RESET, FLUSH, ECHO and NOOP.
	 *
	 * The RESET message will cause arduino to discard all pending tasks and
	 * clear all message queues. The client will typically send it at
	 * startup to clean up anything accumulated on arduino.
	 *
	 * The FLUSH message will cause arduino to write out all responses that
	 * it accumulated in its outgoing queue, followed by a response to the
	 * FLUSH message. The client should send the FLUSH message periodically
	 * in order to drain the queue which on arduino is limited in size due
	 * to it relatively small amount of RAM. The client will send the FLUSH
	 * message, then it will read and process all incoming messages until
	 * it receives a message with the same task ID as the message that
	 * originated the conversation.
	 *
	 * The ECHO message will cause a copy of the same message to be sent
	 * back immediately without draining the output queue. The message
	 * can be used to check if arduino is ready.
	 *
	 * Message execution millis
	 *
	 * A message sent from the client may include time given in millis
	 * of arduino, at which the message should be executed. This way it is
	 * possible to schedule execution at some point in future. Arduino
	 * may also use this mechanism for tasks involving periodic execution
	 * of some actions.
	 *
	 * A message can be marked for immediate execution, by setting a bit
	 * in the byte that carries the message type. If that bit is set,
	 * arduino will not look at the millis value and will put the message
	 * at the front of the queue, only behind other messages scheduled
	 * for immediate execution.
	 *
	 * The mechanism for specification of execution time is application
	 * agnostic, and thus is implemented at the message layer.
	 */
	class Message
	{
	public:

		/// @name Exported Constants
		///@{

		enum
		{
			/**
			 * Maximum size of the payload data that can be included
			 * in one message. If this message is not marked for
			 * immediate execution, the maximum payload size is
			 * decreased by 4 bytes which are needed to store the
			 * execution millis.
			 */
			MAX_DATA_SIZE = 16,

			/**
			 * Maximum value of the message type identifier.
			 */
			MAX_MESSAGE_TYPE = 0x7F
		};

		///@}


		/// @name Accessors
		///@{

		/**
		 * Returns the current size of the payload in this message
		 */
		UInt8 getDataSize () const throw ();

		/**
		 * Returns the maximum size of the payload that can fit in this
		 * message
		 *
		 * The maximum size is determined by whether this message is marked
		 * for immediate execution or not. If not, the data size will be
		 * reduced by 4 bytes necessary to store the desired processing millis.
		 */
		UInt8 getMaxDataSize () const throw ();

  		/**
		 * Returns the ID of the task associated with this message, or
		 * zero if there is no such task
		 */
		UInt16 getTaskId () const throw ();

  		/**
		 * Returns the type of the message
		 */
		UInt8 getMessageType () const throw ();

		/**
		 * Returns whether this message carries a message that should be
		 * executed as soon as possible, and not at some point in the future.
		 *
		 * Messages marked as immediate will be prioritized for processing
		 * on arduino before messages not marked as immediate.
		 */
		bool isImmediate () const throw ();

		/**
		 * Returns millis in arduino at which this message should be
		 * processed
		 *
		 * If millis are smaller than the current millis on arduino,
		 * then the message will be executed without delay. However,
		 * messages will be prioritized by their millis value; messages
		 * with smaller values will be executed before the ones with
		 * larger values.
		 *
		 * @post ! isImmediate() || $result == 0
		 */
		UInt32 getMillis () const throw ();

		/**
		 * Returns an 8-bit integer stored in the data at the given offset
		 *
		 * @param offset the offset in the data to retrieve the value from
		 *
		 * @pre offset < getDataSize() - 1
		 */
		UInt8 getUInt8 (unsigned offset) const throw ();

		/**
		 * Returns a 16-bit integer stored in the data at the given offset
		 *
		 * @param offset the offset in the data to retrieve the value from
		 *
		 * @pre offset < getDataSize() - 2
		 */
		UInt16 getUInt16 (unsigned offset) const throw ();

		/**
		 * Returns a 32-bit integer stored in the data at the given offset
		 *
		 * @param offset the offset in the data to retrieve the value from
		 *
		 * @pre offset < getDataSize() - 4
		 */
		UInt32 getUInt32 (unsigned offset) const throw ();

		///@}


		/// @name Mutators
		///@{

		/**
		 * Sets the data size in this message.
		 *
		 * @pre size <= getMaxDataSize()
		 */
		void setDataSize (UInt8 size) throw ();

		/**
		 * Sets the ID of the task to associate with this message
		 *
		 * @param task_id the ID of the task, or zero to make this
		 *   message not associated with any tasks
		 */
		void setTaskId (UInt16 task_id) throw ();

		/**
		 * Sets the the message type to associate with this message
		 *
		 * @param message_type the message type identifier
		 *
		 * @pre message_type <= MAX_MESSAGE_TYPE
		 */
		void setMessageType (UInt8 message_type) throw ();

		/**
		 * Sets the immediate execution status of this message.
		 *
		 * Messages marked as immediate will be prioritized for processing
		 * on arduino before messages not marked as immediate.
		 *
		 * @post getMillis() == 0
		 * @post isImmediate() == 0
		 */
		void setImmediate () throw ();

		/**
		 * Sets the time in millis on arduino when this message should
		 * be procesed
		 *
		 * This call will clear the immediate execution flag on this object.
		 *
		 * Setting this to a value smaller than the current time on
		 * arduino will cause the message to be processed without delay.
		 * messages will be prioritized by their millis value; messages
		 * with smaller values will be executed before the ones with
		 * larger values.
		 *
		 * @param ms the value of millis in arduino time when the message
		 *   should be processed
		 *
		 * @pre getDataSize() <= MAX_DATA_SIZE - 4
		 * @post ! isImmediate()
		 */
		void setMillis (UInt32 ms) throw ();

		/**
		 * Stores an 8-bit integer in the data at the given offset
		 *
		 * @param offset the offset in the data to store the value
		 * @param value the value to store
		 *
		 * @pre offset <= getDataSize() - 1
		 */
		void setUInt8 (unsigned offset, UInt8 value) throw ();

		/**
		 * Stores an 16-bit integer in the data at the given offset
		 *
		 * @param offset the offset in the data to store the value
		 * @param value the value to store
		 *
		 * @pre offset <= getDataSize() - 2
		 */
		void setUInt16 (unsigned offset, UInt16 value) throw ();

		/**
		 * Stores an 32-bit integer in the data at the given offset
		 *
		 * @param offset the offset in the data to store the value
		 * @param value the value to store
		 *
		 * @pre offset <= getDataSize() - 4
		 */
		void setUInt32 (unsigned offset, UInt32 value) throw ();

		///@}

	private:

		enum
		{
			// A flag of the _code indicating that the message
			// does not carry the millis
			IMMEDIATE_BIT = 0x80
		};

		UInt8 m_data_size;
		UInt8 m_message_type;
		UInt16 m_task_id;
		UInt8 m_data[MAX_DATA_SIZE];
	};

} }

#endif
