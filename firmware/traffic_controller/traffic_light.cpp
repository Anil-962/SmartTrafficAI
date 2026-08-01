#include "traffic_light.h"

//--------------------------------------------------
// Constructors
//--------------------------------------------------

TrafficLight::TrafficLight()
{
    redPin = -1;
    yellowPin = -1;
    greenPin = -1;
}

TrafficLight::TrafficLight(int red, int yellow, int green)
{
    redPin = red;
    yellowPin = yellow;
    greenPin = green;
}

//--------------------------------------------------
// Initialization
//--------------------------------------------------

void TrafficLight::begin()
{
    if (!isValid())
        return;

    pinMode(redPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    pinMode(greenPin, OUTPUT);

    off();
}

//--------------------------------------------------
// States
//--------------------------------------------------

void TrafficLight::red()
{
    if (!isValid())
        return;

    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
}

void TrafficLight::yellow()
{
    if (!isValid())
        return;

    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(greenPin, LOW);
}

void TrafficLight::green()
{
    if (!isValid())
        return;

    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, HIGH);
}

void TrafficLight::off()
{
    if (!isValid())
        return;

    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
}

//--------------------------------------------------
// Utilities
//--------------------------------------------------

bool TrafficLight::isValid() const
{
    return (redPin >= 0 &&
            yellowPin >= 0 &&
            greenPin >= 0);
}

//--------------------------------------------------
// Getters
//--------------------------------------------------

int TrafficLight::getRedPin() const
{
    return redPin;
}

int TrafficLight::getYellowPin() const
{
    return yellowPin;
}

int TrafficLight::getGreenPin() const
{
    return greenPin;
}