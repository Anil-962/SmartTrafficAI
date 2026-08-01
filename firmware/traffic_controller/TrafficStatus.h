#ifndef TRAFFIC_STATUS_H
#define TRAFFIC_STATUS_H

#include <Arduino.h>

struct LaneStatus
{
    String name;
    int vehicles;
    int waiting;
    float priority;
};

struct TrafficStatus
{
    String project;
    String version;

    String currentLane;
    String signalState;

    LaneStatus lanes[4];
};

#endif