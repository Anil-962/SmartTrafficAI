#include "Arduino.h"
#include "traffic_light.h"

TrafficLight::TrafficLight(int red, int yellow, int green)
{
    redPin = red;
    yellowPin = yellow;
    greenPin = green;
}

void TrafficLight::begin()
{
    pinMode(redPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    pinMode(greenPin, OUTPUT);

    off();
}

void TrafficLight::red()
{
    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
}

void TrafficLight::yellow()
{
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(greenPin, LOW);
}

void TrafficLight::green()
{
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, HIGH);
}

void TrafficLight::off()
{
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
}