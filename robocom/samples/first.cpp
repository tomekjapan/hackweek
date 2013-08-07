#include <unistd.h>
#include <stdio.h>

#include "robocom/client/SerialPort.hpp"
#include "robocom/shared/MessageIO.hpp"
#include "robocom/shared/msg/SetWheelDriveRequest.hpp"
#include "robocom/shared/msg/EncoderReadingNotice.hpp"
#include "robocom/shared/msg/EncoderReadingRequest.hpp"
#include "robocom/shared/msg/FlushResponse.hpp"
#include "robocom/shared/msg/SimpleMessage.hxx"

using namespace robocom::client;
using namespace robocom::shared;
using namespace robocom::shared::msg;

SerialPort sp( "/dev/ttyUSB0", BAUD_RATE_57600 );
MessageIO io( sp );
int task_id = 1;

void reset ()
{
	printf( "%d: Reset\n", task_id );
	ResetRequest req( task_id++ );
	io.write( req.asMessage() );
}


void setDrive (UInt8 dir1, UInt8 sig1, UInt8 dir2, UInt8 sig2)
{
	printf( "%d: SetWheelDrive\n", task_id );
	SetWheelDriveRequest req( task_id++, dir1, sig1, dir2, sig2 );
	io.write( req.asMessage() );
}


void subscribe (UInt8 encoder)
{
	printf( "%d: Subscribe %d\n", task_id, encoder );
	EncoderReadingRequest req( task_id++, encoder, true );
	io.write( req.asMessage() );
}


void unsubcribe (UInt8 encoder)
{
	printf( "%d: Unsubscribe %d\n", task_id, encoder );
	EncoderReadingRequest req( task_id++, encoder, false );
	io.write( req.asMessage() );
}


void flush ()
{
	printf( "%d: Flush\n", task_id );
	FlushRequest req( task_id++ );
	io.write( req.asMessage() );

	Message msg;
	do {
		if ( io.read( msg ) )
		{
			if ( msg.getMessageType() == EncoderReadingNotice::MSGID )
			{
				EncoderReadingNotice nt( msg );
				printf( "Got reading for encoder %d\n", nt.getEncoderId() );
			}
			else
			{
				printf( "Got response, type=%d, task ID=%d\n",
						msg.getMessageType(),
						msg.getTaskId() );
			}
		}
	} while ( msg.getTaskId() != req.getTaskId() );
}


int main ()
{
	// Throw out any remaining messages (if any) on arduino
	reset();

	// Subscribe for encoder readings
	subscribe( 0 );
	subscribe( 1 );

	// Turn on drive of both wheels with signal=100
	setDrive( 0, 100, 0, 100 );
	flush();

	// Now let the wheels spin for a few seconds. Keep flushing or
	// the output queue will get clogged with messages.
	for ( int i = 0; i < 30; i++ ) {
		usleep(100000); // sleep for 100ms
		flush();
	}

	// Again, stop the wheels and reset the state
	reset();

	// Read back queued messages (one wheel drive change notice from the
	// reset and a flush response).
	flush();
}
