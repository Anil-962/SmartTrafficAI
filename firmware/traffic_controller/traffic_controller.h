#ifndef TRAFFIC_CONTROLLER_H
#define TRAFFIC_CONTROLLER_H

#include <Arduino.h>
#include "Lane.h"
enum TrafficState
{
    PRE_GREEN_YELLOW_STATE,
    GREEN_STATE,
    YELLOW_STATE,
    ALL_RED_STATE
};

struct LaneStatus
{
    String name;
    int vehicles;
    int waiting;
    float priority;
    bool emergency;
};
struct TrafficStatus
{
    String project;
    String version;

    String currentLane;
    String signalState;

    String wifi;

    int rssi;
    unsigned long uptime;

    LaneStatus lanes[4];
};

class TrafficController
{
private:

    Lane lanes[4];

    int currentLane;
    int currentState;

    unsigned long previousMillis;
    unsigned long stateDuration;

    // Emergency status for each lane
    bool emergency[4];

    void changeState();

    void allRed();

    void nextLane();

    void scheduleNextLane();

    void updateWaitingTimes();

    void simulateTraffic();

    void printStatus();

    int getHighestPriorityLane();

    int getEmergencyLane();

    String getCurrentLaneName();

    String getCurrentStateName();

public:
    TrafficController();
    void begin();

    void update();

    TrafficStatus getStatus();

    Lane* getLanes();

    int getCurrentLane();

    String getSignalState();
    void updateSensorData(
        float north,
        float east,
        float south,
        float west
    );
    void setEmergencyLane(int lane);

    void clearEmergencyLane(int lane);

    void clearAllEmergency();

    bool isEmergencyLane(int lane);
};

#endif