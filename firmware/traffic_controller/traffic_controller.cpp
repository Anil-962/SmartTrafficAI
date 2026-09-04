#include <Arduino.h>
#include "traffic_controller.h"
#include "config.h"
#include <WiFi.h>

TrafficController::TrafficController()
{
    lanes[0] = Lane(NORTH_RED, NORTH_YELLOW, NORTH_GREEN);
    lanes[1] = Lane(EAST_RED, EAST_YELLOW, EAST_GREEN);
    lanes[2] = Lane(SOUTH_RED, SOUTH_YELLOW, SOUTH_GREEN);
    lanes[3] = Lane(WEST_RED, WEST_YELLOW, WEST_GREEN);

    currentLane = 0;
    currentState = PRE_GREEN_YELLOW_STATE;
    yellowBeforeGreen = true;
    currentSeverity = SEVERITY_NORMAL;

    previousMillis = 0;
    stateDuration = 2000;
    currentEvent = EVENT_NONE;
    eventStartTime = 0;
    previousHighestDensity = 0;
    densityChangeTime = 0;

    pendingEvent = EVENT_NONE;
    pendingEventStartTime = 0;

    // -------------------------------------------------
    // Emergency
    // -------------------------------------------------

    for (int i = 0; i < 4; i++)
    {
        emergency[i] = false;
    }

    // -------------------------------------------------
    // Sensor Calibration
    // -------------------------------------------------

    for (int i = 0; i < 4; i++)
    {
        filteredDistance[i] = -1.0;

        invalidReadings[i] = 0;

        sensorInitialized[i] = false;
        densityLevel[i] = 0;
    }
}

// =====================================================
// Begin
// =====================================================

void TrafficController::begin()
{
    randomSeed(millis());

    for (int i = 0; i < 4; i++)
    {
        lanes[i].begin();
        lanes[i].red();
    }

    // -------------------------------------------------
    // Initial demo values
    // -------------------------------------------------

    lanes[0].setVehicleCount(10);
    lanes[1].setVehicleCount(2);
    lanes[2].setVehicleCount(6);
    lanes[3].setVehicleCount(1);

    // -------------------------------------------------
    // Clear Emergency
    // -------------------------------------------------

    clearAllEmergency();

    // -------------------------------------------------
    // Reset Sensor Filtering
    // -------------------------------------------------

    for (int i = 0; i < 4; i++)
    {
        filteredDistance[i] = -1.0;

        invalidReadings[i] = 0;

        sensorInitialized[i] = false;
        densityLevel[i] = 0;
    }

    // -------------------------------------------------
    // Select Initial Lane
    // -------------------------------------------------

    currentLane = getHighestPriorityLane();

    lanes[currentLane].yellow();

    currentState = PRE_GREEN_YELLOW_STATE;
    yellowBeforeGreen = true;

    stateDuration = 3000;

    previousMillis = millis();

    printStatus();
}

// =====================================================
// All Red
// =====================================================

void TrafficController::allRed()
{
    for (int i = 0; i < 4; i++)
    {
        lanes[i].red();
    }
}

// =====================================================
// Next Lane
// =====================================================

void TrafficController::nextLane()
{
    currentLane++;

    if (currentLane >= 4)
    {
        currentLane = 0;
    }
}
// =====================================================
// ADAPTIVE GREEN TIME
// =====================================================

