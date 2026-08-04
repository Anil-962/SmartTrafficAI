#ifndef HTTP_CLIENT_MANAGER_H
#define HTTP_CLIENT_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class HttpClientManager
{
private:

    String serverURL;

public:

    HttpClientManager();

    void begin(String masterIP);

    bool sendJSON(String json);

};

#endif