#include "lane.h"
#include "config.h"

// Default constructor
Lane::Lane()
{
    vehicleCount = 0;
    waitingTime = 0;
    emergency = false;
}

// Parameterized constructor
Lane::Lane(int redPin, int yellowPin, int greenPin)
    : signal(redPin, yellowPin, greenPin)
{
    vehicleCount = 0;
    waitingTime = 0;
    emergency = false;
}
void Lane::red()
{
    signal.red();
}

void Lane::yellow()
{
    signal.yellow();
}

void Lane::green()
{
    signal.green();
}

void Lane::off()
{
    signal.off();
}

void Lane::begin()
{
    signal.begin();
}

void Lane::setVehicleCount(int count)
{
    vehicleCount = count;
}

int Lane::getVehicleCount() const
{
    return vehicleCount;
}

void Lane::incrementWaitingTime()
{
    waitingTime++;
}

void Lane::resetWaitingTime()
{
    waitingTime = 0;
}

int Lane::getWaitingTime() const
{
    return waitingTime;
}

void Lane::setEmergency(bool status)
{
    emergency = status;
}

bool Lane::isEmergency() const
{
    return emergency;
}

float Lane::getPriorityScore() const
{
    float score =
        vehicleCount * VEHICLE_WEIGHT +
        waitingTime * WAITING_WEIGHT;

    if (emergency)
    {
        score += EMERGENCY_WEIGHT;
    }

    return score;
}

TrafficLight& Lane::getTrafficLight()
{
    return signal;
}
void Lane::addVehicle()
{
    vehicleCount++;
}

void Lane::clearVehicles()
{
    vehicleCount = 0;
}
void Lane::removeVehicles(int count)
{
    vehicleCount -= count;

    if (vehicleCount < 0)
        vehicleCount = 0;
}
 void Lane::addVehicles(int count)
    {
    vehicleCount += count;
    }