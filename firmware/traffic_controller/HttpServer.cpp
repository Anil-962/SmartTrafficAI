#include "HttpServer.h"

HttpServer::HttpServer() : server(80)
{
    controller = nullptr;
}

void HttpServer::handleRoot()
{


    server.send(
        200,
        "application/json",
        "{\"project\":\"SmartTrafficAI\",\"version\":\"2.0\",\"status\":\"running\"}"
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
    json += "\"";

    json += "}";

    // Enable CORS

    server.send(
        200,
        "application/json",
        json
    );
}

void HttpServer::begin(TrafficController* ctrl)
{
    server.enableCORS(true);
    controller = ctrl;

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
    Serial.println("=========================");
    Serial.println("HTTP Server Running");
    Serial.println("=========================");
}

void HttpServer::update()
{
    server.handleClient();
}