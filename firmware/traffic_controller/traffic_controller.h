#ifndef TRAFFIC_CONTROLLER_H
#define TRAFFIC_CONTROLLER_H

#include "lane.h"

enum SignalState
{
    GREEN_STATE,
    YELLOW_STATE,
    ALL_RED_STATE
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

public:
    TrafficController();

    void begin();
    void update();

    void nextLane();
    void allRed();

    int getHighestPriorityLane();

    void printStatus();
};

#endif