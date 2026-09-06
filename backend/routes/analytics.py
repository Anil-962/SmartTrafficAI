from flask import Blueprint, jsonify
from backend.database.db import get_connection
from datetime import datetime, timedelta, timezone
from zoneinfo import ZoneInfo

analytics_bp = Blueprint("analytics", __name__)

INDIA_TIMEZONE = ZoneInfo("Asia/Kolkata")


def get_utc_timestamp(local_datetime):
    """
    Convert an India local datetime to UTC timestamp string
    compatible with SQLite CURRENT_TIMESTAMP.
    """
    utc_datetime = local_datetime.astimezone(timezone.utc)

    return utc_datetime.strftime("%Y-%m-%d %H:%M:%S")


def get_period_statistics(start_timestamp):
    connection = get_connection()
    cursor = connection.cursor()

    row = cursor.execute("""
        SELECT
            COUNT(*) AS log_count,

            COALESCE(SUM(north_vehicles), 0) AS north_total,
            COALESCE(SUM(east_vehicles), 0) AS east_total,
            COALESCE(SUM(south_vehicles), 0) AS south_total,
            COALESCE(SUM(west_vehicles), 0) AS west_total,

            COALESCE(AVG(north_vehicles), 0) AS north_average,
            COALESCE(AVG(east_vehicles), 0) AS east_average,
            COALESCE(AVG(south_vehicles), 0) AS south_average,
            COALESCE(AVG(west_vehicles), 0) AS west_average
        FROM traffic_logs
        WHERE timestamp >= ?
    """, (start_timestamp,)).fetchone()

    connection.close()

    total_vehicle_observations = (
        row["north_total"]
        + row["east_total"]
        + row["south_total"]
        + row["west_total"]
    )

    average_vehicle_count = (
        row["north_average"]
        + row["east_average"]
        + row["south_average"]
        + row["west_average"]
    )

    return {
        "log_count": row["log_count"],

        "total_vehicle_observations": total_vehicle_observations,

        "average_vehicle_count": round(
            average_vehicle_count, 2
        ),

        "lanes": {
            "North": {
                "total": row["north_total"],
                "average": round(row["north_average"], 2)
            },
            "East": {
                "total": row["east_total"],
                "average": round(row["east_average"], 2)
            },
            "South": {
                "total": row["south_total"],
                "average": round(row["south_average"], 2)
            },
            "West": {
                "total": row["west_total"],
                "average": round(row["west_average"], 2)
            }
        }
    }
def get_average_for_period(start_timestamp, end_timestamp):
    connection = get_connection()
    cursor = connection.cursor()

    row = cursor.execute("""
        SELECT
            COUNT(*) AS log_count,

            COALESCE(AVG(north_vehicles), 0) AS north_average,
            COALESCE(AVG(east_vehicles), 0) AS east_average,
            COALESCE(AVG(south_vehicles), 0) AS south_average,
            COALESCE(AVG(west_vehicles), 0) AS west_average

        FROM traffic_logs

        WHERE timestamp >= ?
        AND timestamp < ?
    """, (
        start_timestamp,
        end_timestamp
    )).fetchone()

    connection.close()

    average_vehicle_count = (
        row["north_average"]
        + row["east_average"]
        + row["south_average"]
        + row["west_average"]
    )

    return {
        "log_count": row["log_count"],

        "average_vehicle_count": round(
            average_vehicle_count,
            2
        )
    }
def get_hourly_traffic_trend(start_timestamp):
    connection = get_connection()
    cursor = connection.cursor()

    rows = cursor.execute("""
        SELECT
            timestamp,
            north_vehicles,
            east_vehicles,
            south_vehicles,
            west_vehicles

        FROM traffic_logs

        WHERE timestamp >= ?

        ORDER BY timestamp ASC
    """, (start_timestamp,)).fetchall()

    connection.close()


    hourly_data = {}


    for row in rows:

        timestamp = datetime.strptime(
            row["timestamp"],
            "%Y-%m-%d %H:%M:%S"
        )

        hour_key = timestamp.strftime(
            "%Y-%m-%d %H:00:00"
        )


        vehicle_count = (
            row["north_vehicles"]
            + row["east_vehicles"]
            + row["south_vehicles"]
            + row["west_vehicles"]
        )


        if hour_key not in hourly_data:

            hourly_data[hour_key] = []

        
        hourly_data[hour_key].append(
            vehicle_count
        )


    trend = []


    for hour, values in hourly_data.items():

        average = sum(values) / len(values)


        trend.append({
            "timestamp": hour,

            "average_vehicle_count":
                round(average, 2),

            "observation_count":
                len(values)
        })


    return trend