unsigned long TrafficController::calculateGreenTime(int lane)
{
    if (lane < 0 || lane >= 4)
    {
        return MIN_GREEN_TIME * 1000UL;
    }

    int vehicles =
        lanes[lane].getVehicleCount();

    int waiting =
        lanes[lane].getWaitingTime();

    // -------------------------------------------------
    // Base green time
    // -------------------------------------------------

    unsigned long greenTime =
        MIN_GREEN_TIME;

    // -------------------------------------------------
    // Add time according to traffic density
    // -------------------------------------------------

    if (vehicles >= 10)
    {
        greenTime = 35;
    }
    else if (vehicles >= 8)
    {
        greenTime = 30;
    }
    else if (vehicles >= 6)
    {
        greenTime = 25;
    }
    else if (vehicles >= 4)
    {
        greenTime = 20;
    }
    else if (vehicles >= 2)
    {
        greenTime = 15;
    }
    else
    {
        greenTime = 10;
    }

    // -------------------------------------------------
    // Waiting-time bonus
    // -------------------------------------------------

    if (waiting >= 20)
    {
        greenTime += 5;
    }
    else if (waiting >= 10)
    {
        greenTime += 3;
    }

    // -------------------------------------------------
    // Maximum green-time protection
    // -------------------------------------------------

    if (greenTime > 40)
    {
        greenTime = 40;
    }

    // -------------------------------------------------
    // Serial Debug
    // -------------------------------------------------

    Serial.println();
    Serial.println(
        "========== ADAPTIVE GREEN TIME =========="
    );

    Serial.print(
        "Lane : "
    );

    Serial.println(
        getCurrentLaneName()
    );

    Serial.print(
        "Vehicles/Density : "
    );

    Serial.println(
        vehicles
    );

    Serial.print(
        "Waiting Time : "
    );

    Serial.println(
        waiting
    );

    Serial.print(
        "Green Time : "
    );

    Serial.print(
        greenTime
    );

    Serial.println(
        " seconds"
    );

    Serial.println(
        "=========================================="
    );

    return greenTime * 1000UL;
}
void TrafficController::changeState()
{
    switch (currentState)
    {
        case PRE_GREEN_YELLOW_STATE:

            if (yellowBeforeGreen)
            {
                lanes[currentLane].green();

                currentState = GREEN_STATE;
                
                stateDuration =
                    calculateGreenTime(currentLane);

                Serial.println();
                Serial.println(
                    "========== GREEN SIGNAL =========="
                );

                Serial.print(
                    "Lane : "
                );

                Serial.println(
                    getCurrentLaneName()
                );

                Serial.print(
                    "Green Duration : "
                );

                Serial.print(
                    stateDuration / 1000
                );

                Serial.println(
                    " seconds"
                );

                Serial.println(
                    "=================================="
                );

                yellowBeforeGreen = false;
            }

            break;


        // =================================================
        // GREEN
        // =================================================

        case GREEN_STATE:

            // -------------------------------------------------
            // GREEN → YELLOW
            // -------------------------------------------------

            lanes[currentLane].yellow();

            currentState = YELLOW_STATE;

            stateDuration =
                YELLOW_TIME * 1000UL;

            Serial.println();
            Serial.println(
                "========== GREEN → YELLOW =========="
            );

            Serial.print(
                "Lane : "
            );

            Serial.println(
                getCurrentLaneName()
            );

            Serial.print(
                "Yellow Duration : "
            );

            Serial.print(
                YELLOW_TIME
            );

            Serial.println(
                " seconds"
            );

            Serial.println(
                "===================================="
            );

            break;


        // =================================================
        // YELLOW
        // =================================================

        case YELLOW_STATE:

            // -------------------------------------------------
            // YELLOW → ALL RED
            // -------------------------------------------------

            allRed();

            currentState = ALL_RED_STATE;

            stateDuration = 1000;

            Serial.println();
            Serial.println(
                "========== ALL RED =========="
            );

            Serial.println(
                "All signals OFF/RED"
            );

            Serial.println(
                "All Red Duration : 1 second"
            );

            Serial.println(
                "============================="
            );

            break;


        // =================================================
        // ALL RED
        // =================================================

        case ALL_RED_STATE:

            // -------------------------------------------------
            // Select next lane
            // -------------------------------------------------

            scheduleNextLane();

            // -------------------------------------------------
            // New lane starts YELLOW
            // -------------------------------------------------

            lanes[currentLane].yellow();

            currentState =
                PRE_GREEN_YELLOW_STATE;

            // -------------------------------------------------
            // Enable yellow-before-green sequence
            // -------------------------------------------------

            yellowBeforeGreen = true;

            // -------------------------------------------------
            // Yellow warning = 3 seconds
            // -------------------------------------------------

            stateDuration = 3000;

            Serial.println();
            Serial.println(
                "========== NEXT LANE =========="
            );

            Serial.print(
                "Next Lane : "
            );

            Serial.println(
                getCurrentLaneName()
            );

            Serial.println(
                "Yellow Before Green : 3 seconds"
            );

            Serial.println(
                "==============================="
            );

            break;
    }

    previousMillis = millis();
}

// =====================================================
// Main Update
// =====================================================

void TrafficController::update()
{
    if (millis() - previousMillis >= stateDuration)
    {
        changeState();

        Serial.print("Current Lane : ");
        Serial.print(getCurrentLaneName());

        Serial.print(" | State : ");
        Serial.println(getCurrentStateName());
    }
}

// =====================================================
// Print Status
// =====================================================

