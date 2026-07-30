#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

class TrafficLight
{
private:
    int redPin;
    int yellowPin;
    int greenPin;

public:
    // Default constructor
    TrafficLight();

    // Parameterized constructor
    TrafficLight(int red, int yellow, int green);

    void begin();

    void red();
    void yellow();
    void green();
    void off();
};

#endif