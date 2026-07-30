#include <Arduino.h>
#include "traffic_controller.h"
#include "config.h"

TrafficController::TrafficController()
{
    lanes[0] = Lane(NORTH_RED, NORTH_YELLOW, NORTH_GREEN);
    lanes[1] = Lane(EAST_RED, EAST_YELLOW, EAST_GREEN);
    lanes[2] = Lane(SOUTH_RED, SOUTH_YELLOW, SOUTH_GREEN);
    lanes[3] = Lane(WEST_RED, WEST_YELLOW, WEST_GREEN);

    currentLane = 0;
}

void TrafficController::begin()
{
    for (int i = 0; i < 4; i++)
    {
        lanes[i].begin();
    }

    allRed();
}

void TrafficController::allRed()
{
    for (int i = 0; i < 4; i++)
    {
        lanes[i].red();
    }
}

void TrafficController::nextLane()
{
    currentLane++;

    if (currentLane >= 4)
        currentLane = 0;
}

void TrafficController::update()
{
    // Version 1
    // Scheduler logic will be added later.
}