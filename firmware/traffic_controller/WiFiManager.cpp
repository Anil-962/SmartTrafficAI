#include "WiFiManager.h"
#include "config_wifi.h"

void WiFiManager::begin()
{
    Serial.println();
    Serial.println("=========================");
    Serial.println("Connecting to WiFi...");
    Serial.println("=========================");

    WiFi.mode(WIFI_STA);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("=========================");
    Serial.println("WiFi Connected");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
    Serial.println("=========================");
}

bool WiFiManager::connected()
{
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::ipAddress()
{
    return WiFi.localIP().toString();
}