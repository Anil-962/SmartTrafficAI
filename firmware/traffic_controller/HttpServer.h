#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <WebServer.h>
#include "traffic_controller.h"

class HttpServer
{
private:
    WebServer server;

    TrafficController* controller;

    void handleRoot();

    void handleStatus();

public:

    HttpServer();

    void begin(TrafficController* ctrl);

    void update();
};

#endif