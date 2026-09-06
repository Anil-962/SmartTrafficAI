# SmartTrafficAI

## IoT-Based Adaptive Smart Traffic Management System with Emergency Vehicle Priority

SmartTrafficAI is an IoT-based intelligent traffic management system designed to monitor traffic density, dynamically control traffic signals, detect traffic events, provide emergency vehicle priority, and maintain historical traffic data for analytics.

The system combines ESP32-based traffic control, ultrasonic vehicle detection, adaptive signal timing, emergency signal override, a Flask backend, SQLite database, and a web-based monitoring dashboard.

---

## Key Features

### 1. Adaptive Traffic Signal Control

The system dynamically determines green-light duration based on traffic density and lane waiting time.

- Low traffic → shorter green duration
- Higher traffic → longer green duration
- Waiting lanes receive additional priority
- Green time is bounded by configured minimum and maximum limits

The implemented adaptive timing supports approximately:

| Traffic Density | Base Green Time |
|---|---:|
| 0 vehicles | 10 seconds |
| 2 vehicles | 15 seconds |
| 4 vehicles | 20 seconds |
| 6 vehicles | 25 seconds |
| 8 vehicles | 30 seconds |
| 10+ vehicles | 35 seconds |

Additional waiting-time bonuses are applied to reduce starvation of less busy lanes.

---

### 2. IoT-Based Traffic Detection

Four HC-SR04 ultrasonic sensors monitor the four traffic lanes:

- North
- East
- South
- West

A dedicated Sensor ESP32 collects distance measurements and sends traffic information to the Master ESP32.

---

### 3. Traffic Event Detection

The system identifies abnormal traffic conditions using measured traffic-density information.

Supported event types include:

- `NONE`
- `CONGESTION`
- `SUDDEN_TRAFFIC`
- `LANE_BLOCKED`
- `ABNORMAL`

Events use stability confirmation to reduce false detections caused by short-term sensor fluctuations.

---

### 4. Traffic Event Severity

Detected traffic events are classified into severity levels:

- `NORMAL`
- `LOW`
- `MEDIUM`
- `HIGH`
- `CRITICAL`

This allows the dashboard to distinguish normal traffic conditions from increasingly serious traffic situations.

---

### 5. Emergency Vehicle Priority

The system provides manual emergency vehicle priority through the dashboard.

An operator can select:

- North
- East
- South
- West

The selected lane receives emergency priority through the Master ESP32.

The dashboard provides:

- Emergency activation
- Active emergency status
- Priority lane display
- Emergency lane highlighting
- Emergency badge
- Emergency clear operation
- Automatic emergency-status synchronization

---

### 6. Real-Time Web Dashboard

The dashboard provides a centralized interface for monitoring the traffic system.

Main sections include:

- Dashboard
- Live Traffic
- Analytics
- Events
- Emergency
- System

The interface provides real-time information including:

- Current traffic lane
- Current signal state
- Traffic event
- Traffic severity
- Vehicle count
- Waiting time
- Lane priority
- Emergency status

---

### 7. Historical Traffic Logging

Traffic status is periodically recorded by the dashboard through the Flask backend.

The backend stores traffic information in SQLite.

Recorded information includes:

- Timestamp
- North vehicle count
- East vehicle count
- South vehicle count
- West vehicle count
- Current lane
- Signal state
- Traffic event
- Traffic severity

The SQLite database is automatically initialized when the Flask backend starts.

---

### 8. Traffic Analytics

Historical traffic data is processed by the Flask backend and displayed on the dashboard.

The Analytics section provides:

- Daily statistics
- Weekly statistics
- Monthly statistics
- Average traffic values
- Lane-level analysis
- Traffic trend charts
- Event statistics
- Event severity statistics
- Event history

---

## System Architecture

