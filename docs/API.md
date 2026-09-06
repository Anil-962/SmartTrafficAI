# API Documentation

## 1. Overview

The Smart Traffic Management System uses HTTP-based APIs for communication between the web dashboard, Master ESP32, and Flask backend.

The APIs use JSON for structured data exchange where applicable.

The system contains two main API layers:

1. **Master ESP32 API** — Provides real-time traffic status and emergency control.
2. **Flask Backend API** — Handles historical traffic logging and analytics.

---

## 2. API Architecture

```text
                    Web Dashboard
                         │
            ┌────────────┴────────────┐
            │                         │
            ↓                         ↓
     Master ESP32               Flask Backend
            │                         │
     ┌──────┼──────┐                  ↓
     ↓      ↓      ↓             SQLite Database
 /status /emergency
         /clear-emergency
```

The dashboard communicates directly with the Master ESP32 for real-time traffic control and communicates with the Flask backend for historical logging and analytics.

---

# 3. Master ESP32 API

The Master ESP32 provides HTTP endpoints for traffic monitoring and emergency priority control.

## 3.1 API Endpoints

| Endpoint | Method | Purpose |
|---|---|---|
| `/status` | `GET` | Returns the current traffic-controller status |
| `/emergency` | `POST` | Activates emergency priority for a selected lane |
| `/clear-emergency` | `POST` | Clears emergency priority |

---

# 4. GET `/status`

The `/status` endpoint provides the current operating state of the traffic controller.

### Request

```http
GET /status
```

No request body is required.

### Response

The endpoint returns JSON containing system information and lane information.

Example:

```json
{
    "project": "SmartTrafficAI",
    "version": "3.1",
    "currentLane": "North",
    "signalState": "GREEN",
    "trafficEvent": "CONGESTION",
    "trafficSeverity": "MEDIUM",
    "wifi": "Connected",
    "rssi": -34,
    "uptime": 162,
    "lanes": [
        {
            "name": "North",
            "vehicles": 10,
            "waiting": 3,
            "priority": 23,
            "emergency": false
        },
        {
            "name": "East",
            "vehicles": 0,
            "waiting": 1,
            "priority": 1,
            "emergency": false
        },
        {
            "name": "South",
            "vehicles": 0,
            "waiting": 0,
            "priority": 0,
            "emergency": false
        },
        {
            "name": "West",
            "vehicles": 0,
            "waiting": 2,
            "priority": 2,
            "emergency": false
        }
    ]
}
```

## 4.1 Response Fields

### System Fields

| Field | Description |
|---|---|
| `project` | Project identifier |
| `version` | Firmware/API version reported by the controller |
| `currentLane` | Currently active traffic lane |
| `signalState` | Current traffic signal state |
| `trafficEvent` | Current detected traffic event |
| `trafficSeverity` | Severity associated with the current traffic event |
| `wifi` | Wi-Fi connection status |
| `rssi` | Wi-Fi signal strength |
| `uptime` | Controller uptime |

### Lane Fields

| Field | Description |
|---|---|
| `name` | Lane name |
| `vehicles` | Current vehicle count/density value for the lane |
| `waiting` | Waiting information for the lane |
| `priority` | Calculated lane priority |
| `emergency` | Indicates whether emergency priority is active for the lane |

---

# 5. POST `/emergency`

The `/emergency` endpoint activates emergency priority for a selected traffic lane.

### Request

```http
POST /emergency
Content-Type: application/json
```

### Request Body

```json
{
    "lane": 0
}
```

### Lane Mapping

| Lane Index | Lane |
|---:|---|
| `0` | North |
| `1` | East |
| `2` | South |
| `3` | West |

### Example

To activate emergency priority for the North lane:

```json
{
    "lane": 0
}
```

To activate emergency priority for the West lane:

```json
{
    "lane": 3
}
```

### Successful Response

```json
{
    "status": "emergency_set",
    "success": true
}
```

### Invalid Lane

The valid lane indexes are `0` through `3`.

An invalid lane value results in an HTTP `400` response.

---

# 6. POST `/clear-emergency`

The `/clear-emergency` endpoint clears emergency priority.

### Request

```http
POST /clear-emergency
```

When no lane is specified, the emergency state is cleared for all lanes.

### Successful Response

```json
{
    "status": "all_emergency_cleared",
    "success": true
}
```

## 6.1 Clearing a Specific Lane

The endpoint can also accept a lane value when a specific emergency lane needs to be cleared.

Example:

```http
POST /clear-emergency
Content-Type: application/json
```

```json
{
    "lane": 0
}
```

Successful response:

```json
{
    "status": "emergency_cleared",
    "success": true
}
```

---

# 7. Emergency API Flow

```text
Dashboard
    │
    │ POST /emergency
    │ {"lane": 0}
    ↓
Master ESP32
    │
    ↓
Set Emergency Lane
    │
    ↓
Lane Receives Highest Priority
    │
    ↓
Traffic Controller Schedules Emergency Lane
```

To clear the emergency:

```text
Dashboard
    │
    │ POST /clear-emergency
    ↓
Master ESP32
    │
    ↓
Clear Emergency State
    │
    ↓
Normal Traffic Scheduling
```

---

# 8. Flask Backend API

The Flask backend provides endpoints for historical traffic logging and analytics.

## 8.1 Backend Endpoints

| Endpoint | Method | Purpose |
|---|---|---|
| `/api/traffic/log` | `POST` | Stores traffic status in SQLite |
| `/api/traffic/analytics` | `GET` | Returns historical traffic and event analytics |

