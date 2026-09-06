# Setup Guide

## 1. Overview

This guide explains how to set up and run the IoT-Based Adaptive Smart Traffic Management System.

The system consists of:

- Sensor ESP32
- Master ESP32
- Four HC-SR04 ultrasonic sensors
- Four traffic-light modules
- Flask backend
- SQLite database
- Web dashboard

The setup process involves configuring the firmware, connecting the hardware, installing the backend dependencies, starting the Flask server, and opening the dashboard.

---

## 2. Project Structure

The main project structure is:

```text
Smart-Traffic-Management-System/
│
├── backend/
│   ├── api/
│   ├── database/
│   ├── models/
│   ├── routes/
│   ├── services/
│   └── app.py
│
├── dashboard/
│   ├── index.html
│   ├── app.js
│   └── style.css
│
├── diagrams/
│
├── docs/
│   ├── API.md
│   ├── Architecture.md
│   ├── Setup.md
│   └── UserGuide.md
│
├── firmware/
│   ├── sensor_node/
│   ├── traffic_controller/
│   └── Traffic_Light_Test/
│
├── .gitignore
└── README.md
```

---

## 3. Requirements

### Hardware Requirements

| Component | Quantity |
|---|---:|
| ESP32 Development Board | 2 |
| HC-SR04 Ultrasonic Sensor | 4 |
| 4-Way Traffic Light Module | 4 |
| Breadboard | As required |
| Jumper Wires | As required |
| USB Cable | As required |
| Power Supply | As required |

### Software Requirements

The following software is required:

- Arduino IDE
- Python 3
- Git
- Web browser
- Visual Studio Code or another code editor

---

# 4. Configure the ESP32 Firmware

The project uses two ESP32 boards.

```text
ESP32 #1 → Sensor ESP32
ESP32 #2 → Master ESP32
```

### Sensor ESP32

The Sensor ESP32 reads the four HC-SR04 sensors.

The current sensor configuration is:

```cpp
Sensor sensors[] =
{
    {"North", 13, 12},
    {"East", 14, 27},
    {"South", 26, 25},
    {"West", 33, 32}
};
```

The values represent the trigger and echo pins used by each ultrasonic sensor.

### Master ESP32

The Master ESP32 controls the four traffic-light modules.

The current traffic-light GPIO mapping is:

| Lane | Red | Yellow | Green |
|---|---:|---:|---:|
| North | 25 | 26 | 27 |
| East | 18 | 19 | 21 |
| South | 22 | 23 | 5 |
| West | 13 | 14 | 32 |

Before uploading the firmware, verify that the GPIO configuration matches the actual hardware connections.

---

# 5. Configure Wi-Fi

Both ESP32 boards must be connected to the same network used by the computer running the dashboard and Flask backend.

Configure the Wi-Fi credentials in the ESP32 firmware according to the project's configuration.

```text
Wi-Fi SSID
     ↓
Wi-Fi Password
     ↓
ESP32 Connection
     ↓
Local Network
```

After uploading the firmware, open the Arduino Serial Monitor and verify that the ESP32 successfully connects to Wi-Fi.

---

# 6. Upload Sensor ESP32 Firmware

1. Connect the Sensor ESP32 to the computer using USB.
2. Open the Sensor ESP32 firmware in Arduino IDE.
3. Select the correct ESP32 board.
4. Select the correct COM port.
5. Compile the firmware.
6. Upload the firmware.
7. Open the Serial Monitor.
8. Verify sensor measurements are being produced.

The Sensor ESP32 should provide distance measurements corresponding to:

- North
- East
- South
- West

---

# 7. Upload Master ESP32 Firmware

1. Disconnect the Sensor ESP32 if necessary.
2. Connect the Master ESP32 to the computer.
3. Open the traffic-controller firmware in Arduino IDE.
4. Select the correct ESP32 board.
5. Select the correct COM port.
6. Compile the firmware.
7. Upload the firmware.
8. Open the Serial Monitor.
9. Verify that the controller starts correctly.
10. Verify Wi-Fi connectivity.

The Master ESP32 should start the traffic-control system and expose the HTTP API.

---

# 8. Connect the Hardware

Connect the four HC-SR04 sensors to the Sensor ESP32 according to the configured GPIO pins.

```text
North HC-SR04 → Sensor ESP32
East HC-SR04  → Sensor ESP32
South HC-SR04 → Sensor ESP32
West HC-SR04  → Sensor ESP32
```