```text
                    ┌──────────────────────┐
                    │   Ultrasonic Sensors │
                    │       HC-SR04 × 4    │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │     Sensor ESP32     │
                    │ Traffic Measurement  │
                    └──────────┬───────────┘
                               │
                         HTTP / JSON
                               │
                               ▼
                    ┌──────────────────────┐
                    │    Master ESP32      │
                    │  Traffic Controller  │
                    └──────────┬───────────┘
                               │
              ┌────────────────┼────────────────┐
              │                │                │
              ▼                ▼                ▼
       Traffic Signals    Event Detection   Emergency
       Adaptive Control    & Severity        Priority
              │
              ▼
       ┌──────────────────────┐
       │    Web Dashboard     │
       │    HTML / CSS / JS   │
       └──────────┬───────────┘
                  │
               REST API
                  │
                  ▼
       ┌──────────────────────┐
       │    Flask Backend     │
       │      Port 5000       │
       └──────────┬───────────┘
                  │
                  ▼
       ┌──────────────────────┐
       │       SQLite         │
       │    Traffic Logs      │
       └──────────────────────┘
## Hardware Components

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 Development Board | 2 | One ESP32 is used as the Master Traffic Controller and the second ESP32 is used as the Sensor Node |
| HC-SR04 Ultrasonic Sensor | 4 | Detects vehicle presence and estimates traffic density for the four lanes |
| 4-Way Traffic Light Module | 4 | Controls the Red, Yellow, and Green signals for North, East, South, and West lanes |
| Breadboard | 1 or more | Used for prototyping and connecting the electronic components |
| Jumper Wires | As required | Used for electrical connections between ESP32, sensors, and traffic light modules |
| External Power Supply / USB Power | As required | Provides power to the ESP32 boards and connected components |

### Hardware Architecture

The hardware consists of two ESP32 boards.

- **Sensor ESP32:** Collects distance measurements from four HC-SR04 ultrasonic sensors and sends the sensor data to the Master ESP32.
- **Master ESP32:** Processes traffic information, calculates lane priority, controls the four traffic-light modules, handles emergency priority, and provides the HTTP interface used by the dashboard.

The four traffic lanes are:

- North
- East
- South
- West

## Software Stack

### Embedded System

| Technology | Purpose |
|---|---|
| Arduino IDE | Development, compilation, and uploading of firmware to the ESP32 boards |
| C/C++ | Programming language used for the ESP32 firmware |
| ESP32 Wi-Fi | Provides wireless communication between the traffic-control system and the network |
| HTTP | Used for communication between the ESP32 and dashboard/sensor components |
| JSON | Used to exchange structured traffic and sensor data |

### Backend

| Technology | Purpose |
|---|---|
| Python | Backend programming language |
| Flask | Provides the REST API and backend web server |
| Flask-CORS | Enables cross-origin communication between the dashboard and backend |
| SQLite | Stores historical traffic logs and event information |
| REST API | Provides communication between the dashboard and backend services |

### Dashboard

| Technology | Purpose |
|---|---|
| HTML5 | Defines the dashboard structure and user interface |
| CSS3 | Provides the dashboard layout, styling, responsive design, and visual effects |
| JavaScript | Handles real-time data updates, API communication, navigation, and dashboard logic |
| Chart.js | Displays traffic analytics and event data using charts |

### Development and Version Control

| Tool | Purpose |
|---|---|
| Git | Source-code version control |
| GitHub | Remote repository and project version management |
| Visual Studio Code | Source-code editing and project development |
| Arduino IDE | ESP32 firmware development and deployment |
## Project Structure

```text
Smart-Traffic-Management-System/
│
├── backend/
│   ├── api/
│   ├── database/
│   │   ├── db.py
│   │   └── traffic_logs.db
│   ├── models/
│   │   └── traffic_log.py
│   ├── routes/
│   │   └── analytics.py
│   ├── services/
│   │   └── traffic_logger.py
│   └── app.py
│
├── dashboard/
│   ├── index.html
│   ├── app.js
│   └── style.css
│
├── firmware/
│   ├── sensor_node/
│   ├── traffic_controller/
│   └── Traffic_Light_Test/
│
├── diagrams/
│
├── docs/
│   ├── API.md
│   ├── Architecture.md
│   ├── Setup.md
│   └── UserGuide.md
│
├── .gitignore
└── README.md

