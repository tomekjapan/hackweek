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

	private:
		SerialPort (const SerialPort& other);
		SerialPort& operator= (const SerialPort& other);

		void _configure (BaudRate baud_rate) const throw (std::system_error);

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
