#include <string>
#include <unittest++/UnitTest++.h>
#include <unistd.h>

#include "robocom/shared/msg/FlushResponse.hpp"
#include "robocom/shared/MessageIO.hpp"
#include "robocom/shared/msg/SimpleMessage.hxx"
#include "robocom/client/SerialPort.hpp"

using namespace std;
using namespace robocom::shared;
using namespace robocom::shared::msg;
using namespace robocom::client;

SUITE(SerialPortTester)
{
	SerialPort sp( "/dev/ttyACM0", BAUD_RATE_115200 );

	struct Sleeper
	{
		Sleeper () throw ()
		{
			printf ("Waiting 2 secs for Arduino to reset...");
			fflush( stdout );
			sleep(2);
			printf( "OK\n" );
		}
	}
	wait_for_arduino_reset;


	TEST(GetSetBaudRate)
	{
		CHECK( sp.isOpen() );

		BaudRate br = sp.getBaudRate();
		sp.setBaudRate( BAUD_RATE_9600 );
		CHECK_EQUAL( sp.getBaudRate(), BAUD_RATE_9600 );
		sp.setBaudRate( br );
		CHECK_EQUAL( sp.getBaudRate(), br );
	}

	TEST(Noop)
	{
		MessageIO io( sp );

		// Noop requests should just get ignored
		for ( int i = 0; i < 100; i++ ) {
			io.write( NoopRequest(i).asMessage() );
		}
	}

	TEST(Reset)
	{
		MessageIO io( sp );

		// Reset request does not result in any immediate responses
		for ( int i = 0; i < 10; i++ ) {
			io.write( ResetRequest(i).asMessage() );
		}
	}

	TEST(Echo)
	{
		MessageIO io( sp );
		Message msg;

		for ( int i = 0; i < 10; i++ )
		{
			// Write a request
			io.write( EchoRequest(i).asMessage() );

			// Read a response
			sp.awaitAvailable();
			while( ! io.read( msg ) );

			// Check the response
			EchoResponse response( msg );
			CHECK_EQUAL( STATUS_OK, response.validate() );
			CHECK_EQUAL( i, response.getTaskId() );
		}
	}

	TEST(Flush)
	{
		MessageIO io( sp );
		Message msg;

		for ( int i = 100; i < 110; i++ )
		{
			// Write a request
			io.write( FlushRequest(i).asMessage() );

			// Read responses until we get one with the task ID
			// equal to the request ID

			do
			{
				sp.awaitAvailable();
				while( ! io.read( msg ) );
			}
			while ( msg.getTaskId() != i );

			// Check the response
			FlushResponse response( msg );
			CHECK_EQUAL( STATUS_OK, response.validate() );
		}
	}
}