Connect the four traffic-light modules to the Master ESP32 according to the traffic-light GPIO configuration.

```text
North Traffic Light → Master ESP32
East Traffic Light  → Master ESP32
South Traffic Light → Master ESP32
West Traffic Light  → Master ESP32
```

### Important

The HC-SR04 Echo output should be electrically compatible with the ESP32 GPIO input.

Use an appropriate voltage-level solution where required.

---

# 9. Verify Sensor Communication

After both ESP32 boards are running, verify that the Sensor ESP32 is providing traffic information to the Master ESP32.

Example sensor data:

```json
{
    "north": 55.0,
    "east": 31.1,
    "south": 59.1,
    "west": -1.0
}
```

The Master ESP32 should use this information to update the traffic condition of each lane.

---

# 10. Verify the Master ESP32 API

Find the IP address assigned to the Master ESP32 from the Serial Monitor or network information.

Open the following endpoint in a browser:

```text
http://<MASTER_ESP32_IP>/status
```

For example:

```text
http://192.168.x.x/status
```

A successful response should contain JSON traffic information.

Example:

```json
{
    "project": "SmartTrafficAI",
    "currentLane": "North",
    "signalState": "GREEN",
    "trafficEvent": "NONE",
    "trafficSeverity": "NORMAL",
    "lanes": []
}
```

The exact values depend on the current traffic and sensor conditions.

---

# 11. Install Backend Dependencies

Open a terminal in the project directory:

```powershell
cd "Smart-Traffic-Management-System"
```

Create a Python virtual environment if required:

```powershell
python -m venv venv
```

Activate the virtual environment:

```powershell
venv\Scripts\activate
```

Install Flask:

```powershell
pip install flask
```

Install Flask-CORS:

```powershell
pip install flask-cors
```

The backend uses SQLite, which is included with standard Python installations.

If the project contains a `requirements.txt` file, dependencies can instead be installed using:

```powershell
pip install -r requirements.txt
```

---

# 12. Initialize the SQLite Database

The Flask backend initializes the database when the application starts.

The database is stored under:

```text
backend/database/traffic_logs.db
```

The main database table is:

```text
traffic_logs
```

The database contains historical traffic information such as:

- Timestamp
- Lane vehicle counts
- Current lane
- Signal state
- Traffic event
- Traffic severity

The database file is excluded from Git tracking.

---

# 13. Start the Flask Backend

Open a terminal in the project directory.

Activate the Python virtual environment if one is being used:

```powershell
venv\Scripts\activate
```

Start the Flask application:

```powershell
python backend/app.py
```

The backend is configured to run on:

```text
0.0.0.0:5000
```

The backend can then be accessed locally using:

```text
http://localhost:5000
```

---

# 14. Verify the Flask Backend

Verify that the backend is running before starting dashboard analytics.

The traffic logging endpoint is:

```text
POST /api/traffic/log
```

The analytics endpoint is:

```text
GET /api/traffic/analytics
```

The complete backend URL format is:

```text
http://<COMPUTER_IP>:5000/api/traffic/analytics
```

For a local test on the same computer:

```text
http://localhost:5000/api/traffic/analytics
```

---

# 15. Configure the Dashboard

Open:

```text
dashboard/app.js
```

The dashboard requires the correct network addresses for:

- Master ESP32
- Flask backend

The configuration follows this pattern:

```javascript
const url = "http://<MASTER_ESP32_IP>/status";

const loggingUrl =
    "http://<COMPUTER_IP>:5000/api/traffic/log";

const emergencyUrl =
    "http://<MASTER_ESP32_IP>/emergency";

const clearEmergencyUrl =
    "http://<MASTER_ESP32_IP>/clear-emergency";
```

Replace the placeholder addresses with the actual IP addresses used on the local network.

The computer running the dashboard and Flask backend must be able to communicate with the Master ESP32.

---

# 16. Start the Dashboard

The dashboard is located in:

```text
dashboard/
├── index.html
├── app.js
└── style.css
```

Open `index.html` using a web browser or serve the dashboard through a local web server.

For example, from the project directory:

```powershell
cd dashboard
```

If Python is installed, a simple local server can be started using:

```powershell
python -m http.server 8080
```

Then open:

```text
http://localhost:8080
```

The dashboard should load the SmartTrafficAI interface.

---

# 17. Verify Real-Time Dashboard Communication

After opening the dashboard, verify the following:

- Current lane is displayed.
- Signal state is displayed.
- Traffic event is displayed.
- Traffic severity is displayed.
- Four lane cards are displayed.
- Vehicle information is updated.
- Waiting information is updated.
- Priority values are updated.
- System information is displayed.

The dashboard periodically requests the Master ESP32 `/status` endpoint.

```text
Master ESP32
      ↓
GET /status
      ↓
Dashboard JavaScript
      ↓
Update Interface
```

---

# 18. Test Adaptive Traffic Control

Observe the four traffic-light modules.

The normal signal sequence is:

```text
Pre-Green Yellow
        ↓
Green
        ↓
Yellow
        ↓
All Red
        ↓
Next Lane
```

Verify that:

- Only the selected lane receives Green.
- Yellow transition occurs before Green.
- Yellow occurs after Green.
- All-Red safety interval occurs before the next lane.
- The next lane is selected based on priority.

---

# 19. Test Adaptive Green Timing

Generate different traffic conditions using the sensors.

The current base Green timing is:

| Traffic Condition | Green Time |
|---|---:|
| 0 vehicles | 10 seconds |
| 2 vehicles | 15 seconds |
| 4 vehicles | 20 seconds |
| 6 vehicles | 25 seconds |
| 8 vehicles | 30 seconds |
| 10+ vehicles | 35 seconds |

Waiting-time bonuses are:

| Waiting Time | Bonus |
|---|---:|
| Less than 10 seconds | 0 seconds |
| 10 seconds or more | +3 seconds |
| 20 seconds or more | +5 seconds |

The maximum Green time is:

```text
40 seconds
```

Verify that higher traffic demand results in longer Green duration within the configured limits.

---

# 20. Test Emergency Priority

Open the **Emergency** section of the dashboard.

Select one of the four lanes:

```text
North
East
South
West
```

The dashboard sends:

```http
POST /emergency
```

with a JSON body such as:

```json
{
    "lane": 0
}
```

Verify that:

- Emergency status becomes active.
- The selected lane is displayed as the priority lane.
- The selected Live Traffic lane is highlighted.
- The `EMERGENCY` badge appears.
- The traffic controller gives the selected lane the highest priority.

---

# 21. Clear Emergency Priority

Click:

```text
Clear Emergency
```

The dashboard sends:

```http
POST /clear-emergency
```

Verify that:

- Emergency status changes to inactive.
- Priority lane changes to `NONE`.
- The emergency dashboard indicator returns to normal.
- The lane emergency highlight disappears.
- The `EMERGENCY` badge disappears.
- Normal traffic scheduling resumes.

---

# 22. Test Traffic Events

Generate different traffic conditions using the available sensors.

The system supports:

```text
NONE
CONGESTION
SUDDEN_TRAFFIC
LANE_BLOCKED
ABNORMAL
```

The event must remain stable for approximately five seconds before being treated as a confirmed event.

Verify that the dashboard displays:

- Current event
- Event severity
- Event history
- Event statistics

---

# 23. Verify Traffic Logging

The dashboard periodically sends traffic status information to:

```text
POST /api/traffic/log
```

Verify that the Flask backend receives the request successfully.

The data should be stored in:

```text
backend/database/traffic_logs.db
```

The stored information should include:

- Timestamp
- North traffic
- East traffic
- South traffic
- West traffic
- Current lane
- Signal state
- Traffic event
- Traffic severity

---

# 24. Verify Analytics

Open the **Analytics** section of the dashboard.

Verify that the system displays:

- Today traffic statistics
- Weekly traffic statistics
- Monthly traffic statistics
- Lane-based analysis
- Traffic trends
- Event statistics

The analytics data is obtained from:

```text
GET /api/traffic/analytics
```

The Flask backend retrieves and aggregates information from the SQLite database.

---

# 25. Verify Event History

Open the **Events** section.

Verify that recent traffic events are displayed with information such as:

- Timestamp
- Event type
- Severity

The dashboard also displays event statistics using charts.

---

# 26. Complete System Test

Perform the following sequence for a complete test:

```text
1. Power the Sensor ESP32
        ↓
2. Power the Master ESP32
        ↓
3. Verify Wi-Fi connection
        ↓
4. Verify sensor measurements
        ↓
5. Verify Master ESP32 /status
        ↓
6. Start Flask backend
        ↓
7. Start dashboard
        ↓
8. Verify real-time traffic
        ↓
9. Test adaptive Green timing
        ↓
10. Test traffic events
        ↓
11. Test emergency priority
        ↓
12. Clear emergency
        ↓
13. Verify traffic logging
        ↓
14. Verify analytics
```

