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

    currentState = GREEN_STATE;

    previousMillis = 0;

    stateDuration = MIN_GREEN_TIME * 1000UL;
}

void TrafficController::begin()
{
    for (int i = 0; i < 4; i++)
    {
        lanes[i].begin();
        lanes[i].red();
    }

    lanes[currentLane].green();

    previousMillis = millis();
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
    {
        currentLane = 0;
    }
}

void TrafficController::changeState()
{
    switch (currentState)
    {
    case GREEN_STATE:

        lanes[currentLane].yellow();

        currentState = YELLOW_STATE;

        stateDuration = YELLOW_TIME * 1000UL;

        break;

    case YELLOW_STATE:

        allRed();

        currentState = ALL_RED_STATE;

        stateDuration = 1000;

        break;

    case ALL_RED_STATE:

        nextLane();

        lanes[currentLane].green();

        currentState = GREEN_STATE;

        stateDuration = MIN_GREEN_TIME * 1000UL;

        break;
    }

    previousMillis = millis();
}

void TrafficController::update()
{
    if (millis() - previousMillis >= stateDuration)
    {
        changeState();
    }
}