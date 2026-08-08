#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <Arduino.h>
#include <WebServer.h>
#include "traffic_controller.h"

class HttpServer
{
private:

    WebServer server;

    TrafficController* controller;
    void handleRoot();

    void handleStatus();

    void handleSensor();

    void handleEmergency();

    void handleClearEmergency();

public:
    HttpServer();
    void begin(TrafficController* ctrl);

    void update();
};

#endif