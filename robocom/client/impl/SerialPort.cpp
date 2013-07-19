// System headers
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <sstream>
#include <termios.h>
#include <unistd.h>

// External component headers
#include "common/ErrorReporting.hpp"

// Component headers
#include "../Handle.hpp"

// Module header
#include "../SerialPort.hpp"

namespace robocom {
namespace client
{
	using namespace std;

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
			return B115200;
		default:
			NCR_UNEXPECTED(
				"invalid baud rate : " + to_string( baud_rate )
			);
			return B115200;
		}
	}


	static BaudRate __fromNative (::speed_t native_baud_rate) throw ()
	{
		switch ( native_baud_rate )
		{
		case B50:
			return BAUD_RATE_50;
		case B75:
			return BAUD_RATE_75;
		case B110:
			return BAUD_RATE_110;
		case B134:
			return BAUD_RATE_134;
		case B150:
			return BAUD_RATE_150;
		case B200:
			return BAUD_RATE_200;
		case B300:
			return BAUD_RATE_300;
		case B600:
			return BAUD_RATE_600;
		case B1200:
			return BAUD_RATE_1200;
		case B1800:
			return BAUD_RATE_1800;
		case B2400:
			return BAUD_RATE_2400;
		case B4800:
			return BAUD_RATE_4800;
		case B9600:
			return BAUD_RATE_9600;
		case B19200:
			return BAUD_RATE_19200;
		case B38400:
			return BAUD_RATE_38400;
		case B57600:
			return BAUD_RATE_57600;
		case B115200:
			return BAUD_RATE_115200;
		default:
			NCR_UNEXPECTED(
				"invalid native baud rate : " + to_string( native_baud_rate )
			);
			return BAUD_RATE_115200;
		}
	}


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
		close();
	}

		
	bool
	SerialPort::isOpen () const throw ()
	{
		return m_handle.isOpen();
	}


	BaudRate
	SerialPort::getBaudRate () const throw (system_error)
	{
		USE_CONTRACT_CHECK( isOpen() );

		::termios tio;
		_getAttributes( & tio );
	
		return __fromNative( ::cfgetispeed( & tio ) );
	}


	void
	SerialPort::setBaudRate (BaudRate baud_rate) throw (system_error)
	{
		USE_CONTRACT_CHECK( isOpen() );

		::termios tio;
		_getAttributes( & tio );
		_setSpeed( & tio, baud_rate );
		_flush();
		_setAttributes( & tio );
	}


	void
	SerialPort::close () throw ()
	{
		m_handle.close();
	}


	void
	SerialPort::print (const string& text) const throw (system_error)
	{
		const SysHandleType fd = m_handle.getNative();

		::fd_set wrset;
		FD_ZERO( & wrset );
		FD_SET( m_handle.getNative(), & wrset );

		const char* p_data = text.c_str();
		size_t count = text.size();

		while ( count > 0 )
		{
			::timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 200000;

			if ( ::select( fd + 1, NULL, & wrset, NULL, & tv ) < 0 )
			{
				THROW_SYSTEM_ERROR(
					"Error blocking for output on " + m_port_name
				);
			}

			if ( ! FD_ISSET( fd, & wrset ) ) {
				continue;
			}

			const ::ssize_t num =
				::write( m_handle.getNative(), p_data, count );

			if ( num >= 0 )
			{
				count -= num;
				p_data += num;
			}
			else if ( errno != EINTR ) {
				THROW_SYSTEM_ERROR( "error writing to " + m_port_name );
			}
		}
	}


	string
	SerialPort::readln () const throw (system_error)
	{
		const SysHandleType fd = m_handle.getNative();

		::fd_set rdset;
		FD_ZERO( & rdset );
		FD_SET( m_handle.getNative(), & rdset );

		ostringstream sb;

		char c = 0;
		while ( c != '!' )
		{
			::timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 200000;

			if ( ::select( fd + 1, & rdset, NULL, NULL, & tv ) < 0 )
			{
				THROW_SYSTEM_ERROR(
					"Error blocking for input on " + m_port_name
				);
			}

			if ( ! FD_ISSET( fd, & rdset ) ) {
				continue;
			}

			const ::ssize_t num = ::read( fd, & c, 1 );
			if ( num > 0 ) {
				sb.put( c );
			}
			else if ( num < 0 ) {
				THROW_SYSTEM_ERROR( "error reading from " + m_port_name );
			}
		}

		return sb.str();
	}


	void
	SerialPort::dumpAttributes () const throw (system_error)
	{
		::termios tio;
		_getAttributes( & tio );

		fprintf( stdout, "iflag=%x\n", tio.c_iflag );
		fprintf( stdout, "oflag=%x\n", tio.c_oflag );
		fprintf( stdout, "cflag=%x\n", tio.c_cflag );
		fprintf( stdout, "lflag=%x\n", tio.c_lflag );
		fprintf( stdout, "vmin=%d\n", tio.c_cc[VMIN] );
		fprintf( stdout, "vtime=%d\n", tio.c_cc[VTIME] );
	}


	void
	SerialPort::_configure (BaudRate baud_rate) const throw (system_error)
	{
		::termios tio;
		_getAttributes( & tio );

		tio.c_iflag &= ~IGNBRK;
		tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
		tio.c_oflag &= ~OPOST;

		tio.c_cc[VMIN] = 0;
		tio.c_cc[VTIME] = 0;

		tio.c_iflag &= ~(IXON | IXOFF | IXANY);
		tio.c_cflag |= (CLOCAL | CREAD | CS8);

		tio.c_cflag &= ~PARENB;
		tio.c_cflag &= ~PARODD;
		tio.c_cflag &= ~CSTOPB;
		tio.c_cflag &= ~CRTSCTS;

		_setSpeed( & tio, baud_rate );
		_flush();
		_setAttributes( & tio );
	}


	void
	SerialPort::_initAttributes (void* p_mem) const throw (system_error)
	{
		::termios* p_tio = static_cast< ::termios*>( p_mem );
		::bzero( p_tio, sizeof( * p_tio ) );

		p_tio->c_cflag = CS8 | CLOCAL | CREAD;
		p_tio->c_iflag = IGNPAR;
		p_tio->c_oflag = 0;
		p_tio->c_lflag = 0;
		p_tio->c_cc[VTIME] = 0;
		p_tio->c_cc[VMIN] = 1;
	}


	void
	SerialPort::_getAttributes (void* p_mem) const throw (system_error)
	{
		::termios* p_tio = static_cast< ::termios*>( p_mem );
		::bzero( p_tio, sizeof( * p_tio ) );

		if ( ::tcgetattr( m_handle.getNative(), p_tio ) < 0 ) {
			THROW_SYSTEM_ERROR( "failed to get attributes for " + m_port_name );
		}
	}


	void
	SerialPort::_setAttributes (const void* p_mem) const throw (system_error)
	{
		const ::termios* p_tio = static_cast<const ::termios*>( p_mem );

		if ( ::tcsetattr( m_handle.getNative(), TCSANOW, p_tio ) < 0 ) {
			THROW_SYSTEM_ERROR( "failed to set attributes for " + m_port_name );
		}
	}


	void
	SerialPort::_flush () const throw (system_error)
	{
		if ( ::tcflush( m_handle.getNative(), TCIFLUSH ) < 0 ) {
			THROW_SYSTEM_ERROR( "failed to flush " + m_port_name );
		}
	}


	void
	SerialPort::_setSpeed (
		void* p_mem,
		BaudRate baud_rate
	) const throw (
		system_error
	)
	{
		::termios* p_tio = static_cast< ::termios*>( p_mem );
		const ::speed_t native_baud_rate = __toNative( baud_rate );

		if ( ::cfsetispeed( p_tio, native_baud_rate ) < 0 )
		{
			THROW_SYSTEM_ERROR(
				"failed to set input speed on " + m_port_name +
				" to " + to_string( baud_rate )
			);
		}

		if ( ::cfsetospeed( p_tio, native_baud_rate ) < 0 )
		{
			THROW_SYSTEM_ERROR(
				"failed to set input speed on " + m_port_name +
				" to " + to_string( baud_rate )
			);
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