## Traffic Signal Control

The Master ESP32 controls four traffic lanes:

- North
- East
- South
- West

Each lane has an independent traffic-light module with Red, Yellow, and Green signals.

The traffic controller uses a **Finite State Machine (FSM)** to manage signal transitions safely and predictably.

### Signal Control States

| State | Description |
|---|---|
| `PRE_GREEN_YELLOW_STATE` | Activates the selected lane's Yellow signal before Green |
| `GREEN_STATE` | Allows traffic to move through the selected lane |
| `YELLOW_STATE` | Changes the selected lane from Green to Yellow before stopping |
| `ALL_RED_STATE` | Temporarily keeps all lanes Red before the next lane becomes active |

### Normal Signal Sequence

The normal traffic-control sequence is:

```text
Select Highest-Priority Lane
        ↓
Pre-Green Yellow
        ↓
Green
        ↓
Yellow
        ↓
All Red
        ↓
Select Next Highest-Priority Lane
        ↓
Repeat

## Adaptive Traffic Management

The system dynamically adjusts traffic signal timing based on the traffic conditions detected in each lane.

The four ultrasonic sensors continuously provide distance measurements to the Sensor ESP32. The Sensor ESP32 sends the processed sensor data to the Master ESP32, which uses the information to estimate traffic density and calculate lane priority.

### Adaptive Traffic Flow

```text
HC-SR04 Sensors
       ↓
Sensor ESP32
       ↓
Distance Measurements
       ↓
Traffic Density Calculation
       ↓
Lane Priority Calculation
       ↓
Master ESP32
       ↓
Adaptive Green Time
       ↓
Traffic Signal Control

## Sensor Integration

The system uses a dedicated Sensor ESP32 to collect traffic information from four HC-SR04 ultrasonic sensors.

Each sensor corresponds to one traffic lane:

| Lane | Sensor |
|---|---|
| North | HC-SR04 |
| East | HC-SR04 |
| South | HC-SR04 |
| West | HC-SR04 |

### Sensor ESP32

The Sensor ESP32 periodically measures the distance between each ultrasonic sensor and the detected vehicle.

The measured values are associated with their respective lanes and transmitted to the Master ESP32 using JSON data.

Example sensor data:

```json
{
    "north": 55.0,
    "east": 31.1,
    "south": 59.1,
    "west": -1.0
}

## Traffic Event Detection

The system monitors traffic conditions continuously and identifies abnormal traffic patterns using the available sensor data.

The current implementation supports the following traffic event types:

| Event | Description |
|---|---|
| `NONE` | Normal traffic conditions |
| `CONGESTION` | High traffic density is detected |
| `SUDDEN_TRAFFIC` | A significant change in traffic density is detected |
| `LANE_BLOCKED` | A lane indicates conditions consistent with a blockage |
| `ABNORMAL` | Traffic conditions do not match the normal operating pattern |

### Event Detection Flow

```text
Sensor Data
     ↓
Traffic Density
     ↓
Compare Current and Previous Conditions
     ↓
Traffic Event Detection
     ↓
Event Stability Check
     ↓
Confirmed Traffic Event
     ↓
Severity Calculation
     ↓
Dashboard + Database

## Emergency Vehicle Priority

The system provides an emergency vehicle priority mechanism that allows a selected traffic lane to receive the highest scheduling priority.

When emergency priority is activated for a lane, the Master ESP32 updates the lane's emergency status and gives that lane priority over normal traffic during signal scheduling.

### Emergency Priority Flow

```text
Emergency Vehicle / Operator
          ↓
Emergency Lane Selection
          ↓
Master ESP32
          ↓
Emergency Priority Enabled
          ↓
Highest Lane Priority
          ↓
Traffic Signal Override
          ↓
Emergency Lane Gets Green

## Data Logging and Database

The system stores historical traffic information using a Flask backend and SQLite database.

Traffic data is collected from the Master ESP32 and periodically sent to the backend through a REST API.

### Data Flow

```text
Master ESP32
     ↓
