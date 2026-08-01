#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <WebServer.h>

class HttpServer
{
private:

    WebServer server;

public:

    HttpServer();

    void begin();

    void handleClient();
};

#endif