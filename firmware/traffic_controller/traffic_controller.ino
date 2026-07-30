#include <Arduino.h>
#include "traffic_controller.h"

TrafficController controller;

void setup()
{
    Serial.begin(115200);

    Serial.println("=================================");
    Serial.println("Smart Traffic Management System");
    Serial.println("Initializing...");
    Serial.println("=================================");

    controller.begin();

    Serial.println("System Ready");
}

void loop()
{
    controller.update();
}