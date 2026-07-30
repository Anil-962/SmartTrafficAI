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
    lanes[0].setVehicleCount(3);
    lanes[1].setVehicleCount(15);
    lanes[2].setVehicleCount(6);
    lanes[3].setVehicleCount(1);

    printStatus();
    int bestLane = getHighestPriorityLane();

    Serial.print("Best Lane = ");
    Serial.println(bestLane);

    currentLane = getHighestPriorityLane();
    lanes[currentLane].green();

    previousMillis = millis();
    Serial.println("Reached here");
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

        updateWaitingTimes();
        lanes[currentLane].removeVehicles(5);
        simulateTraffic();
        printStatus();

        currentLane = getHighestPriorityLane();
        Serial.print("Next Green Lane = ");
        Serial.println(currentLane);

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
    Serial.println("========== START ==========");

    const char* laneNames[4] =
    {
        "North",
        "East",
        "South",
        "West"
    };

    for (int i = 0; i < 4; i++)
    {
        Serial.print("Lane Index = ");
        Serial.println(i);

        Serial.print("Name = ");
        Serial.println(laneNames[i]);

        Serial.print("Vehicles = ");
        Serial.println(lanes[i].getVehicleCount());

        Serial.print("Waiting = ");
        Serial.println(lanes[i].getWaitingTime());

        Serial.print("Priority = ");
        Serial.println(lanes[i].getPriorityScore());

        Serial.println("----------------");
    }

    Serial.println("=========== END ===========");
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
void TrafficController::updateWaitingTimes()
{
    for (int i = 0; i < 4; i++)
    {
        if (i == currentLane)
        {
            lanes[i].resetWaitingTime();
        }
        else
        {
            lanes[i].incrementWaitingTime();
        }
    }
}
void TrafficController::simulateTraffic()
{
    for (int i = 0; i < 4; i++)
    {
        int newVehicles = random(0, 4);   // 0–3 new vehicles
        lanes[i].addVehicles(newVehicles);
    }
}