#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <Arduino.h>

class TrafficLight
{
private:
    int redPin;
    int yellowPin;
    int greenPin;

public:
    // Constructors
    TrafficLight();
    TrafficLight(int red, int yellow, int green);

    // Initialization
    void begin();

    // Signal Control
    void red();
    void yellow();
    void green();
    void off();

    // Utility
    bool isValid() const;

    // Getters
    int getRedPin() const;
    int getYellowPin() const;
    int getGreenPin() const;
};

#endif