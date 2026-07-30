#include "config.h"
#include "lane.h"

Lane north(
    NORTH_RED,
    NORTH_YELLOW,
    NORTH_GREEN
);

void setup()
{
    north.begin();
}

void loop()
{
    north.getTrafficLight().red();
    delay(3000);

    north.getTrafficLight().yellow();
    delay(1000);

    north.getTrafficLight().green();
    delay(3000);
}