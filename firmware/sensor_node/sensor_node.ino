#include "SensorNode.h"
#include "WiFiManager.h"
#include "HttpClientManager.h"
#include "config.h"

SensorNode sensorNode;
WiFiManager wifiManager;
HttpClientManager httpClient;

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("========================================");
    Serial.println(" SmartTrafficAI Sensor Node");
    Serial.println("========================================");

    // Initialize Sensors
    sensorNode.begin();

    // Connect WiFi
    wifiManager.begin();

    // Initialize HTTP Client
    httpClient.begin(MASTER_IP);

    Serial.println();
    Serial.println("System Ready");
    Serial.println("========================================");
}

void loop()
{
  
    if (!wifiManager.isConnected())
    {
        wifiManager.reconnect();
    }

    sensorNode.update();

    String json = sensorNode.createJSON();

    Serial.println();
    Serial.println("Generated JSON");
    Serial.println(json);

    bool success = httpClient.sendJSON(json);

    if(success)
    {
        Serial.println("Data Sent Successfully");
    }
    else
    {
        Serial.println("Failed to Send Data");
    }

    Serial.println("----------------------------------------");

    delay(1000);
}