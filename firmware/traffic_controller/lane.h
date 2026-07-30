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
    // Default constructor
    Lane();

    // Parameterized constructor
    Lane(int redPin, int yellowPin, int greenPin);

    // Initialize lane
    void begin();
    void red();
    void yellow();
    void green();
    void off();
    void addVehicle();
    void clearVehicles();

    // Vehicle count
    void setVehicleCount(int count);
    int getVehicleCount() const;

    // Waiting time
    void incrementWaitingTime();
    void resetWaitingTime();
    int getWaitingTime() const;

    // Emergency
    void setEmergency(bool status);
    bool isEmergency() const;

    // Priority
    float getPriorityScore() const;

    // Traffic Light Access
    TrafficLight& getTrafficLight();
};

#endif