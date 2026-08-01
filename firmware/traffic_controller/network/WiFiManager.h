#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager
{
public:
    void begin();

    bool isConnected();

    String getIP();
};

#endif