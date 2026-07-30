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

    // Simulated vehicle counts
    lanes[0].setVehicleCount(10);
    lanes[1].setVehicleCount(2);
    lanes[2].setVehicleCount(6);
    lanes[3].setVehicleCount(1);

    printStatus();

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
        currentLane = 0;
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

void TrafficController::printStatus()
{
    Serial.println("================================");

    const char *laneNames[4] =
    {
        "North",
        "East",
        "South",
        "West"
    };

    for (int i = 0; i < 4; i++)
    {
        Serial.print(laneNames[i]);
        Serial.print(" Vehicles: ");
        Serial.print(lanes[i].getVehicleCount());

        Serial.print(" Waiting: ");
        Serial.print(lanes[i].getWaitingTime());

        Serial.print(" Priority: ");
        Serial.println(lanes[i].getPriorityScore());
    }

    Serial.println("================================");
}

int TrafficController::getHighestPriorityLane()
{
    int bestLane = 0;
    float highestPriority = lanes[0].getPriorityScore();

    for (int i = 1; i < 4; i++)
    {
        float priority = lanes[i].getPriorityScore();

        if (priority > highestPriority)
        {
            highestPriority = priority;
            bestLane = i;
        }
    }

    return bestLane;
}