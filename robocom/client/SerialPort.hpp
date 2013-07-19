#ifndef ROBOCOM_CLIENT_SERIAL_PORT_HPP
#define ROBOCOM_CLIENT_SERIAL_PORT_HPP

// System headers
#include <string>
#include <system_error>

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
	{
	public:

		SerialPort () throw ();

		SerialPort (
			const std::string& port_name,
			BaudRate baud_rate = BAUD_RATE_9600
		) throw (
			std::system_error
		);

		~SerialPort () throw ();

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
		 * Prints the given string to the serial port.
		 *
		 * @param text the string to write
		 */
		void print (const std::string& text) const throw (std::system_error);

		/**
		 * Reads one newline-terminated string from the serial port
		 *
		 * The newline character is included in the returned string
		 *
		 * @return a string containing the read line
		 */
		std::string readln () const throw (std::system_error);

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
	};

} }

#endif // ROBOCOM_CLIENT_SERIAL_PORT_HPP
