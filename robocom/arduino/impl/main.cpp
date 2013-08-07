#include "../RobotServer.hpp"

RobotServer server( Serial );

void setup ()
{
	Serial.begin(57600);
	server.setup();
}

void loop ()
{
	server.loop();
}
