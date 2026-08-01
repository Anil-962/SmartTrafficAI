#include "traffic_controller.h"
#include "WiFiManager.h"
#include "HttpServer.h"

TrafficController controller;
WiFiManager wifi;
HttpServer httpServer;

void setup()
{
    Serial.begin(115200);

    controller.begin();

    wifi.begin();

    if(wifi.connected())
    {
        httpServer.begin(&controller);
    }
}

void loop()
{
    controller.update();

    if(wifi.connected())
    {
        httpServer.update();
    }
}