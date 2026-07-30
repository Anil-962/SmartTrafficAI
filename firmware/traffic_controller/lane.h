#ifndef LANE_H
#define LANE_H

#include "traffic_light.h"

class Lane
{
private:
    TrafficLight signal;

    int vehicleCount;
    int waitingTime;
    bool emergency;

public:
    Lane(int redPin, int yellowPin, int greenPin);

    void begin();

    void setVehicleCount(int count);
    int getVehicleCount() const;

    void incrementWaitingTime();
    void resetWaitingTime();
    int getWaitingTime() const;

    void setEmergency(bool status);
    bool isEmergency() const;

    float getPriorityScore() const;

    TrafficLight& getTrafficLight();
};

#endif