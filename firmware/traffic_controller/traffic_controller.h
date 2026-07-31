#ifndef TRAFFIC_CONTROLLER_H
#define TRAFFIC_CONTROLLER_H

#include "lane.h"

enum SignalState
{
    GREEN_STATE,
    YELLOW_STATE,
    ALL_RED_STATE,
    PRE_GREEN_YELLOW_STATE
};

class TrafficController
{
private:

    Lane lanes[4];

    int currentLane;

    SignalState currentState;

    unsigned long previousMillis;
    unsigned long stateDuration;

    void changeState();

    void updateWaitingTimes();

    void simulateTraffic();

    void scheduleNextLane();

    int getHighestPriorityLane();

    int getEmergencyLane();

    void allRed();

    void nextLane();      // keep for testing/debugging

public:

    TrafficController();

    void begin();

    void update();

    void printStatus();
};

#endif