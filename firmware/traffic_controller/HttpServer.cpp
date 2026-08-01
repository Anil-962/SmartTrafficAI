#include "HttpServer.h"
#include <WiFi.h>

HttpServer::HttpServer() : server(80)
{
    controller = nullptr;
}

void HttpServer::handleRoot()
{
    server.send(
        200,
        "application/json",
        "{\"project\":\"SmartTrafficAI\",\"version\":\"2.3\",\"status\":\"running\"}"
    );
}
void HttpServer::handleStatus()
{
    if (controller == nullptr)
    {
        server.send(500, "text/plain", "Controller Missing");
        return;
    }

    TrafficStatus status = controller->getStatus();

    String json = "{";

    json += "\"project\":\"";
    json += status.project;
    json += "\",";

    json += "\"version\":\"";
    json += status.version;
    json += "\",";

    json += "\"currentLane\":\"";
    json += status.currentLane;
    json += "\",";

    json += "\"signalState\":\"";
    json += status.signalState;
    json += "\",";

    json += "\"wifi\":\"";
    json += status.wifi;
    json += "\",";

    json += "\"rssi\":";
    json += String(status.rssi);
    json += ",";

    json += "\"uptime\":";
    json += String(status.uptime);
    json += ",";

    json += "\"lanes\":[";

    for (int i = 0; i < 4; i++)
    {
        json += "{";

        json += "\"name\":\"";
        json += status.lanes[i].name;
        json += "\",";

        json += "\"vehicles\":";
        json += String(status.lanes[i].vehicles);
        json += ",";

        json += "\"waiting\":";
        json += String(status.lanes[i].waiting);
        json += ",";
                json += "\"priority\":";
        json += String(status.lanes[i].priority, 1);

        json += "}";

        if (i < 3)
        {
            json += ",";
        }
    }

    json += "]";

    json += "}";

    server.send(
        200,
        "application/json",
        json
    );
}

void HttpServer::begin(TrafficController* ctrl)
{
    controller = ctrl;

    server.enableCORS(true);

    server.on(
        "/",
        [this]()
        {
            handleRoot();
        });

    server.on(
        "/status",
        [this]()
        {
            handleStatus();
        });

    server.begin();

    Serial.println();
    Serial.println("==================================");
    Serial.println(" SmartTrafficAI HTTP Server");
    Serial.println("==================================");
    Serial.print("Server Running : http://");
    Serial.println(WiFi.localIP());
    Serial.println();
}
void HttpServer::update()
{
    server.handleClient();
}