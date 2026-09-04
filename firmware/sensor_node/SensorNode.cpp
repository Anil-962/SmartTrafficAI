#include "SensorNode.h"
SensorNode::SensorNode()
{
    sensors[0] = {"North", 13, 12, 0};
    sensors[1] = {"East", 14, 27, 0};
    sensors[2] = {"South", 26, 25, 0};
    sensors[3] = {"West", 33, 32, 0};
}
void SensorNode::begin()
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("SmartTrafficAI Sensor Node");
    Serial.println("==============================");

    for(int i = 0; i < 4; i++)
    {
        pinMode(sensors[i].trigPin, OUTPUT);
        pinMode(sensors[i].echoPin, INPUT);

        digitalWrite(sensors[i].trigPin, LOW);
    }

    delay(1000);

    Serial.println("Sensors Initialized");
    Serial.println();
}

float SensorNode::readDistance(uint8_t trigPin,
                               uint8_t echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);

    if(duration == 0)
    {
        return -1;
    }

    float distance = duration * 0.0343 / 2.0;

    if(distance < 2 || distance > 100)
    {
        return -1;
    }

    return distance;
}

void SensorNode::update()
{
    Serial.println("==============================");

    for(int i = 0; i < 4; i++)
    {
        sensors[i].distance =
            readDistance(
                sensors[i].trigPin,
                sensors[i].echoPin
            );

        Serial.print(sensors[i].name);
        Serial.print(" : ");

        if(sensors[i].distance < 0)
        {
            Serial.println("Out of Range");
        }
        else
        {
            Serial.print(sensors[i].distance);
            Serial.println(" cm");
        }

        delay(100);
    }

    Serial.println("==============================");
}

float SensorNode::getNorth()
{
    return sensors[0].distance;
}

float SensorNode::getEast()
{
    return sensors[1].distance;
}

float SensorNode::getSouth()
{
    return sensors[2].distance;
}

float SensorNode::getWest()
{
    return sensors[3].distance;
}

String SensorNode::createJSON()
{
    String json = "{";

    json += "\"north\":";
    json += String(getNorth(),1);
    json += ",";

    json += "\"east\":";
    json += String(getEast(),1);
    json += ",";

    json += "\"south\":";
    json += String(getSouth(),1);
    json += ",";

    json += "\"west\":";
    json += String(getWest(),1);

    json += "}";

    return json;
}