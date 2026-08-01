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

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - start < 10000)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("=========================");
        Serial.println("WiFi Connected");
        Serial.print("IP Address : ");
        Serial.println(WiFi.localIP());
        Serial.println("=========================");
    }
    else
    {
        Serial.println("=========================");
        Serial.println("WiFi Connection Failed");
        Serial.println("Traffic Controller will continue.");
        Serial.println("=========================");
    }
}

bool WiFiManager::connected()
{
    return WiFi.status() == WL_CONNECTED;
}