def get_event_statistics():
    connection = get_connection()
    cursor = connection.cursor()

    rows = cursor.execute("""
        SELECT
            traffic_event,
            traffic_severity,
            COUNT(*) AS event_count

        FROM traffic_logs

        WHERE traffic_event IS NOT NULL
        AND traffic_event != 'NONE'

        GROUP BY
            traffic_event,
            traffic_severity

        ORDER BY event_count DESC
    """).fetchall()

    connection.close()


    event_statistics = []


    for row in rows:

        event_statistics.append({
            "event": row["traffic_event"],

            "severity": row["traffic_severity"],

            "count": row["event_count"]
        })


    return event_statistics
def get_event_history(limit=20):
    connection = get_connection()
    cursor = connection.cursor()

    rows = cursor.execute("""
        SELECT
            timestamp,
            traffic_event,
            traffic_severity
        FROM traffic_logs
        WHERE traffic_event IS NOT NULL
        AND traffic_event != 'NONE'
        ORDER BY timestamp DESC
        LIMIT ?
    """, (limit,)).fetchall()

    connection.close()

    event_history = []

    for row in rows:
        event_history.append({
            "timestamp": row["timestamp"],
            "event": row["traffic_event"],
            "severity": row["traffic_severity"]
        })

    return event_history

@analytics_bp.route("/api/traffic/analytics", methods=["GET"])
def traffic_analytics():

    now = datetime.now(INDIA_TIMEZONE)

    # Start of today in India
    start_of_today = now.replace(
        hour=0,
        minute=0,
        second=0,
        microsecond=0
    )

    # Start of current week (Monday)
    start_of_week = start_of_today - timedelta(
        days=start_of_today.weekday()
    )

    # Start of current month
    start_of_month = start_of_today.replace(day=1)
    # =====================================================
    # LAST 4 WEEKS
    # =====================================================

    weekly_comparison = []

    for weeks_ago in range(3, -1, -1):

        week_start = start_of_week - timedelta(
            weeks=weeks_ago
        )

        week_end = week_start + timedelta(
            weeks=1
        )

        week_stats = get_average_for_period(
            get_utc_timestamp(week_start),
            get_utc_timestamp(week_end)
        )

        weekly_comparison.append({
            "label": (
                week_start.strftime("%d %b")
                + " - "
                + (week_end - timedelta(days=1)).strftime("%d %b")
            ),

            "average_vehicle_count":
                week_stats["average_vehicle_count"],

            "log_count":
                week_stats["log_count"]
        })
    # =====================================================
    # LAST 4 MONTHS
    # =====================================================

    monthly_comparison = []

    month_starts = []

    current_month = start_of_month

    for _ in range(4):

        month_starts.insert(
            0,
            current_month
        )

        previous_month = (
            current_month - timedelta(days=1)
        ).replace(day=1)

        current_month = previous_month


    for index, month_start in enumerate(month_starts):

        if month_start.month == 12:

            month_end = month_start.replace(
                year=month_start.year + 1,
                month=1,
                day=1
            )

        else:

            month_end = month_start.replace(
                month=month_start.month + 1,
                day=1
            )


        month_stats = get_average_for_period(
            get_utc_timestamp(month_start),
            get_utc_timestamp(month_end)
        )


        monthly_comparison.append({
            "label":
                month_start.strftime("%b %Y"),

            "average_vehicle_count":
                month_stats["average_vehicle_count"],

            "log_count":
                month_stats["log_count"]
        })
    # =====================================================
    # LAST 24 HOURS TRAFFIC TREND
    # =====================================================

    start_of_24_hours = now - timedelta(
        hours=24
    )
    event_statistics = get_event_statistics()
    event_history = get_event_history()


    hourly_trend = get_hourly_traffic_trend(
        get_utc_timestamp(start_of_24_hours)
    )

    today_stats = get_period_statistics(
        get_utc_timestamp(start_of_today)
    )

    week_stats = get_period_statistics(
        get_utc_timestamp(start_of_week)
    )

    month_stats = get_period_statistics(
        get_utc_timestamp(start_of_month)
    )
    

    return jsonify({
        "status": "success",

        "timezone": "Asia/Kolkata",

        "today": today_stats,

        "week": week_stats,

        "month": month_stats,
        "weekly_comparison": weekly_comparison,

        "monthly_comparison": monthly_comparison,
        "hourly_trend": hourly_trend,
        "event_statistics": event_statistics,
        "event_history": event_history
    })