void TrafficController::printStatus()
{
    const char *laneNames[4] =
    {
        "North",
        "East",
        "South",
        "West"
    };

    for (int i = 0; i < 4; i++)
    {
        Serial.print("Lane Index = ");
        Serial.println(i);

        Serial.print("Name = ");
        Serial.println(laneNames[i]);

        Serial.print("Vehicles = ");
        Serial.println(lanes[i].getVehicleCount());

        Serial.print("Waiting = ");
        Serial.println(lanes[i].getWaitingTime());

        Serial.print("Priority = ");
        Serial.println(lanes[i].getPriorityScore());

        Serial.print("Emergency = ");
        Serial.println(
            emergency[i] ? "YES" : "NO"
        );

        Serial.println("----------------");
    }
}

// =====================================================
// Highest Priority Lane
// =====================================================

int TrafficController::getHighestPriorityLane()
{
    int bestLane = 0;

    float highestPriority =
        lanes[0].getPriorityScore();

    for (int i = 1; i < 4; i++)
    {
        float priority =
            lanes[i].getPriorityScore();

        if (priority > highestPriority)
        {
            highestPriority = priority;

            bestLane = i;
        }
    }

    return bestLane;
}

// =====================================================
// Waiting Time
// =====================================================

void TrafficController::updateWaitingTimes()
{
    for (int i = 0; i < 4; i++)
    {
        if (i == currentLane)
        {
            lanes[i].resetWaitingTime();
        }
        else
        {
            lanes[i].incrementWaitingTime();
        }
    }
}

// =====================================================
// Emergency Lane
// =====================================================

