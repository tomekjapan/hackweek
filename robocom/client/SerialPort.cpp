// System headers
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <termios.h>

// External component headers
#include "common/ErrorReporting.hpp"

// Component headers
#include "Handle.hpp"

// Module header
#include "SerialPort.hpp"

namespace robocom {
namespace client
{
	using namespace std;

	SerialPort::SerialPort () throw ()
		: m_handle( )
		, m_port_name( )
	{ }


	SerialPort::SerialPort (
		const std::string& port_name,
		BaudRate baud_rate
	) throw (
		system_error
	)
		: m_handle( _openPort( port_name ) )
		, m_port_name( port_name )
	{
		_configure( baud_rate );
	}


	SerialPort::~SerialPort () throw ()
	{
	}


	static ::speed_t __toNative (BaudRate baud_rate) throw ()
	{
		switch ( baud_rate )
		{
		case BAUD_RATE_50:
			return B50;
		case BAUD_RATE_75:
			return B75;
		case BAUD_RATE_110:
			return B110;
		case BAUD_RATE_134:
			return B134;
		case BAUD_RATE_150:
			return B150;
		case BAUD_RATE_200:
			return B200;
		case BAUD_RATE_300:
			return B300;
		case BAUD_RATE_600:
			return B600;
		case BAUD_RATE_1200:
			return B1200;
		case BAUD_RATE_1800:
			return B1800;
		case BAUD_RATE_2400:
			return B2400;
		case BAUD_RATE_4800:
			return B4800;
		case BAUD_RATE_9600:
			return B9600;
		case BAUD_RATE_19200:
			return B19200;
		case BAUD_RATE_38400:
			return B38400;
		case BAUD_RATE_57600:
			return B57600;
		case BAUD_RATE_115200:
		default:
			return B115200;
		}
	}


	void
	SerialPort::_configure (BaudRate baud_rate) const throw (system_error)
	{
		::termios tio;
		::bzero( &tio, sizeof(tio) );

		tio.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
		tio.c_iflag = IGNPAR;
		tio.c_oflag = 0;
		tio.c_lflag = 0;
		tio.c_cc[VTIME] = 0;
		tio.c_cc[VMIN] = 1;

		if ( ::cfsetispeed( & tio, __toNative( baud_rate ) ) < 0 )
		{
			THROW_SYSTEM_ERROR(
				"failed to set input speed on " + m_port_name +
				" to " + to_string( baud_rate )
			);
		}

		if ( ::cfsetospeed( & tio, __toNative( baud_rate ) ) < 0 )
		{
			THROW_SYSTEM_ERROR(
				"failed to set input speed on " + m_port_name +
				" to " + to_string( baud_rate )
			);
		}

		if ( ::tcflush( m_handle.getNative(), TCIFLUSH ) < 0 ) {
			THROW_SYSTEM_ERROR( "failed to flush " + m_port_name );
		}

		if ( ::tcsetattr( m_handle.getNative(), TCSANOW, & tio ) < 0 ) {
			THROW_SYSTEM_ERROR( "failed to set attributes for " + m_port_name );
		}
	}


	SysHandleType
	SerialPort::_openPort (
		const string& port_name
	) throw (
		system_error
	)
	{
		const int fd = ::open( port_name.c_str(), O_RDWR|O_NOCTTY );

		if ( fd < 0 ) {
			THROW_SYSTEM_ERROR( "failed to open " + port_name );
		}

		return fd;
	}

} }
