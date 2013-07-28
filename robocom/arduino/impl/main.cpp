#include "../RobotServer.hpp"

RobotServer server( Serial );

void setup ()
{
	Serial.begin( 115200 );
	server.setup();
}

void loop ()
{
	server.loop();
}