int TrafficController::getEmergencyLane()
{
    // -------------------------------------------------
    // Check our emergency flags
    // -------------------------------------------------

    for (int i = 0; i < 4; i++)
    {
        if (emergency[i])
        {
            return i;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (lanes[i].isEmergency())
        {
            return i;
        }
    }

    return -1;
}

void TrafficController::scheduleNextLane()
{
    updateWaitingTimes();

    lanes[currentLane].removeVehicles(3);

    int emergencyLane =
        getEmergencyLane();

    if (emergencyLane != -1)
    {
        currentLane = emergencyLane;

        Serial.println();
        Serial.println(
            " EMERGENCY VEHICLE PRIORITY"
        );

        Serial.print(
            "Emergency Lane : "
        );

        Serial.println(
            getCurrentLaneName()
        );
    }
    else
    {

        currentLane =
            getHighestPriorityLane();
    }

    printStatus();
}

String TrafficController::getCurrentLaneName()
{
    switch (currentLane)
    {
        case 0:
            return "North";

        case 1:
            return "East";

        case 2:
            return "South";

        case 3:
            return "West";

        default:
            return "Unknown";
    }
}

String TrafficController::getCurrentStateName()
{
    switch (currentState)
    {
        case GREEN_STATE:
            return "GREEN";

        case YELLOW_STATE:
            return "YELLOW";

        case ALL_RED_STATE:
            return "ALL_RED";

        case PRE_GREEN_YELLOW_STATE:
            return "PRE_GREEN_YELLOW";

        default:
            return "UNKNOWN";
    }
}

TrafficStatus TrafficController::getStatus()
{
    TrafficStatus status;

    status.project = "SmartTrafficAI";

    status.version = "3.1";

    status.currentLane =
        getCurrentLaneName();

    status.signalState =
        getCurrentStateName();

    status.wifi = "Connected";

    status.rssi = WiFi.RSSI();

    status.uptime = millis() / 1000;

    const char* names[4] =
    {
        "North",
        "East",
        "South",
        "West"
    };

    for (int i = 0; i < 4; i++)
    {
        status.lanes[i].name =
            names[i];

        status.lanes[i].vehicles =
            lanes[i].getVehicleCount();

        status.lanes[i].waiting =
            lanes[i].getWaitingTime();

        status.lanes[i].priority =
            lanes[i].getPriorityScore();

        status.lanes[i].emergency =
            emergency[i] ||
            lanes[i].isEmergency();
    }

    return status;
}

float TrafficController::calibrateDistance(
    int lane,
    float rawDistance)
{
    if (lane < 0 || lane >= 4)
    {
        return -1.0;
    }

    if (rawDistance < 0)
    {
        invalidReadings[lane]++;

        // Keep the previous valid value
        // for a few bad readings.
        if (
            sensorInitialized[lane] &&
            invalidReadings[lane] <
            MAX_INVALID_READINGS
        )
        {
            return filteredDistance[lane];
        }

        // Too many invalid readings
        // means we consider the lane empty.
        if (
            invalidReadings[lane] >=
            MAX_INVALID_READINGS
        )
        {
            filteredDistance[lane] = -1.0;

            sensorInitialized[lane] = false;

            return -1.0;
        }

        return -1.0;
    }

    invalidReadings[lane] = 0;

    if (!sensorInitialized[lane])
    {
        filteredDistance[lane] =
            rawDistance;

        sensorInitialized[lane] = true;

        return filteredDistance[lane];
    }

    filteredDistance[lane] =
        (
            FILTER_ALPHA * rawDistance
        )
        +
        (
            (1.0 - FILTER_ALPHA)
            * filteredDistance[lane]
        );

    return filteredDistance[lane];
}

int TrafficController::distanceToVehicles(
    float distance)
{
    if (distance < 0)
    {
        return 0;
    }

    if (distance <= 10)
    {
        return 10;
    }

    if (distance <= 20)
    {
        return 8;
    }

    if (distance <= 30)
    {
        return 6;
    }

    if (distance <= 40)
    {
        return 4;
    }

    if (distance <= 60)
    {
        return 2;
    }

    return 0;
}

int TrafficController::getStableDensity(
    int lane,
    float distance
)
{
    if (lane < 0 || lane >= 4)
    {
        return 0;
    }

    if (distance < 0)
    {
        densityLevel[lane] = 0;

        return 0;
    }

    int currentLevel =
        densityLevel[lane];


    if (currentLevel == 0)
    {
        int newLevel =
            distanceToVehicles(distance);

        densityLevel[lane] =
            newLevel;

        return newLevel;
    }

    // Critical → Very High
    if (currentLevel == 10)
    {
        if (distance > 11.5)
        {
            densityLevel[lane] = 8;
        }

        return densityLevel[lane];
    }

    // Very High → Critical
    if (currentLevel == 8)
    {
        if (distance <= 9.0)
        {
            densityLevel[lane] = 10;
        }
        else if (distance > 21.0)
        {
            densityLevel[lane] = 6;
        }

        return densityLevel[lane];
    }

    // High → Very High / Moderate
    if (currentLevel == 6)
    {
        if (distance <= 19.0)
        {
            densityLevel[lane] = 8;
        }
        else if (distance > 31.0)
        {
            densityLevel[lane] = 4;
        }

        return densityLevel[lane];
    }

    // Moderate → High / Low
    if (currentLevel == 4)
    {
        if (distance <= 29.0)
        {
            densityLevel[lane] = 6;
        }
        else if (distance > 41.0)
        {
            densityLevel[lane] = 2;
        }

        return densityLevel[lane];
    }

    // Low → Moderate / Very Low
    if (currentLevel == 2)
    {
        if (distance <= 39.0)
        {
            densityLevel[lane] = 4;
        }
        else if (distance > 61.0)
        {
            densityLevel[lane] = 0;
        }

        return densityLevel[lane];
    }


    densityLevel[lane] =
        distanceToVehicles(distance);

    return densityLevel[lane];
}

void TrafficController::updateSensorData(
    float north,
    float east,
    float south,
    float west)
{
    // -------------------------------------------------
    // Raw sensor readings
    // -------------------------------------------------

    float rawDistance[4] =
    {
        north,
        east,
        south,
        west
    };

    // -------------------------------------------------
    // Process each lane
    // -------------------------------------------------

    for (int i = 0; i < 4; i++)
    {
        float calibrated =
            calibrateDistance(
                i,
                rawDistance[i]
            );

        int density =
            getStableDensity(
            i,
            calibrated
            );

        lanes[i].setVehicleCount(
            density
        );
    }

    // -------------------------------------------------
    // Serial Monitor
    // -------------------------------------------------

    Serial.println();

    Serial.println(
        "========== SENSOR UPDATE =========="
    );

    const char* names[4] =
    {
        "North",
        "East",
        "South",
        "West"
    };

    for (int i = 0; i < 4; i++)
    {
        Serial.print(
            names[i]
        );

        Serial.print(
            " | Raw = "
        );

        Serial.print(
            rawDistance[i],
            2
        );

        Serial.print(
            " cm | Filtered = "
        );

        if (filteredDistance[i] < 0)
        {
            Serial.print(
                "Out of Range"
            );
        }
        else
        {
            Serial.print(
                filteredDistance[i],
                2
            );

            Serial.print(
                " cm"
            );
        }

        Serial.print(
    " | Density = "
);

Serial.print(
    lanes[i].getVehicleCount()
);

Serial.print(
    " | Level = "
);

switch (densityLevel[i])
{
    case 0:
        Serial.println("VERY LOW");
        break;

    case 2:
        Serial.println("LOW");
        break;

    case 4:
        Serial.println("MODERATE");
        break;

    case 6:
        Serial.println("HIGH");
        break;

    case 8:
        Serial.println("VERY HIGH");
        break;

    case 10:
        Serial.println("CRITICAL");
        break;

    default:
        Serial.println("UNKNOWN");
        break;
}

        Serial.println(
            lanes[i].getVehicleCount()
        );
    }

    Serial.println(
        "==================================="
    );
    detectTrafficEvents();
}
 
// =====================================================
// SET EMERGENCY
// =====================================================

void TrafficController::setEmergencyLane(
    int lane)
{
    if (lane < 0 || lane >= 4)
    {
        Serial.println(
            "Invalid emergency lane"
        );

        return;
    }

    // -------------------------------------------------
    // Clear other emergency flags
    // -------------------------------------------------

    for (int i = 0; i < 4; i++)
    {
        emergency[i] = false;
    }

    emergency[lane] = true;

    Serial.println();

    Serial.println(
        "=================================="
    );

    Serial.println(
        " EMERGENCY VEHICLE DETECTED"
    );

    Serial.println(
        "=================================="
    );

    Serial.print(
        "Emergency Lane : "
    );

    switch (lane)
    {
        case 0:
            Serial.println("North");
            break;

        case 1:
            Serial.println("East");
            break;

        case 2:
            Serial.println("South");
            break;

        case 3:
            Serial.println("West");
            break;
    }

    Serial.println(
        "=================================="
    );
}

// =====================================================
// CLEAR ONE EMERGENCY
// =====================================================

void TrafficController::clearEmergencyLane(
    int lane)
{
    if (lane < 0 || lane >= 4)
    {
        return;
    }

    emergency[lane] = false;

    Serial.print(
        "Emergency cleared for lane : "
    );

    switch (lane)
    {
        case 0:
            Serial.println("North");
            break;

        case 1:
            Serial.println("East");
            break;

        case 2:
            Serial.println("South");
            break;

        case 3:
            Serial.println("West");
            break;
    }
}

// =====================================================
// CLEAR ALL EMERGENCY
// =====================================================

void TrafficController::clearAllEmergency()
{
    for (int i = 0; i < 4; i++)
    {
        emergency[i] = false;
    }
}

// =====================================================
// CHECK EMERGENCY
// =====================================================

bool TrafficController::isEmergencyLane(
    int lane)
{
    if (lane < 0 || lane >= 4)
    {
        return false;
    }

    return emergency[lane];
}

void TrafficController::calculateEventSeverity()
{
    int highestDensity = 0;
    int blockedLanes = 0;

    for (int i = 0; i < 4; i++)
    {
        int density = lanes[i].getVehicleCount();

        if (density > highestDensity)
        {
            highestDensity = density;
        }

        if (density >= 8)
        {
            blockedLanes++;
        }
    }

    switch (currentEvent)
    {
        case EVENT_NONE:
            currentSeverity = SEVERITY_NORMAL;
            break;

        case EVENT_CONGESTION:
            if (highestDensity >= 10)
            {
                currentSeverity = SEVERITY_HIGH;
            }
            else if (highestDensity >= 8)
            {
                currentSeverity = SEVERITY_MEDIUM;
            }
            else
            {
                currentSeverity = SEVERITY_LOW;
            }
            break;

        case EVENT_SUDDEN_TRAFFIC:
            if (highestDensity >= 10)
            {
                currentSeverity = SEVERITY_HIGH;
            }
            else
            {
                currentSeverity = SEVERITY_MEDIUM;
            }
            break;

        case EVENT_LANE_BLOCKED:
            if (blockedLanes >= 3)
            {
                currentSeverity = SEVERITY_CRITICAL;
            }
            else
            {
                currentSeverity = SEVERITY_HIGH;
            }
            break;

        case EVENT_ABNORMAL:
            currentSeverity = SEVERITY_HIGH;
            break;

        default:
            currentSeverity = SEVERITY_NORMAL;
            break;
    }
}
String TrafficController::getSeverityName()
{
    switch (currentSeverity)
    {
        case SEVERITY_NORMAL:
            return "NORMAL";

        case SEVERITY_LOW:
            return "LOW";

        case SEVERITY_MEDIUM:
            return "MEDIUM";

        case SEVERITY_HIGH:
            return "HIGH";

        case SEVERITY_CRITICAL:
            return "CRITICAL";

        default:
            return "NORMAL";
    }
}
String TrafficController::getEventName()
{
    switch (currentEvent)
    {
        case EVENT_CONGESTION:
            return "CONGESTION";

        case EVENT_SUDDEN_TRAFFIC:
            return "SUDDEN_TRAFFIC";

        case EVENT_LANE_BLOCKED:
            return "LANE_BLOCKED";

        case EVENT_ABNORMAL:
            return "ABNORMAL";

        case EVENT_NONE:
        default:
            return "NONE";
    }
}
String TrafficController::getTrafficEvent()
{
    return getEventName();
}


void TrafficController::detectTrafficEvents()
{
    TrafficEventType detectedEvent = EVENT_NONE;

    int highestDensity = 0;
    int blockedLanes = 0;

    // -------------------------------------------------
    // Analyze all lanes
    // -------------------------------------------------

    for (int i = 0; i < 4; i++)
    {
        int density =
            lanes[i].getVehicleCount();

        if (density > highestDensity)
        {
            highestDensity = density;
        }

        if (density >= 8)
        {
            blockedLanes++;
        }
    }

    // -------------------------------------------------
    // 1. Lane blocked has highest priority
    // -------------------------------------------------

    if (blockedLanes >= 2)
    {
        detectedEvent = EVENT_LANE_BLOCKED;
    }

    // -------------------------------------------------
    // 2. Sudden traffic
    // -------------------------------------------------

    else if (
        highestDensity -
        previousHighestDensity >= 4
    )
    {
        detectedEvent = EVENT_SUDDEN_TRAFFIC;

        densityChangeTime = millis();

        Serial.println();
        Serial.println(
            "========== SUDDEN TRAFFIC =========="
        );

        Serial.print(
            "Previous Density : "
        );

        Serial.println(
            previousHighestDensity
        );

        Serial.print(
            "Current Density : "
        );

        Serial.println(
            highestDensity
        );

        Serial.println(
            "===================================="
        );
    }

    // -------------------------------------------------
    // 3. Severe congestion
    // -------------------------------------------------

    else if (highestDensity >= 8)
    {
        detectedEvent = EVENT_CONGESTION;
    }

    // -------------------------------------------------
    // Update previous density
    // -------------------------------------------------

    previousHighestDensity =
        highestDensity;

if (detectedEvent != currentEvent)
{
    // New event detected
    if (detectedEvent != pendingEvent)
    {
        pendingEvent = detectedEvent;
        pendingEventStartTime = millis();

        Serial.println();
        Serial.println("========== PENDING TRAFFIC EVENT ==========");
        Serial.print("Event : ");

        switch (pendingEvent)
        {
            case EVENT_CONGESTION:
                Serial.println("CONGESTION");
                break;

            case EVENT_SUDDEN_TRAFFIC:
                Serial.println("SUDDEN_TRAFFIC");
                break;

            case EVENT_LANE_BLOCKED:
                Serial.println("LANE_BLOCKED");
                break;

            case EVENT_ABNORMAL:
                Serial.println("ABNORMAL");
                break;

            case EVENT_NONE:
            default:
                Serial.println("NONE");
                break;
        }

        Serial.println("Confirmation Time : 5 seconds");
        Serial.println("===========================================");
    }

    // Confirm event only after remaining stable
    if (millis() - pendingEventStartTime >= EVENT_CONFIRMATION_TIME)
    {
        currentEvent = pendingEvent;
        eventStartTime = millis();

        Serial.println();
        Serial.println("========== TRAFFIC EVENT CONFIRMED ==========");
        Serial.print("Event : ");
        Serial.println(getEventName());
        Serial.print("Highest Density : ");
        Serial.println(highestDensity);
        Serial.print("Affected Lanes : ");
        Serial.println(blockedLanes);
        Serial.println("=============================================");
    }
}
else
{
    // Event is already stable
    pendingEvent = currentEvent;
    pendingEventStartTime = millis();
}   
calculateEventSeverity();

Serial.print("Severity : ");
Serial.println(getSeverityName());
}