#ifndef TRAFFIC_CONTROLLER_H
#define TRAFFIC_CONTROLLER_H

#include "lane.h"

class TrafficController
{
private:
    Lane lanes[4];
    int currentLane;

public:
    TrafficController();

    void begin();

    void update();

    void nextLane();

    void allRed();
};

#endif