Traffic Status JSON
     ↓
Dashboard
     ↓
Flask REST API
     ↓
Traffic Logger Service
     ↓
SQLite Database

## Traffic Analytics

The dashboard provides historical traffic analytics using the data stored in the SQLite database.

The Flask backend processes the stored traffic logs and provides aggregated information to the dashboard through REST API endpoints.

### Analytics Overview

The dashboard provides traffic statistics for different time periods:

| Period | Information |
|---|---|
| Today | Total vehicle count, average traffic, and number of logs |
| This Week | Weekly vehicle count, average traffic, and number of logs |
| This Month | Monthly vehicle count, average traffic, and number of logs |
| Lane Analysis | Average traffic for North, East, South, and West lanes |

### Analytics Data Flow

```text
SQLite Traffic Logs
        ↓
Flask Analytics API
        ↓
Traffic Aggregation
        ↓
Dashboard
        ↓
Charts and Statistics

## Web Dashboard

The system provides a web-based dashboard for monitoring and managing the smart traffic controller.

The dashboard is designed as a single-page interface where different functional sections can be accessed through the header navigation without reloading the page.

### Dashboard Sections

| Section | Purpose |
|---|---|
| Dashboard | Provides an overview of the current traffic-control status |
| Live Traffic | Displays real-time traffic information for all four lanes |
| Analytics | Displays historical traffic statistics and charts |
| Events | Displays detected traffic events, severity information, and event history |
| Emergency | Provides emergency vehicle priority controls |
| System | Displays system and connectivity information |

### Real-Time Monitoring

The dashboard periodically requests the current status from the Master ESP32.

The real-time information includes:

- Current active lane
- Current signal state
- Vehicle count for each lane
- Waiting information
- Lane priority
- Emergency status
- Current traffic event
- Traffic severity
- Wi-Fi status
- Signal-controller uptime

### Live Traffic Display

Each lane is represented using a dedicated traffic card.

```text
                LIVE TRAFFIC

        ┌───────────┐
        │   NORTH   │
        │ Vehicles  │
        │ Waiting   │
        │ Priority  │
        └───────────┘

┌───────────┐             ┌───────────┐
│   WEST    │             │   EAST    │
│ Vehicles  │             │ Vehicles  │
│ Waiting   │             │ Waiting   │
│ Priority  │             │ Priority  │
└───────────┘             └───────────┘

        ┌───────────┐
        │   SOUTH   │
        │ Vehicles  │
        │ Waiting   │
        │ Priority  │
        └───────────┘
## REST API

The system uses HTTP-based REST APIs for communication between the dashboard, Master ESP32, and Flask backend.

### Master ESP32 API

The Master ESP32 provides endpoints for real-time traffic monitoring and emergency control.

| Endpoint | Method | Purpose |
|---|---|---|
| `/status` | `GET` | Returns the current traffic-controller status |
| `/emergency` | `POST` | Activates emergency priority for a selected lane |
| `/clear-emergency` | `POST` | Clears emergency priority |

### `/status`

The dashboard periodically requests the `/status` endpoint to obtain the latest traffic information.

The response contains information including:

- Current lane
- Signal state
- Traffic event
- Traffic severity
- Wi-Fi status
- RSSI
- Uptime
- Vehicle count for each lane
- Waiting information
- Lane priority
- Emergency status

Example response structure:

