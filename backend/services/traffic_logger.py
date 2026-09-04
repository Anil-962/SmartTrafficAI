from backend.models.traffic_log import TrafficLog


def log_traffic_status(data):
    """
    Store the current traffic controller status in SQLite.
    """

    lanes = data.get("lanes", [])

    # Default values
    north = 0
    east = 0
    south = 0
    west = 0

    # Extract vehicle counts from lane data
    for lane in lanes:
        name = lane.get("name", "")
        vehicles = lane.get("vehicles", 0)

        if name == "North":
            north = vehicles
        elif name == "East":
            east = vehicles
        elif name == "South":
            south = vehicles
        elif name == "West":
            west = vehicles

    TrafficLog.create(
        north_vehicles=north,
        east_vehicles=east,
        south_vehicles=south,
        west_vehicles=west,
        current_lane=data.get("currentLane", "Unknown"),
        signal_state=data.get("signalState", "UNKNOWN"),
        traffic_event=data.get("trafficEvent", "NONE"),
        traffic_severity=data.get("trafficSeverity", "NORMAL")
    )

    return True