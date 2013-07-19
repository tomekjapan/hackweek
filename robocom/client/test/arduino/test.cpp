#include <Arduino.h>

void setup ()
{
	Serial.begin(115200);
}

void echoOneLine () throw ()
{
	if ( ! Serial.available() ) {
		return;
	}

	char buffer[100];

	byte num = Serial.readBytesUntil( '!', buffer, sizeof(buffer) );

	if ( num >= 0 )
	{
		// Make sure we have a zero-terminated 
		if ( num >= sizeof(buffer) ) {
			num = sizeof(buffer) - 1;
		}
		buffer[num] = '\0';

		// Write the line back
		Serial.write( buffer );
		Serial.print( "!" );
		Serial.flush();
	}
}


void loop ()
{
 	echoOneLine();
}
