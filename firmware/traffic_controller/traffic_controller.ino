#include <Arduino.h>
#include "config.h"
#include "traffic_light.h"

TrafficLight north(NORTH_RED, NORTH_YELLOW, NORTH_GREEN);

void setup()
{
    north.begin();
}

void loop()
{
    north.red();
    delay(3000);

    north.yellow();
    delay(1000);

    north.green();
    delay(3000);
}