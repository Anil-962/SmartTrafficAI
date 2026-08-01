#include <Arduino.h>
#include "traffic_controller.h"
#include "config.h"
#include <WiFi.h>

TrafficController::TrafficController()
{
    lanes[0] = Lane(NORTH_RED, NORTH_YELLOW, NORTH_GREEN);
    lanes[1] = Lane(EAST_RED, EAST_YELLOW, EAST_GREEN);
    lanes[2] = Lane(SOUTH_RED, SOUTH_YELLOW, SOUTH_GREEN);
    lanes[3] = Lane(WEST_RED, WEST_YELLOW, WEST_GREEN);

    currentLane = 0;
    currentState = PRE_GREEN_YELLOW_STATE;

    previousMillis = 0;
    stateDuration = 2000;
}
void TrafficController::begin()
{
    randomSeed(millis());

    for (int i = 0; i < 4; i++)
    {
        lanes[i].begin();
        lanes[i].red();
    }

    // Demo traffic values
    lanes[0].setVehicleCount(10);
    lanes[1].setVehicleCount(2);
    lanes[2].setVehicleCount(6);
    lanes[3].setVehicleCount(1);

    currentLane = getHighestPriorityLane();

    lanes[currentLane].yellow();

    currentState = PRE_GREEN_YELLOW_STATE;

    stateDuration = 2000;

    previousMillis = millis();

    printStatus();
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
        case PRE_GREEN_YELLOW_STATE:

            lanes[currentLane].green();

            currentState = GREEN_STATE;

            stateDuration = MIN_GREEN_TIME * 1000UL;

            break;

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

            scheduleNextLane();

            lanes[currentLane].yellow();

            currentState = PRE_GREEN_YELLOW_STATE;

            stateDuration = 2000;

            break;
    }

    previousMillis = millis();
}

void TrafficController::update()
{
    if (millis() - previousMillis >= stateDuration)
    {
        changeState();

        Serial.print("Current Lane : ");
        Serial.print(getCurrentLaneName());

        Serial.print(" | State : ");
        Serial.println(getCurrentStateName());
    }
}

void TrafficController::printStatus()
{
    const char *laneNames[4] =
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
        int newVehicles = random(0, 4);

        lanes[i].addVehicles(newVehicles);
    }
}

int TrafficController::getEmergencyLane()
{
    for (int i = 0; i < 4; i++)
    {
        if (lanes[i].isEmergency())
        {
            return i;
        }
    }

    return -1;
}
void TrafficController::scheduleNextLane()
{
    updateWaitingTimes();

    lanes[currentLane].removeVehicles(3);

    simulateTraffic();

    int emergencyLane = getEmergencyLane();

    if (emergencyLane != -1)
    {
        currentLane = emergencyLane;

        Serial.println("Emergency Vehicle Detected");
    }
    else
    {
        currentLane = getHighestPriorityLane();
    }

    printStatus();
}
String TrafficController::getCurrentLaneName()
{
    switch (currentLane)
    {
        case 0:
            return "North";

        case 1:
            return "East";

        case 2:
            return "South";

        case 3:
            return "West";

        default:
            return "Unknown";
    }
}
String TrafficController::getCurrentStateName()
{
    switch (currentState)
    {
        case GREEN_STATE:
            return "GREEN";

        case YELLOW_STATE:
            return "YELLOW";

        case ALL_RED_STATE:
            return "ALL_RED";

        case PRE_GREEN_YELLOW_STATE:
            return "PRE_GREEN_YELLOW";

        default:
            return "UNKNOWN";
    }
}
TrafficStatus TrafficController::getStatus()
{
    TrafficStatus status;

    status.project = "SmartTrafficAI";
    status.version = "2.3";

    status.currentLane = getCurrentLaneName();
    status.signalState = getCurrentStateName();

    status.wifi = "Connected";
    status.rssi = WiFi.RSSI();
    status.uptime = millis() / 1000;

    const char* names[4] =
    {
        "North",
        "East",
        "South",
        "West"
    };

    for(int i=0;i<4;i++)
    {
        status.lanes[i].name = names[i];
        status.lanes[i].vehicles = lanes[i].getVehicleCount();
        status.lanes[i].waiting = lanes[i].getWaitingTime();
        status.lanes[i].priority = lanes[i].getPriorityScore();
    }

    return status;
}