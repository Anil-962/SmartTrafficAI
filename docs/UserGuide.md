# User Guide

## 1. Overview

The Smart Traffic Management System provides a web-based dashboard for monitoring traffic conditions, viewing historical analytics, monitoring traffic events, and managing emergency vehicle priority.

The dashboard communicates with the Master ESP32 for real-time traffic information and with the Flask backend for historical traffic data and analytics.

---

## 2. Dashboard Navigation

The dashboard contains the following sections:

| Section | Purpose |
|---|---|
| Dashboard | Provides an overview of the current traffic-control state |
| Live Traffic | Displays real-time traffic information for all four lanes |
| Analytics | Displays historical traffic statistics and charts |
| Events | Displays traffic events, severity, and event history |
| Emergency | Provides emergency vehicle priority controls |
| System | Displays system and connectivity information |

Navigation between sections is performed through the dashboard header.

---

# 3. Dashboard Overview

The Dashboard section provides a quick summary of the current system state.

The overview displays:

- Current active lane
- Current signal state
- Current traffic event
- Current traffic severity
- Emergency priority status

Example:

```text
┌─────────────────────────────────────────────┐
│              SmartTrafficAI                 │
├─────────────────────────────────────────────┤
│ Dashboard │ Live Traffic │ Analytics │ ... │
├─────────────────────────────────────────────┤
│                                             │
│ Current Lane       NORTH                    │
│ Signal State       GREEN                    │
│ Traffic Event      CONGESTION               │
│ Severity           MEDIUM                   │
│ Emergency          NONE                     │
│                                             │
└─────────────────────────────────────────────┘
```

The displayed values change according to the current system state.

---

# 4. Live Traffic

The Live Traffic section displays the current traffic condition of the four lanes.

The four lanes are:

- North
- East
- South
- West

Each lane displays traffic information such as:

- Vehicle count
- Waiting information
- Priority score
- Emergency status

Example structure:

```text
┌──────────────┐
│    NORTH     │
│              │
│ Vehicles: 10 │
│ Waiting:  3  │
│ Priority: 23 │
└──────────────┘

┌──────────────┐
│     EAST     │
│              │
│ Vehicles:  0 │
│ Waiting:  1  │
│ Priority:  1 │
└──────────────┘
```

The values are updated using the latest `/status` response from the Master ESP32.

---

# 5. Understanding Lane Priority

The priority value represents the calculated scheduling priority of a lane.

The traffic controller considers:

- Vehicle density
- Waiting time
- Emergency priority

A higher priority value indicates that the lane has a stronger requirement for service.

```text
Vehicle Density
      +
Waiting Time
      +
Emergency Priority
      ↓
Priority Score
      ↓
Lane Scheduling
```

Emergency priority receives substantially greater weight than normal traffic factors.

---

# 6. Traffic Signal States

The traffic controller uses a Finite State Machine to control signal transitions.

The main states are:

| State | Meaning |
|---|---|
| `PRE_GREEN_YELLOW_STATE` | Yellow transition before Green |
| `GREEN_STATE` | Selected lane is allowed to proceed |
| `YELLOW_STATE` | Transition from Green toward Red |
| `ALL_RED_STATE` | Safety interval before the next lane |

The normal sequence is:

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

The controller repeats this process continuously.

---

# 7. Adaptive Traffic Control

The system dynamically adjusts Green duration according to traffic conditions.

The base Green timing is:

| Traffic Condition | Green Time |
|---|---:|
| 0 vehicles | 10 seconds |
| 2 vehicles | 15 seconds |
| 4 vehicles | 20 seconds |
| 6 vehicles | 25 seconds |
| 8 vehicles | 30 seconds |
| 10+ vehicles | 35 seconds |

Waiting time can add additional Green time:

| Waiting Time | Additional Time |
|---|---:|
| Less than 10 seconds | 0 seconds |
| 10 seconds or more | +3 seconds |
| 20 seconds or more | +5 seconds |

The maximum Green duration is limited to:

```text
40 seconds
```

This allows the controller to provide additional service to lanes with greater traffic demand while maintaining a maximum signal duration.

---

# 8. Traffic Events

The Events section displays traffic conditions identified by the traffic controller.

The supported event types are:

