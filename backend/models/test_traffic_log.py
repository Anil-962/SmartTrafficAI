from backend.models.traffic_log import TrafficLog


TrafficLog.create(
    north_vehicles=5,
    east_vehicles=3,
    south_vehicles=7,
    west_vehicles=2,
    current_lane="North",
    signal_state="GREEN",
    traffic_event="CONGESTION",
    traffic_severity="MEDIUM"
)

print("Traffic log inserted successfully.")