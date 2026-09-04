#ifndef TRAFFIC_CONTROLLER_H
#define TRAFFIC_CONTROLLER_H

#include <Arduino.h>
#include "Lane.h"

enum TrafficState
{
    PRE_GREEN_YELLOW_STATE,
    GREEN_STATE,
    YELLOW_STATE,
    ALL_RED_STATE
};

struct LaneStatus
{
    String name;

    int vehicles;
    int waiting;

    float priority;

    bool emergency;
};

struct TrafficStatus
{
    String project;
    String version;

    String currentLane;
    String signalState;

    String wifi;

    int rssi;

    unsigned long uptime;

    LaneStatus lanes[4];
};

class TrafficController
{
private:

    Lane lanes[4];

    int currentLane;
    int currentState;
    bool yellowBeforeGreen;

    unsigned long previousMillis;
    unsigned long stateDuration;
    int densityLevel[4];

    bool emergency[4];

    float filteredDistance[4];

    int invalidReadings[4];

    bool sensorInitialized[4];

    static constexpr float FILTER_ALPHA = 0.40f;

    static constexpr int MAX_INVALID_READINGS = 3;


    void changeState();

    void allRed();

    void nextLane();

    void scheduleNextLane();

    void updateWaitingTimes();

    void simulateTraffic();

    void printStatus();

    int getHighestPriorityLane();
    unsigned long calculateGreenTime(int lane);

    int getEmergencyLane();

    String getCurrentLaneName();

    String getCurrentStateName();

    float calibrateDistance(
        int lane,
        float rawDistance
    );
    int getStableDensity(
    int lane,
    float distance
    );
    int distanceToVehicles(
        float distance
    );
enum TrafficEventType
{
    EVENT_NONE,
    EVENT_CONGESTION,
    EVENT_SUDDEN_TRAFFIC,
    EVENT_LANE_BLOCKED,
    EVENT_ABNORMAL
};

TrafficEventType currentEvent;

unsigned long eventStartTime;

int previousHighestDensity;
unsigned long densityChangeTime;
TrafficEventType pendingEvent;

unsigned long pendingEventStartTime;

const unsigned long EVENT_CONFIRMATION_TIME = 5000;
enum TrafficSeverity
{
    SEVERITY_NORMAL,
    SEVERITY_LOW,
    SEVERITY_MEDIUM,
    SEVERITY_HIGH,
    SEVERITY_CRITICAL
};

TrafficSeverity currentSeverity;


void calculateEventSeverity();
void detectTrafficEvents();


public:

  
    TrafficController();

    void begin();

    void update();
    String getTrafficEvent();

    TrafficStatus getStatus();
    String getSeverityName();
    String getEventName();

    Lane* getLanes();

    int getCurrentLane();

    String getSignalState();
    void updateSensorData(
        float north,
        float east,
        float south,
        float west
    );
    void setEmergencyLane(int lane);

    void clearEmergencyLane(int lane);

    void clearAllEmergency();

    bool isEmergencyLane(int lane);
};

#endif