| Event | Description |
|---|---|
| `NONE` | Normal traffic conditions |
| `CONGESTION` | High traffic density condition |
| `SUDDEN_TRAFFIC` | Significant change in traffic density |
| `LANE_BLOCKED` | Condition indicating a possible lane blockage |
| `ABNORMAL` | Traffic condition outside the normal pattern |

The system uses an event stability period of approximately five seconds to reduce false detections caused by temporary sensor changes.

---

# 9. Traffic Severity

Traffic events are associated with a severity level.

The supported levels are:

- `NORMAL`
- `LOW`
- `MEDIUM`
- `HIGH`
- `CRITICAL`

The dashboard displays the current severity together with the detected traffic event.

Example:

```text
Traffic Event: CONGESTION
Severity:      MEDIUM
```

Severity information is also stored with historical traffic records.

---

# 10. Event History

The Events section provides recent traffic-event information.

The event history can contain:

- Timestamp
- Event type
- Severity

Example:

```text
┌────────────────────┬────────────────┬──────────┐
│ Timestamp          │ Event          │ Severity │
├────────────────────┼────────────────┼──────────┤
│ 2026-09-06 18:30   │ CONGESTION     │ MEDIUM   │
│ 2026-09-06 18:25   │ SUDDEN_TRAFFIC │ LOW      │
└────────────────────┴────────────────┴──────────┘
```

The event history is obtained from the Flask analytics API.

---

# 11. Analytics

The Analytics section provides historical traffic information stored in the SQLite database.

The dashboard provides statistics for:

- Today
- This Week
- This Month
- Individual lanes

### Today

Displays:

- Total traffic
- Average traffic
- Number of logs

### This Week

Displays:

- Weekly traffic total
- Weekly average traffic
- Number of logs

### This Month

Displays:

- Monthly traffic total
- Monthly average traffic
- Number of logs

### Lane Analysis

Traffic information can be compared across:

- North
- East
- South
- West

---

# 12. Traffic Charts

The Analytics section uses charts to visualize historical traffic information.

The dashboard includes visualizations for:

- Lane-based traffic analysis
- Traffic by time period
- Monthly traffic information
- Traffic trends

Charts help the operator identify differences in traffic demand and changes in traffic conditions over time.

---

# 13. Emergency Vehicle Priority

The Emergency section provides controls for emergency vehicle priority.

The operator can select one of the following lanes:

```text
North
East
South
West
```

Selecting a lane activates emergency priority for that lane.

---

# 14. Activating Emergency Priority

To activate emergency priority:

1. Open the **Emergency** section.
2. Select the lane requiring emergency priority.
3. Click the corresponding lane button.
4. The dashboard sends the emergency request to the Master ESP32.
5. The selected lane receives the highest scheduling priority.

The emergency request uses the following API:

```text
POST /emergency
```

Example request:

```json
{
    "lane": 0
}
```

Lane mapping:

| Index | Lane |
|---:|---|
| `0` | North |
| `1` | East |
| `2` | South |
| `3` | West |

---

# 15. Emergency Status

When emergency priority is active, the dashboard displays:

- `ACTIVE EMERGENCY`
- Priority lane
- Emergency indicator
- Emergency badge on the active Live Traffic lane

The Dashboard overview also indicates the active emergency lane.

Example:

```text
Emergency Priority
NORTH
```

The corresponding lane is visually highlighted in the Live Traffic section.

---

# 16. Clearing Emergency Priority

To clear emergency priority:

1. Open the **Emergency** section.
2. Click **Clear Emergency**.
3. The dashboard sends the request to the Master ESP32.
4. The emergency state is cleared.
5. Normal traffic scheduling resumes.

The request uses:

```text
POST /clear-emergency
```

After clearing the emergency, the dashboard should display:

```text
NO ACTIVE EMERGENCY
```

and:

```text
Priority Lane
NONE
```

The emergency lane highlight and badge should also disappear.

---

# 17. Automatic Emergency Synchronization

The dashboard periodically receives emergency information from the Master ESP32 `/status` endpoint.

This allows the interface to synchronize automatically with the actual controller state.

If an emergency lane is active, the dashboard updates:

