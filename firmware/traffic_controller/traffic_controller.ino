#include "traffic_controller.h"
// #include "WiFiManager.h"
// #include "HttpServer.h"

TrafficController controller;

// WiFiManager wifi;
// HttpServer httpServer;

void setup()
{
    Serial.begin(115200);

    Serial.println("Setup Started");

    controller.begin();

    Serial.println("Controller Started");
}

void loop()
{
    controller.update();
}