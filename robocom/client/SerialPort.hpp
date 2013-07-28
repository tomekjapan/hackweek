#ifndef ROBOCOM_CLIENT_SERIAL_PORT_HPP
#define ROBOCOM_CLIENT_SERIAL_PORT_HPP

#include "client_base.hpp"

// System headers
#include <string>
#include <system_error>

// External component headers
#include "robocom/shared/StreamIO.hpp"

// Component headers
#include "Handle.hpp"


namespace robocom {
namespace client
{

	/**
	 * Enumeration describing the speed of a serial port connection
	 */
	enum BaudRate
	{
		BAUD_RATE_50,
		BAUD_RATE_75,
		BAUD_RATE_110,
		BAUD_RATE_134,
		BAUD_RATE_150,
		BAUD_RATE_200,
		BAUD_RATE_300,
		BAUD_RATE_600,
		BAUD_RATE_1200,
		BAUD_RATE_1800,
		BAUD_RATE_2400,
		BAUD_RATE_4800,
		BAUD_RATE_9600,
		BAUD_RATE_19200,
		BAUD_RATE_38400,
		BAUD_RATE_57600,
		BAUD_RATE_115200
	};

	/**
	 * This class implements a line of communication between
	 * the client and the robot over a serial port.
	 */
	class SerialPort
		: public shared::StreamIO
	{
	public:

		SerialPort () throw ();

		SerialPort (
			const std::string& port_name,
			BaudRate baud_rate = BAUD_RATE_9600
		) throw (
			std::system_error
		);

		virtual ~SerialPort () throw ();

		/**
		 * Returns whether this serial port is open
		 */
		bool isOpen () const throw ();

		/**
		 * Gets the baud rate for this serial port
		 *
		 * @return the baud rate
		 *
		 * @pre isOpen()
		 */
		BaudRate getBaudRate () const throw (std::system_error);

		/**
		 * Sets the baud rate for this serial port
		 *
		 * @param baud_rate the new baud rate
		 *
		 * @pre isOpen()
		 */
		void setBaudRate (BaudRate baud_rate) throw (std::system_error);

		/**
		 * Closes this serial port.
		 */
		void close () throw ();

		/**
		 * Returns the number of bytes immediately available for reading
		 *
		 * @return the number available bytes
		 */
		virtual int available () throw (std::system_error);

		/**
		 * Blocks until some data are available for reading
		 */
		void awaitAvailable () throw (std::system_error);

		/**
		 * Returns the next byte available for reading without removing
		 * it from the stream
		 *
		 * @return the byte value or -1 if no bytes are available
		 */
		virtual int peek () throw (std::system_error);

		/**
		 * Reads the next available byte
		 *
		 * @return the byte value or -1 if no bytes are available
		 */
		virtual int read () throw (std::system_error);

		/**
		 * Reads at most the given number of bytes into the given buffer
		 * without blocking
		 *
		 * @param p_buffer the place to store the read bytes
		 * @param size the number of bytes to read
		 *
		 * @return the actual number of bytes read
		 */
		UInt32 read (
			UInt8* p_buffer,
			UInt32 size
		) throw (
			std::system_error
		);
		
		/**
		 * Reads at most the given number of bytes into the given buffer
		 * without blocking
		 *
		 * @param p_buffer the place to store the read bytes
		 * @param size the number of bytes to read
		 *
		 * @return the actual number of bytes read
		 */
		virtual UInt32 readBytes (
			char* p_buffer,
			UInt32 size
		) throw (
			std::system_error
		);

		/**
		 * Writes one byte of data to the stream
		 *
		 * This function will block until the byte has been written
		 *
		 * @param b the byte to write
		 *
		 * @return the number of bytes written
		 */
		virtual UInt32 write (UInt8 b) throw (std::system_error);

		/**
		 * Write at most the given number of bytes from the given buffer
		 * blocking until all data have been written
		 *
		 * @param p_buffer the bytes two write
		 * @param size the number of bytes to write
		 *
		 * @return the number of bytes written
		 */
		virtual UInt32 write (
			const UInt8* p_buffer,
			UInt32 size
		) throw (
			std::system_error
		);

		/**
		 * Reads one newline-terminated string from the serial port
		 *
		 * The newline character is included in the returned string
		 *
		 * @return a string containing the read line
		 */
		std::string readln () throw (std::system_error);

		/**
		 * Prints the given string to the serial port.
		 *
		 * @param text the string to write
		 */
		void print (const std::string& text) throw (std::system_error);

		/**
		 * Dumps attributes of this object to the standard output
		 */
		void dumpAttributes () const throw (std::system_error);

	private:
		SerialPort (const SerialPort& other);
		SerialPort& operator= (const SerialPort& other);

		void _configure (BaudRate baud_rate) const throw (std::system_error);

		void _setSpeed (
			void* p_mem,
			BaudRate baud_rate
		) const throw (
			std::system_error
		);

		void _initAttributes(void* p_mem) const throw (std::system_error);
		void _getAttributes(void* p_mem) const throw (std::system_error);
		void _setAttributes(const void* p_mem) const throw (std::system_error);
		void _flush () const throw (std::system_error);

		static SysHandleType _openPort (
			const std::string& port_name
		) throw (
			std::system_error
		);

		Handle m_handle;
		std::string m_port_name;
		int m_peeked_byte;
		UInt8* const m_p_old_config;
	};

} }

#endif // ROBOCOM_CLIENT_SERIAL_PORT_HPP
