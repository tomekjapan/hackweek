#include <Arduino.h>

#include "robocom/shared/Server.hpp"

robocom::shared::Server server( Serial );

void setup ()
{
	Serial.begin(115200);
	server.setup();
}

void loop ()
{
	server.loop();
}
