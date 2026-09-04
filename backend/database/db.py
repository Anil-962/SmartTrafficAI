import sqlite3
from pathlib import Path


# Project root
BASE_DIR = Path(__file__).resolve().parents[2]

# Database directory
DATABASE_DIR = BASE_DIR / "backend" / "database"

# SQLite database file
DATABASE_PATH = DATABASE_DIR / "traffic_logs.db"


def get_connection():
    """
    Create and return a SQLite database connection.
    """
    DATABASE_DIR.mkdir(parents=True, exist_ok=True)

    connection = sqlite3.connect(DATABASE_PATH)

    # Allows accessing columns by name
    connection.row_factory = sqlite3.Row

    return connection


def initialize_database():
    """
    Create the traffic_logs table if it does not already exist.
    """
    connection = get_connection()

    cursor = connection.cursor()

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS traffic_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,

            north_vehicles INTEGER DEFAULT 0,
            east_vehicles INTEGER DEFAULT 0,
            south_vehicles INTEGER DEFAULT 0,
            west_vehicles INTEGER DEFAULT 0,

            current_lane TEXT,
            signal_state TEXT,

            traffic_event TEXT DEFAULT 'NONE',
            traffic_severity TEXT DEFAULT 'NORMAL'
        )
    """)

    connection.commit()
    connection.close()


if __name__ == "__main__":
    initialize_database()
    print("SQLite database initialized successfully.")
    print(f"Database: {DATABASE_PATH}")