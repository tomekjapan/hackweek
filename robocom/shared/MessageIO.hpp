#ifndef ROBOCOM_SHARED_MESSAGE_IO_HPP
#define ROBOCOM_SHARED_MESSAGE_IO_HPP

#include "shared_base.hpp"

namespace robocom {
namespace shared
{

	/**
	 * This class implements reading and writing of Messages
	 * over the arduino Stream interface.
	 *
	 * Each message starts with the start byte ('>') and ends with the
	 * end byte ('<'). Between the start byte and the end byte come the
	 * message data. The first byte of the message data is the size of that
	 * data (includes the size byte, but does not include the start and
	 * end bytes).
	 */
	class MessageIO
	{
	public:


		/// @name Lifetime management
		///@{

		enum MagicConstants
		{
			MC_MESSAGE_START = '>',
			MC_MESSAGE_END = '<'
		};

		///@}


		/// @name Lifetime management
		///@{
		
		/**
		 * Creates a new instance that will use the given stream for
		 * communication.
		 *
		 * @param stream the reference to the Stream instance to use
		 */
		MessageIO (StreamIO& stream) throw ();

		///@}


		/// @name Methods
		///@{

		/**
		 * Attempts to read a message from the communication stream.
		 *
		 * If this function throws, the message could have been read
		 * only partially and will be in an undefined state.
		 *
		 * @param msg on output stores the message that has been read from
		 *  the stream
		 *
		 * @return true if the message was read, false if no message is yet
		 *   fully available
		 */
		bool read (Message& msg);

		/**
		 * Writes the given message to the communication stream.
		 *
		 * This function will block until the whole message data has
		 * been written. Since the output buffer on arduino holds only
		 * 64 bytes, the microcontroller can quickly lock itself out
		 * if messages are written without anyone listening.
		 *
		 * If this function throws an exception, the message might have not
		 * been completely written.
		 *
		 * @param msg the message to write
		 */
		void write (const Message& msg);

		///@}

	private:

		enum IOState
		{
			// When the IO is in this state, it discards all incoming
			// bytes until it finds the byte marking the beginning of
			// the message.
			IOS_NEED_MESSAGE_START,
			
			// In this state, the IO received the byte marking the
			// beginning of the message, but not enough bytes are available
			// in the input buffer to read the whole message.
			IOS_NEED_DATA,

			// In this state, the IO received the byte marking the
			// beginning of the message and there are enough bytes in the
			// input buffer to read the whole message.
			IOS_HAVE_DATA
		};

		bool _readMessageStart ();
		bool _waitForDataAvailable ();
		bool _readData (Message& msg);

		StreamIO& m_stream;
		IOState m_state;
	};

} }

#endif // ROBOCOM_SHARED_MESSAGE_IO_HPP
