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

    Serial.println("================================");
    Serial.println("HTTP Server Started");
    Serial.println("================================");
}

void HttpServer::handleClient()
{
    server.handleClient();
}