#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

class WiFiManager
{
private:

    bool connected;

public:

    WiFiManager();

    void begin();

    void reconnect();

    bool isConnected();

    String getIP();

};

#endif