- Emergency overview
- Emergency status
- Priority lane
- Live Traffic lane highlight
- Emergency badge

If no emergency lane is active, the dashboard returns these indicators to the normal state.

---

# 18. System Information

The System section provides information about the operating condition of the traffic controller.

The Master ESP32 status response can provide:

- Wi-Fi connection status
- Wi-Fi RSSI
- System uptime
- Current lane
- Signal state
- Traffic event
- Traffic severity

This information can be used to verify that the controller is operating and connected to the network.

---

# 19. Understanding the Complete Dashboard Flow

The dashboard continuously interacts with the Master ESP32 and Flask backend.

```text
             Master ESP32
                  │
                  ↓
              /status
                  │
                  ↓
            Web Dashboard
                  │
       ┌──────────┼──────────┐
       ↓          ↓          ↓
 Live Traffic  Events    Emergency
       │          │          │
       └──────────┼──────────┘
                  ↓
              Analytics
                  │
                  ↓
           Flask Backend
                  │
                  ↓
             SQLite DB
```

---

# 20. Operator Workflow

A typical operator workflow is:

```text
Open Dashboard
      ↓
Check Dashboard Overview
      ↓
Check Live Traffic
      ↓
Monitor Lane Priority
      ↓
Check Traffic Events
      ↓
Review Analytics
      ↓
Handle Emergency if Required
      ↓
Clear Emergency
      ↓
Continue Monitoring
```

---

# 21. Recommended Demonstration Sequence

For a project demonstration, the following sequence can be used:

### Step 1 — Show Hardware

Explain:

- Two ESP32 boards
- Four ultrasonic sensors
- Four traffic-light modules
- Four traffic lanes

### Step 2 — Show Sensor Data

Demonstrate that the sensors provide traffic measurements.

### Step 3 — Show Normal Signal Operation

Demonstrate:

```text
Yellow → Green → Yellow → All Red → Next Lane
```

### Step 4 — Demonstrate Adaptive Timing

Change traffic conditions and show that Green duration changes according to traffic demand.

### Step 5 — Demonstrate Traffic Event Detection

Create a traffic-density condition that triggers an event and show the event and severity on the dashboard.

### Step 6 — Demonstrate Emergency Priority

Select an emergency lane from the Emergency section.

Show:

- Active emergency status
- Priority lane
- Live Traffic highlight
- Emergency badge
- Signal priority

### Step 7 — Clear Emergency

Click **Clear Emergency** and demonstrate the return to normal scheduling.

### Step 8 — Show Analytics

Open the Analytics section and demonstrate:

- Daily statistics
- Weekly statistics
- Monthly statistics
- Lane analysis
- Traffic trends
- Event statistics

### Step 9 — Show Database

Explain that historical traffic information is stored in SQLite through the Flask backend.

---

# 22. Dashboard Indicators

The following indicators can be used when interpreting the dashboard:

| Indicator | Meaning |
|---|---|
| Current Lane | Lane currently being served |
| Signal State | Current FSM signal state |
| Vehicles | Current lane traffic measurement |
| Waiting | Lane waiting information |
| Priority | Calculated lane scheduling priority |
| Traffic Event | Current detected traffic event |
| Severity | Severity of the current event |
| Emergency | Emergency priority state |
| RSSI | Wi-Fi signal strength |
| Uptime | Controller operating time |

---

# 23. Important Limitations

The current prototype uses ultrasonic sensors for traffic detection.

The available sensor configuration is suitable for:

- Vehicle presence/distance measurement
- Traffic-density estimation
- Congestion-related detection
- Traffic anomaly detection

However, the current hardware does not provide reliable identification of:

- Rash driving
- Vehicle speed
- Vehicle identity
- License plates
- Hit-and-run vehicles
- Driver behavior

These capabilities would require additional sensing technologies such as cameras, speed sensors, or other vehicle-tracking systems.

---

# 24. User Guide Summary

The dashboard provides a centralized interface for:

```text
Real-Time Traffic Monitoring
          +
Adaptive Traffic Information
          +
Traffic Event Monitoring
          +
Emergency Priority Control
          +
Historical Analytics
```

The operator can monitor the current traffic state, respond to emergency situations, investigate traffic events, and review historical traffic patterns through the dashboard.