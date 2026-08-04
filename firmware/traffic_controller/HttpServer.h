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

    // Existing Endpoints
    void handleRoot();
    void handleStatus();

    // New Endpoint
    void handleSensor();

public:

    HttpServer();

    void begin(TrafficController* ctrl);

    void update();
};

#endif