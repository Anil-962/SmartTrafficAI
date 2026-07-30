#include <Arduino.h>
#include "traffic_controller.h"

TrafficController controller;

void setup()
{
    Serial.begin(115200);

    controller.begin();
}

void loop()
{
    controller.update();
}