```json
{
    "project": "SmartTrafficAI",
    "version": "3.1",
    "currentLane": "North",
    "signalState": "GREEN",
    "trafficEvent": "CONGESTION",
    "trafficSeverity": "MEDIUM",
    "lanes": [
        {
            "name": "North",
            "vehicles": 10,
            "waiting": 3,
            "priority": 23,
            "emergency": false
        }
    ]
}

## System Workflow

The complete system combines sensor data collection, traffic analysis, adaptive signal control, emergency priority, event detection, data logging, and dashboard monitoring.

### Overall Workflow

```text
                 ┌──────────────────────┐
                 │   HC-SR04 Sensors    │
                 │      (4 Lanes)       │
                 └──────────┬───────────┘
                            │
                            ↓
                 ┌──────────────────────┐
                 │    Sensor ESP32      │
                 │  Sensor Processing   │
                 └──────────┬───────────┘
                            │
                       JSON Data
                            │
                            ↓
                 ┌──────────────────────┐
                 │     Master ESP32     │
                 │ Traffic Controller   │
                 └──────────┬───────────┘
                            │
              ┌─────────────┼─────────────┐
              ↓             ↓             ↓
       Density Analysis  Event Detection  Emergency
              │             │             │
              └─────────────┼─────────────┘
                            ↓
                 ┌──────────────────────┐
                 │  Lane Priority       │
                 │  Calculation          │
                 └──────────┬───────────┘
                            ↓
                 ┌──────────────────────┐
                 │ Adaptive Signal      │
                 │ Scheduling            │
                 └──────────┬───────────┘
                            ↓
                 ┌──────────────────────┐
                 │ Traffic Light        │
                 │ Modules (4 Lanes)    │
                 └──────────────────────┘
                            │
                            │
             ┌──────────────┴──────────────┐
             ↓                             ↓
   ┌──────────────────────┐    ┌──────────────────────┐
   │   Web Dashboard      │    │   Flask Backend      │
   │ Real-Time Monitoring │    │ SQLite + Analytics   │
   └──────────────────────┘    └──────────────────────┘
## Installation and Setup

### Prerequisites

Install the following software before running the project:

| Software | Purpose |
|---|---|
| Arduino IDE | ESP32 firmware development and upload |
| Python 3 | Flask backend |
| Git | Source-code version control |
| Web Browser | Dashboard access |

The ESP32 boards must also be configured with the required Wi-Fi network credentials.

### 1. Clone the Repository

```bash
git clone https://github.com/Anil-962/SmartTrafficAI.git
cd SmartTrafficAI
### 2. Configure the ESP32 Firmware

The project uses two ESP32 boards:

- **Sensor ESP32:** Collects traffic information from four HC-SR04 ultrasonic sensors.
- **Master ESP32:** Processes traffic information, controls the traffic signals, handles emergency priority, and provides the HTTP interface for the dashboard.

Open the corresponding firmware projects in **Arduino IDE**.

#### Sensor ESP32

Open the Sensor Node firmware and configure the Wi-Fi credentials according to the local network.

The Sensor ESP32 is responsible for:

- Reading the four HC-SR04 ultrasonic sensors
- Processing distance measurements
- Estimating traffic conditions
- Sending sensor data to the Master ESP32

Upload the Sensor Node firmware to the Sensor ESP32.

#### Master ESP32

Open the Traffic Controller firmware in Arduino IDE.

The Master ESP32 is responsible for:

- Receiving sensor data
- Maintaining lane traffic information
- Calculating lane priority
- Performing adaptive signal scheduling
- Controlling the four traffic-light modules
- Detecting traffic events
- Handling emergency vehicle priority
- Providing HTTP endpoints for the dashboard

Upload the Traffic Controller firmware to the Master ESP32.

#### Wi-Fi Configuration

Configure the required Wi-Fi network credentials in the ESP32 firmware before uploading.

The Master ESP32 and Sensor ESP32 must be connected to the appropriate network for communication with each other and with the dashboard.

```text
Wi-Fi Network
     │
     ├───────────────┐
     ↓               ↓
Sensor ESP32     Master ESP32
     │               │
     │               ├── Traffic Signals
     │               └── Dashboard
     │
     └── Sensor Data

### 3. Connect the Hardware

Connect the hardware components according to the project architecture.

The system uses two ESP32 boards:

- **Sensor ESP32:** Connected to four HC-SR04 ultrasonic sensors.
- **Master ESP32:** Connected to four traffic-light modules.

#### Sensor ESP32 Connections

The four HC-SR04 sensors are assigned to the four traffic lanes.

