#include "HttpServer.h"
#include <ArduinoJson.h>

HttpServer::HttpServer() : server(80)
{
}

void HttpServer::begin()
{
    server.on("/", [this]()
    {
        server.send(200, "text/plain", "SmartTrafficAI Server Running");
    });

    server.on("/status", [this]()
    {
        JsonDocument doc;

        doc["project"] = "SmartTrafficAI";
        doc["version"] = "1.1";
        doc["status"] = "running";

        String response;

        serializeJson(doc, response);

        server.send(200, "application/json", response);
    });

    server.begin();

    Serial.println("==============================");
    Serial.println("HTTP Server Started");
    Serial.println("==============================");
}

void HttpServer::handleClient()
{
    server.handleClient();
}