#include "WiFiManager.h"
WiFiManager::WiFiManager()
{
    connected = false;
}

void WiFiManager::begin()
{
    Serial.println();
    Serial.println("==================================");
    Serial.println(" Connecting to WiFi");
    Serial.println("==================================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 30)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        connected = true;

        Serial.println("==================================");
        Serial.println(" WiFi Connected");
        Serial.print(" IP Address : ");
        Serial.println(WiFi.localIP());
        Serial.println("==================================");
    }
    else
    {
        connected = false;

        Serial.println("==================================");
        Serial.println(" WiFi Connection Failed");
        Serial.println("==================================");
    }
}

void WiFiManager::reconnect()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        connected = true;
        return;
    }

    connected = false;

    Serial.println();
    Serial.println("WiFi Lost");
    Serial.println("Reconnecting...");

    WiFi.disconnect(true);
    delay(1000);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        connected = true;

        Serial.println("Reconnected");
        Serial.print("IP : ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        connected = false;
        Serial.println("Reconnect Failed");
    }
}

bool WiFiManager::isConnected()
{
    return connected;
}

String WiFiManager::getIP()
{
    if (connected)
    {
        return WiFi.localIP().toString();
    }

    return "0.0.0.0";
}