| Lane | Trigger Pin | Echo Pin |
|---|---:|---:|
| North | GPIO 13 | GPIO 12 |
| East | GPIO 14 | GPIO 27 |
| South | GPIO 26 | GPIO 25 |
| West | GPIO 33 | GPIO 32 |

The Sensor ESP32 reads the distance measured by each sensor and associates the measurement with its corresponding traffic lane.

#### Master ESP32 Traffic-Light Connections

The Master ESP32 controls the four traffic-light modules.

| Lane | Red | Yellow | Green |
|---|---:|---:|---:|
| North | GPIO 25 | GPIO 26 | GPIO 27 |
| East | GPIO 18 | GPIO 19 | GPIO 21 |
| South | GPIO 22 | GPIO 23 | GPIO 5 |
| West | GPIO 13 | GPIO 14 | GPIO 32 |

#### Hardware Architecture

```text
                    ┌─────────────────┐
                    │  Sensor ESP32   │
                    └────────┬────────┘
                             │
              ┌──────────────┼──────────────┐
              ↓              ↓              ↓
          North Sensor   East Sensor   South Sensor
                             │
                         West Sensor
                             │
                             ↓
                    Sensor Data Processing
                             │
                             ↓
                    ┌─────────────────┐
                    │  Master ESP32   │
                    │ Traffic Control │
                    └────────┬────────┘
                             │
          ┌──────────────────┼──────────────────┐
          ↓                  ↓                  ↓
      North Light        East Light         South Light
                             │
                         West Light

### 4. Install Backend Dependencies

The backend is implemented using Python and Flask.

Before starting the backend, make sure Python 3 is installed on the system.

Navigate to the backend directory:

```bash
cd backend
pip install flask flask-cors
pip show flask
pip show flask-cors

### 5. Start the Flask Backend

After installing the backend dependencies, start the Flask application.

Navigate to the backend directory:

```bash
cd backend
python app.py


### 6. Start the Dashboard

```markdown
### 6. Start the Dashboard

The dashboard is located in the `dashboard/` directory.

```text
dashboard/
├── index.html
├── app.js
└── style.css


### 7. Verify System Communication

```markdown
### 7. Verify System Communication

After starting the Sensor ESP32, Master ESP32, Flask backend, and dashboard, verify communication between all system components.

### Communication Chain

```text
HC-SR04 Sensors
       ↓
Sensor ESP32
       ↓
Sensor Data
       ↓
Master ESP32
       ↓
┌──────┴──────────────┐
↓                     ↓
Dashboard          Traffic Control
↓
Flask Backend
↓
SQLite Database


### 8. Test Adaptive Traffic Control

```markdown
### 8. Test Adaptive Traffic Control

After verifying basic communication, test the adaptive traffic-control functionality.

Observe the four traffic lanes and change the traffic density detected by the sensors.

The controller should:

1. Receive sensor information.
2. Update the traffic condition of each lane.
3. Calculate lane priority.
4. Select the next lane.
5. Calculate the appropriate Green duration.
6. Control the traffic-light sequence.
7. Continue monitoring the traffic conditions.

### Expected Signal Sequence

```text
Selected Lane
     ↓
Yellow
     ↓
Green
     ↓
Yellow
     ↓
All Red
     ↓
Next Selected Lane


### 9. Test Emergency Priority

```markdown
### 9. Test Emergency Priority

The Emergency section of the dashboard can be used to test emergency vehicle priority.

Select one of the four lanes:

- North
- East
- South
- West

The dashboard sends an emergency request to the Master ESP32.

### Expected Behavior

```text
Select Emergency Lane
        ↓
Emergency Priority Enabled
        ↓
Lane Receives Highest Priority
        ↓
Signal Controller Handles Transition
        ↓
Emergency Lane Gets Green


### 10. Test Traffic Events

```markdown
### 10. Test Traffic Events

The traffic-event detection system can be tested by changing the traffic conditions detected by the sensors.

The system can identify traffic conditions such as:

- `CONGESTION`
- `SUDDEN_TRAFFIC`
- `LANE_BLOCKED`
- `ABNORMAL`

The event detection logic uses a stability period of approximately 5 seconds to reduce false detections caused by temporary sensor changes.

### Expected Behavior

```text
Traffic Condition Changes
          ↓
Event Detection
          ↓
Stability Check
          ↓
Confirmed Event
          ↓
Severity Calculation
          ↓
Dashboard
          ↓
SQLite Historical Log


### 11. Verify Traffic Logging

```markdown
### 11. Verify Traffic Logging

The dashboard periodically sends the latest traffic status to the Flask backend.

The backend stores the received information in the SQLite `traffic_logs` table.

The logging flow is:

```text
Master ESP32
     ↓
Dashboard
     ↓
POST /api/traffic/log
     ↓
Flask Backend
     ↓
Traffic Logger Service
     ↓
SQLite


### 12. Verify Analytics

```markdown
### 12. Verify Analytics

Open the **Analytics** section of the dashboard after traffic data has been stored in SQLite.

The analytics module retrieves historical information through:

```text
GET /api/traffic/analytics



### 13. Complete System Test

```markdown
### 13. Complete System Test

Perform a final end-to-end test after all individual components have been verified.

### Test Sequence

```text
1. Power the Sensor ESP32
          ↓
2. Power the Master ESP32
          ↓
3. Verify Sensor Data
          ↓
4. Verify Traffic Signals
          ↓
5. Start Flask Backend
          ↓
6. Start Dashboard
          ↓
7. Verify Real-Time Traffic
          ↓
8. Test Adaptive Green Timing
          ↓
9. Test Traffic Events
          ↓
10. Test Emergency Priority
          ↓
11. Test Emergency Clear
          ↓
12. Verify SQLite Logging
          ↓
13. Verify Analytics


### 14. Troubleshooting

```markdown
### 14. Troubleshooting

| Problem | Possible Cause | Check |
|---|---|---|
| Sensor data unavailable | Sensor wiring or incorrect GPIO configuration | Check sensor connections and GPIO assignments |
| Master ESP32 unavailable | Wi-Fi or IP address problem | Check Serial Monitor and IP address |
| Traffic lights do not operate | Incorrect GPIO wiring | Verify traffic-light GPIO configuration |
| Dashboard cannot connect to ESP32 | Incorrect ESP32 IP address | Check `dashboard/app.js` |
| Dashboard cannot connect to backend | Flask server not running or incorrect backend IP | Start Flask and verify the configured address |
| CORS error | Backend or ESP32 CORS configuration | Verify CORS handling and browser console |
| Analytics are empty | No historical records | Run the system long enough to create traffic logs |
| Emergency button does not work | Incorrect ESP32 address or endpoint | Verify `/emergency` configuration |
| Emergency cannot be cleared | Incorrect clear endpoint | Verify `/clear-emergency` configuration |
| Traffic events fluctuate | Temporary sensor changes | Allow the event stability period to complete |
| SQLite records are missing | Logging request failed | Check Flask terminal and browser console |

For debugging, check the Arduino IDE Serial Monitor, Flask terminal output, and browser developer console.

## Limitations and Future Improvements

The current prototype demonstrates adaptive traffic management, traffic-event detection, emergency priority, historical logging, and analytics using ESP32-based hardware and ultrasonic sensors.

### Current Limitations

- HC-SR04 ultrasonic sensors provide basic vehicle presence and distance information but do not provide camera-level vehicle classification.
- The current sensor setup cannot reliably identify rash driving behavior.
- The current system cannot reliably perform hit-and-run identification or vehicle identification because it does not use cameras, license-plate recognition, or vehicle tracking.
- Emergency vehicle priority is currently activated through the system's emergency-control interface rather than automatic camera-based emergency vehicle recognition.
- Traffic-density estimation depends on the accuracy and placement of the ultrasonic sensors.
- The prototype is designed for demonstration and experimental traffic-control scenarios rather than direct deployment on public roads.