---

# 27. Troubleshooting

## Dashboard Does Not Load

Check:

- `dashboard/index.html` exists.
- `dashboard/app.js` exists.
- `dashboard/style.css` exists.
- The browser is connected to the correct local server.
- The browser console does not show JavaScript errors.

---

## Dashboard Shows No ESP32 Data

Check:

- Master ESP32 is powered.
- Master ESP32 is connected to Wi-Fi.
- The configured Master ESP32 IP address is correct.
- The `/status` endpoint responds correctly.
- The computer and ESP32 are on the same network.

Test:

```text
http://<MASTER_ESP32_IP>/status
```

---

## Flask Backend Does Not Start

Check:

- Python is installed.
- The virtual environment is activated if used.
- Flask is installed.
- Flask-CORS is installed.
- The command is executed from the project directory.

Try:

```powershell
python backend/app.py
```

---

## Analytics Are Empty

Check:

- Flask backend is running.
- Traffic logging requests are reaching the backend.
- `traffic_logs.db` has been created.
- Traffic records are being inserted.
- The dashboard can access `/api/traffic/analytics`.

---

## Emergency Button Does Not Work

Check:

- Master ESP32 is reachable.
- `/emergency` is available.
- The selected lane index is between `0` and `3`.
- Browser console does not report a network or CORS error.
- The dashboard is using the correct Master ESP32 IP address.

---

## Emergency Status Is Not Updating

Check:

- The Master ESP32 `/status` response contains lane emergency information.
- The dashboard is receiving the latest status response.
- The selected lane has the expected emergency state.
- Browser console does not report JavaScript errors.

---

## Traffic Events Are Not Detected

Check:

- Sensors are producing valid measurements.
- Traffic density values are changing as expected.
- The event condition remains stable long enough for confirmation.
- The Master ESP32 is receiving sensor data correctly.

---

# 28. Network Configuration Summary

The complete network communication can be summarized as:

```text
                 Local Network
                      │
        ┌─────────────┴─────────────┐
        │                           │
        ↓                           ↓
 Sensor ESP32                 Master ESP32
        │                           │
        │                           ├── /status
        │                           ├── /emergency
        │                           └── /clear-emergency
        │                           │
        └───────────────┬───────────┘
                        │
                        ↓
                    Computer
                        │
              ┌─────────┴─────────┐
              ↓                   ↓
        Flask Backend          Dashboard
              │                   │
              ↓                   │
          SQLite DB              │
              └───────────────────┘
```

---

# 29. Final Verification Checklist

Before demonstrating the project, verify the following:

### Hardware

- [ ] Both ESP32 boards power on.
- [ ] Four ultrasonic sensors provide measurements.
- [ ] Four traffic-light modules operate correctly.
- [ ] Hardware connections match the configured GPIO pins.

### Firmware

- [ ] Sensor ESP32 connects to Wi-Fi.
- [ ] Master ESP32 connects to Wi-Fi.
- [ ] Sensor data reaches the Master ESP32.
- [ ] `/status` returns valid JSON.
- [ ] Traffic signals follow the FSM sequence.
- [ ] Adaptive Green timing works.
- [ ] Traffic events are detected.
- [ ] Emergency priority works.

### Backend

- [ ] Flask backend starts successfully.
- [ ] SQLite database is created.
- [ ] Traffic logs are stored.
- [ ] Analytics endpoint returns data.

### Dashboard

- [ ] Dashboard loads correctly.
- [ ] Live Traffic updates.
- [ ] Analytics display correctly.
- [ ] Events display correctly.
- [ ] Emergency controls work.
- [ ] Emergency status synchronizes automatically.
- [ ] System information is displayed.

---

# 30. Setup Complete

After completing the above steps, the complete Smart Traffic Management System should operate as follows:

```text
Sensors
   ↓
Sensor ESP32
   ↓
Master ESP32
   ↓
Traffic Processing
   ↓
Adaptive Signal Control
   ↓
Traffic Lights
   │
   ├──────────────→ Dashboard
   │
   └──────────────→ Traffic Logging
                          ↓
                    Flask Backend
                          ↓
                       SQLite
                          ↓
                      Analytics
```

The system is then ready for functional testing, demonstration, documentation, and further development.