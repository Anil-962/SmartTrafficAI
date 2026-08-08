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
        "{\"project\":\"SmartTrafficAI\",\"version\":\"3.1\",\"status\":\"running\"}"
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

    StaticJsonDocument<1536> doc;

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
        lane["emergency"] = status.lanes[i].emergency;
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

    String body = server.arg("plain");

    Serial.println();
    Serial.println("========================================");
    Serial.println("         SENSOR DATA RECEIVED");
    Serial.println("========================================");

    Serial.print("JSON : ");
    Serial.println(body);

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

    float north = doc["north"] | -1.0;
    float east  = doc["east"]  | -1.0;
    float south = doc["south"] | -1.0;
    float west  = doc["west"]  | -1.0;

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

    controller->updateSensorData(
        north,
        east,
        south,
        west
    );

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

// =====================================================
// EMERGENCY
// POST /emergency
//
// JSON:
// {
//     "lane": 2
// }
//
// 0 = North
// 1 = East
// 2 = South
// 3 = West
// =====================================================

void HttpServer::handleEmergency()
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

    if (!server.hasArg("plain"))
    {
        server.send(
            400,
            "text/plain",
            "Missing JSON"
        );

        return;
    }

    String body = server.arg("plain");

    Serial.println();
    Serial.println("========================================");
    Serial.println("       EMERGENCY REQUEST RECEIVED");
    Serial.println("========================================");

    Serial.print("JSON : ");
    Serial.println(body);

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

    if (!doc.containsKey("lane"))
    {
        server.send(
            400,
            "text/plain",
            "Missing lane"
        );

        return;
    }

    int lane = doc["lane"];

    if (lane < 0 || lane > 3)
    {
        server.send(
            400,
            "text/plain",
            "Invalid lane. Use 0-3"
        );

        return;
    }

    controller->setEmergencyLane(lane);

    server.send(
        200,
        "application/json",
        "{\"status\":\"emergency_set\",\"success\":true}"
    );

    Serial.println("Emergency lane set successfully");
    Serial.println("HTTP Response : 200 OK");
    Serial.println("========================================");
}

// =====================================================
// CLEAR EMERGENCY
// POST /clear-emergency
//
// JSON:
// {
//     "lane": 2
// }
//
// If lane is omitted, all emergencies are cleared.
// =====================================================

void HttpServer::handleClearEmergency()
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

    if (!server.hasArg("plain"))
    {
        controller->clearAllEmergency();

        server.send(
            200,
            "application/json",
            "{\"status\":\"all_emergency_cleared\",\"success\":true}"
        );

        return;
    }

    String body = server.arg("plain");

    StaticJsonDocument<256> doc;

    DeserializationError error =
        deserializeJson(doc, body);

    if (error)
    {
        server.send(
            400,
            "text/plain",
            "Invalid JSON"
        );

        return;
    }

    if (!doc.containsKey("lane"))
    {
        controller->clearAllEmergency();

        server.send(
            200,
            "application/json",
            "{\"status\":\"all_emergency_cleared\",\"success\":true}"
        );

        return;
    }

    int lane = doc["lane"];

    if (lane < 0 || lane > 3)
    {
        server.send(
            400,
            "text/plain",
            "Invalid lane. Use 0-3"
        );

        return;
    }

    controller->clearEmergencyLane(lane);

    server.send(
        200,
        "application/json",
        "{\"status\":\"emergency_cleared\",\"success\":true}"
    );

    Serial.print("Emergency cleared for lane : ");
    Serial.println(lane);
}
void HttpServer::begin(TrafficController* ctrl)
{
    controller = ctrl;

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

    server.on(
        "/emergency",
        HTTP_POST,
        std::bind(
            &HttpServer::handleEmergency,
            this
        )
    );

    server.on(
        "/clear-emergency",
        HTTP_POST,
        std::bind(
            &HttpServer::handleClearEmergency,
            this
        )
    );

    // Start server
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
    Serial.println("POST /emergency");
    Serial.println("POST /clear-emergency");

    Serial.println("==================================");
}

void HttpServer::update()
{
    server.handleClient();
}