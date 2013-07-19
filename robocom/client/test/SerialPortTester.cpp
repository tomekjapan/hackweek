#include <string>
#include <unittest++/UnitTest++.h>
#include <unistd.h>

#include "robocom/client/SerialPort.hpp"

using namespace std;
using namespace robocom::client;

SUITE(SerialPortTester)
{
	TEST(TestOpenClose)
	{
		SerialPort sp( "/dev/ttyACM0", BAUD_RATE_115200 );
		CHECK( sp.isOpen() );
		sp.close();
		CHECK( ! sp.isOpen() );
		sp.close();
	}

	TEST(TestGetSetBaudRate)
	{
		SerialPort sp( "/dev/ttyACM0", BAUD_RATE_115200 );
		CHECK_EQUAL( sp.getBaudRate(), BAUD_RATE_115200 );
		sp.setBaudRate( BAUD_RATE_9600 );
		CHECK_EQUAL( sp.getBaudRate(), BAUD_RATE_9600 );
	}

	TEST(TestEcho)
	{
		SerialPort sp( "/dev/ttyACM0", BAUD_RATE_115200 );

		// Now, arduino needs some time to reset before it can
		// accept messages
		sleep(2);

		const string msg = "Hello World!";
		sp.print( msg );
		const string reply = sp.readln();
		CHECK_EQUAL( msg, reply );
	}
}
