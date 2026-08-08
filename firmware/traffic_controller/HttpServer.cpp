#include "HttpServer.h"
#include <WiFi.h>
#include <ArduinoJson.h>

HttpServer::HttpServer() : server(80)
{
    controller = nullptr;
}

void HttpServer::handleRoot()
{
    server.send(
        200,
        "application/json",
        "{\"project\":\"SmartTrafficAI\",\"version\":\"2.4\",\"status\":\"running\"}"
    );
}

void HttpServer::handleStatus()
{
    if (controller == nullptr)
    {
        server.send(
            500,
            "text/plain",
            "Controller Missing"
        );

        return;
    }

    TrafficStatus status = controller->getStatus();

    StaticJsonDocument<1024> doc;

    doc["project"] = status.project;
    doc["version"] = status.version;
    doc["currentLane"] = status.currentLane;
    doc["signalState"] = status.signalState;
    doc["wifi"] = status.wifi;
    doc["rssi"] = status.rssi;
    doc["uptime"] = status.uptime;

    JsonArray lanes = doc.createNestedArray("lanes");

    for (int i = 0; i < 4; i++)
    {
        JsonObject lane = lanes.createNestedObject();

        lane["name"] = status.lanes[i].name;
        lane["vehicles"] = status.lanes[i].vehicles;
        lane["waiting"] = status.lanes[i].waiting;
        lane["priority"] = status.lanes[i].priority;
    }

    String json;

    serializeJson(doc, json);

    server.send(
        200,
        "application/json",
        json
    );
}
void HttpServer::handleSensor()
{
    if (controller == nullptr)
    {
        server.send(
            500,
            "text/plain",
            "Controller Missing"
        );

        return;
    }

    // Check whether JSON body exists
    if (!server.hasArg("plain"))
    {
        Serial.println("ERROR: No JSON body received");

        server.send(
            400,
            "text/plain",
            "Missing JSON"
        );

        return;
    }

    // Get JSON body
    String body = server.arg("plain");

    Serial.println();
    Serial.println("========================================");
    Serial.println("         SENSOR DATA RECEIVED");
    Serial.println("========================================");

    Serial.print("JSON : ");
    Serial.println(body);

    // Parse JSON
    StaticJsonDocument<256> doc;

    DeserializationError error =
        deserializeJson(doc, body);

    if (error)
    {
        Serial.print("JSON Parse Error : ");
        Serial.println(error.c_str());

        server.send(
            400,
            "text/plain",
            "Invalid JSON"
        );

        return;
    }

    // Read sensor values
    float north = doc["north"] | -1.0;
    float east  = doc["east"]  | -1.0;
    float south = doc["south"] | -1.0;
    float west  = doc["west"]  | -1.0;

    // Display values
    Serial.println();
    Serial.println("Sensor Distances:");

    Serial.print("North : ");
    Serial.println(north);

    Serial.print("East  : ");
    Serial.println(east);

    Serial.print("South : ");
    Serial.println(south);

    Serial.print("West  : ");
    Serial.println(west);

    // Update traffic controller
    controller->updateSensorData(
        north,
        east,
        south,
        west
    );

    // Send successful response
    server.send(
        200,
        "text/plain",
        "OK"
    );

    Serial.println();
    Serial.println("Sensor Data Updated Successfully");
    Serial.println("HTTP Response : 200 OK");
    Serial.println("========================================");
}
void HttpServer::begin(TrafficController* ctrl)
{
    controller = ctrl;

    // Enable CORS for dashboard
    server.enableCORS(true);
    server.on(
        "/",
        HTTP_GET,
        std::bind(
            &HttpServer::handleRoot,
            this
        )
    );
    server.on(
        "/status",
        HTTP_GET,
        std::bind(
            &HttpServer::handleStatus,
            this
        )
    );
    server.on(
        "/sensor",
        HTTP_POST,
        std::bind(
            &HttpServer::handleSensor,
            this
        )
    );

    // Start HTTP server
    server.begin();

    Serial.println();
    Serial.println("==================================");
    Serial.println(" SmartTrafficAI HTTP Server");
    Serial.println("==================================");

    Serial.print("Server Running : http://");
    Serial.println(WiFi.localIP());

    Serial.println();
    Serial.println("Available Endpoints:");
    Serial.println("GET  /");
    Serial.println("GET  /status");
    Serial.println("POST /sensor");

    Serial.println("==================================");
}
void HttpServer::update()
{
    server.handleClient();
}