#include "HttpServer.h"

HttpServer::HttpServer() : server(80)
{
}

void HttpServer::begin()
{
    server.on("/", [this]()
    {
        server.send(200, "text/plain", "SmartTrafficAI Server Running");
    });

    server.begin();

    Serial.println("HTTP Server Started");
}

void HttpServer::handleClient()
{
    server.handleClient();
}