### Future Improvements

The system can be extended with additional sensing and intelligent traffic-management capabilities.

Potential improvements include:

1. **Camera-Based Vehicle Detection**
   - Use computer vision to detect and classify vehicles.
   - Distinguish cars, buses, trucks, motorcycles, and other road users.

2. **Automatic Emergency Vehicle Detection**
   - Detect emergency vehicles using computer vision, siren recognition, or dedicated communication systems.
   - Automatically trigger emergency priority without manual selection.

3. **Rash Driving Detection**
   - Add camera-based vehicle tracking and speed estimation.
   - Detect behaviors such as excessive speed or abnormal lane movement.

4. **Hit-and-Run Detection**
   - Use cameras and vehicle tracking to identify vehicles involved in incidents.
   - Integrate license-plate recognition where legally and technically appropriate.

5. **IoT Cloud Integration**
   - Store traffic data in a cloud platform.
   - Enable remote monitoring and long-term analytics.

6. **Advanced Traffic Prediction**
   - Apply machine-learning models to historical traffic data.
   - Predict congestion and traffic demand before conditions become severe.

7. **Multi-Junction Coordination**
   - Connect multiple traffic controllers.
   - Coordinate signals across several intersections to improve traffic flow.

8. **Production-Grade Hardware**
   - Replace prototype wiring and sensors with ruggedized sensors, protected enclosures, appropriate power systems, and field-rated communication hardware.

### Future System Vision

```text
Current Prototype
      ↓
Improved Sensors
      ↓
Computer Vision
      ↓
Vehicle Classification
      ↓
Automatic Emergency Detection
      ↓
Traffic Prediction
      ↓
Multi-Junction Coordination
      ↓
Intelligent City-Wide Traffic Management

## Project Status

The project has been developed as an end-to-end working prototype combining embedded traffic control, IoT communication, web monitoring, emergency priority, event detection, historical data storage, and analytics.

### Implemented Features

- [x] Four-lane traffic signal control
- [x] FSM-based traffic signal scheduling
- [x] Four HC-SR04 ultrasonic sensors
- [x] Dedicated Sensor ESP32
- [x] Master ESP32 traffic controller
- [x] Traffic density estimation
- [x] Lane priority calculation
- [x] Adaptive Green signal timing
- [x] Waiting-time consideration
- [x] Traffic event detection
- [x] Traffic event severity classification
- [x] Event stability confirmation
- [x] Emergency vehicle priority control
- [x] Emergency priority dashboard controls
- [x] Real-time traffic dashboard
- [x] Historical traffic logging
- [x] SQLite database
- [x] Daily, weekly, and monthly traffic analytics
- [x] Lane-based traffic analysis
- [x] Event statistics and event history
- [x] Responsive dashboard interface
- [x] Git and GitHub version control

### Current Development Stage

The system is currently at the **working prototype and demonstration stage**.

The implementation focuses on demonstrating the complete data flow:

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
Dashboard
   ↓
Flask Backend
   ↓
SQLite
   ↓
Analytics   

## GitHub Repository

The complete project source code, firmware, backend, dashboard, documentation, and project files are maintained in the GitHub repository.

Repository:

**SmartTrafficAI — IoT-Based Adaptive Smart Traffic Management System with Emergency Vehicle Priority**

The repository contains the implementation of the ESP32-based traffic controller, sensor node, Flask backend, SQLite database integration, web dashboard, and supporting documentation.

### Version Control

Git is used to manage the development history of the project.

Major development milestones are maintained using Git commits and version tags, including:

```text
v3.0-sensor-integration
v3.1-emergency-priority
v3.2-sensor-calibration
v3.3-adaptive-signal-timing
v4.0-sqlite-logging-foundation
v4.1-traffic-logging
v4.2-emergency-dashboard

## License

This project is developed as an academic major project for educational, research, and demonstration purposes.

The source code and project materials are provided through the GitHub repository for learning, experimentation, and further development.

Unless otherwise specified, all project-specific source code and documentation are the work of the project author.