#ifndef SENSOR_NODE_H
#define SENSOR_NODE_H

#include <Arduino.h>

struct Sensor
{
    String name;
    uint8_t trigPin;
    uint8_t echoPin;
    float distance;
};

class SensorNode
{
private:

    Sensor sensors[4];

    float readDistance(uint8_t trigPin,
                       uint8_t echoPin);

public:

    SensorNode();

    void begin();

    void update();

    float getNorth();

    float getEast();

    float getSouth();

    float getWest();

    String createJSON();

};

#endif