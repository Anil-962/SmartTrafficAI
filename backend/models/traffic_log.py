from backend.database.db import get_connection


class TrafficLog:

    @staticmethod
    def create(
        north_vehicles,
        east_vehicles,
        south_vehicles,
        west_vehicles,
        current_lane,
        signal_state,
        traffic_event,
        traffic_severity
    ):
        connection = get_connection()

        cursor = connection.cursor()

        cursor.execute("""
            INSERT INTO traffic_logs (
                north_vehicles,
                east_vehicles,
                south_vehicles,
                west_vehicles,
                current_lane,
                signal_state,
                traffic_event,
                traffic_severity
            )
            VALUES (?, ?, ?, ?, ?, ?, ?, ?)
        """, (
            north_vehicles,
            east_vehicles,
            south_vehicles,
            west_vehicles,
            current_lane,
            signal_state,
            traffic_event,
            traffic_severity
        ))

        connection.commit()
        connection.close()