#ifndef ROBOCOM_SHARED_STREAM_IO_HPP
#define ROBOCOM_SHARED_STREAM_IO_HPP

#include "shared_base.hpp"

#if defined(AVR)
#include <Stream.h>
#endif

namespace robocom {
namespace shared
{

#if ! defined(AVR)

	/**
	 * The purpose of this interface is to mimic the Stream
	 * class of the Arduino libraries
	 *
	 * Implementations of this interface on the client side can
	 * be used for communication with Arduino or for testing.
	 *
	 * This interface only implements a subset of the Stream
	 * class necessary to support the communication.
	 */
	class StreamIO
	{
	public:

		/// @name Lifetime management
		///@{

		/**
		 * Creates a new instance
		 */
		StreamIO () throw ()
		{ }

		/**
		 * Destroys this object
		 *
		 * Virtual destructor ensures correct deletion via base class pointer
		 */
		virtual ~StreamIO () throw ()
		{ }

		///@}


		/// @name Methods
		///@{

		/**
		 * Gets the number of bytes available in the stream that
		 * have already arrived
		 *
		 * @return the number of bytes available to read
		 */
		virtual int available () = 0;

		/**
		 * Reads the first byte from the incoming stream without advancing
		 * to the next one
		 *
		 * The successive calls to peek will return the same value, as will
		 * the next call to read.
		 *
		 * @return the next byte of incoming data if available, or -1
		 *   if no data is available
		 */
		virtual int peek () = 0;

		/**
		 * Reads the next byte from the incoming stream
		 *
		 * @return the next byte of incoming data if available, or -1
		 *   if no data is available
		 */
		virtual int read () = 0;

		/**
		 * Reads data from the stream into the buffer
		 *
		 * This function may not return the requested size. It will only
		 * return as many bytes as they are available in the input buffer.
		 *
		 * @param p_buffer the buffer to store the data
		 * @param size the maximum number of bytes to read
		 *
		 * @return the number of bytes placed in the buffer
		 */
		virtual UInt32 readBytes (char* p_buffer, UInt32 size) = 0;

		/**
		 * Writes one byte of data to the stream
		 *
		 * This function will block until the byte has been written
		 *
		 * @param b the byte to write
		 *
		 * @return the number of bytes written
		 */
		virtual UInt32 write (UInt8 b) = 0;

		/**
		 * Writes data to the stream
		 *
		 * This function will block until all data have been written.
		 *
		 * @param p_buffer the buffer containing the data to write
		 * @param size the size of the data in bytes
		 *
		 * @return the number of bytes written
		 */
		virtual UInt32 write (const UInt8* p_buffer, UInt32 size) = 0;

		///@}

	private:

		StreamIO (const StreamIO&);
		void operator= (const StreamIO&);

	};


#endif
} }

#endif // ROBOCOM_SHARED_STREAM_IO_HPP
