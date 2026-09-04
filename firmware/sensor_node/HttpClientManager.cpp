#include "HttpClientManager.h"
HttpClientManager::HttpClientManager()
{
    serverURL = "";
}

void HttpClientManager::begin(String masterIP)
{
    serverURL = "http://" + masterIP + "/sensor";

    Serial.println();
    Serial.println("==================================");
    Serial.println(" HTTP Client Initialized");
    Serial.print(" Server : ");
    Serial.println(serverURL);
    Serial.println("==================================");
}
bool HttpClientManager::sendJSON(String json)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi Not Connected");
        return false;
    }

    HTTPClient http;

    http.begin(serverURL);

    http.addHeader("Content-Type", "application/json");

    Serial.println();
    Serial.println("========== HTTP POST ==========");
    Serial.print("Sending : ");
    Serial.println(json);

    int httpCode = http.POST(json);

    Serial.print("HTTP Code : ");
    Serial.println(httpCode);

    if (httpCode > 0)
    {
        String response = http.getString();

        Serial.print("Response : ");
        Serial.println(response);

        http.end();

        return true;
    }
    else
    {
        Serial.print("HTTP Error : ");
        Serial.println(http.errorToString(httpCode));

        http.end();

        return false;
    }
}