---

# 9. POST `/api/traffic/log`

The dashboard periodically sends the latest Master ESP32 traffic status to this endpoint.

### Request

```http
POST /api/traffic/log
Content-Type: application/json
```

### Request Body

The request contains traffic status data obtained from the Master ESP32.

Example:

```json
{
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
        },
        {
            "name": "East",
            "vehicles": 0,
            "waiting": 1,
            "priority": 1,
            "emergency": false
        },
        {
            "name": "South",
            "vehicles": 0,
            "waiting": 0,
            "priority": 0,
            "emergency": false
        },
        {
            "name": "West",
            "vehicles": 0,
            "waiting": 2,
            "priority": 2,
            "emergency": false
        }
    ]
}
```

The backend extracts the traffic information required for historical storage.

### Stored Information

The backend stores:

- Timestamp
- North vehicle count
- East vehicle count
- South vehicle count
- West vehicle count
- Current lane
- Signal state
- Traffic event
- Traffic severity

### Response

A successful logging request returns a JSON response indicating that the traffic data was stored.

---

# 10. GET `/api/traffic/analytics`

The analytics endpoint retrieves aggregated historical traffic information from the SQLite database.

### Request

```http
GET /api/traffic/analytics
```

No request body is required.

### Response

The response provides information used by the dashboard for:

- Daily traffic statistics
- Weekly traffic statistics
- Monthly traffic statistics
- Lane-based averages
- Traffic trends
- Event statistics
- Event history

The analytics response contains sections for the different traffic-analysis results.

---

# 11. Analytics Data

The analytics API provides information for different time periods.

### Today

The dashboard uses the returned data to display:

- Total traffic
- Average traffic
- Number of logs

### This Week

The dashboard displays:

- Weekly traffic total
- Weekly average traffic
- Number of logs

### This Month

The dashboard displays:

- Monthly traffic total
- Monthly average traffic
- Number of logs

### Lane Analysis

The dashboard calculates/displays average traffic information for:

- North
- East
- South
- West

---

# 12. Event Analytics

The analytics API also provides traffic-event information.

Event statistics are grouped using:

- Event type
- Event severity

Events with `NONE` are excluded from event statistics.

The dashboard uses this information to generate event-related charts.

---

# 13. Event History

The analytics API provides recent traffic-event history.

The event history contains information such as:

| Field | Description |
|---|---|
| `timestamp` | Time at which the event was recorded |
| `event` | Detected traffic event |
| `severity` | Severity of the event |

The dashboard displays the most recent event records in the Events section.

---

# 14. API Communication Flow

The complete dashboard communication pattern is:

```text
                         Web Dashboard
                              │
             ┌────────────────┼────────────────┐
             │                │                │
             ↓                ↓                ↓
        GET /status     POST /emergency   POST /clear-emergency
             │                │                │
             └────────────────┼────────────────┘
                              ↓
                       Master ESP32
                              │
                              │
                              ↓
                    Traffic Controller
                              │
                              │
                         Dashboard
                              │
                 ┌────────────┴────────────┐
                 ↓                         ↓
       POST /api/traffic/log       GET /api/traffic/analytics
                 │                         │
                 ↓                         ↓
          Flask Backend             Flask Backend
                 │                         │
                 └──────────┬──────────────┘
                            ↓
                     SQLite Database
```

---

# 15. HTTP Status Handling

The dashboard checks the HTTP response status after API requests.

Typical handling includes:

| Status | Meaning |
|---:|---|
| `200` | Request successfully processed |
| `204` | Request processed with no response body |
| `400` | Invalid request or invalid lane |
| Other `4xx/5xx` | Request or server error |

The dashboard reports failed requests in the browser console.

---

# 16. JSON Data Exchange

JSON is used as the primary structured data format for communication.

### Sensor-to-Master Example

```json
{
    "north": 55.0,
    "east": 31.1,
    "south": 59.1,
    "west": -1.0
}
```

### Master-to-Dashboard Example

```json
{
    "currentLane": "North",
    "signalState": "GREEN",
    "trafficEvent": "CONGESTION",
    "trafficSeverity": "MEDIUM",
    "lanes": []
}
```

### Dashboard-to-Master Emergency Example

```json
{
    "lane": 0
}
```

---

# 17. API Security and Network Considerations

The current prototype operates on a local network.

The dashboard communicates with the Master ESP32 and Flask backend using their local network addresses.

For deployment beyond a local development environment, additional security mechanisms should be considered, including:

- Authentication
- HTTPS/TLS
- API authorization
- Input validation
- Network access control
- Secure device credentials

These features are outside the current prototype implementation.

---

# 18. API Summary

| Component | Endpoint | Method | Function |
|---|---|---|---|
| Master ESP32 | `/status` | `GET` | Real-time controller status |
| Master ESP32 | `/emergency` | `POST` | Activate emergency priority |
| Master ESP32 | `/clear-emergency` | `POST` | Clear emergency priority |
| Flask Backend | `/api/traffic/log` | `POST` | Store traffic data |
| Flask Backend | `/api/traffic/analytics` | `GET` | Retrieve historical analytics |

---

# 19. API Architecture Summary

The API architecture separates real-time traffic control from historical data management.

The Master ESP32 provides the real-time traffic-control interface, while the Flask backend manages persistent traffic records and analytics.

```text
Real-Time Control
       ↓
Master ESP32 API
       ↓
Traffic Controller


Historical Data
       ↓
Flask REST API
       ↓
SQLite
       ↓
Analytics
       ↓
Dashboard
```