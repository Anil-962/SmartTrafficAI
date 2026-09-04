from flask import Flask, request, jsonify

from backend.database.db import initialize_database
from backend.services.traffic_logger import log_traffic_status


app = Flask(__name__)


# Initialize SQLite database when Flask starts
initialize_database()


@app.route("/")
def home():
    return jsonify({
        "project": "SmartTrafficAI",
        "status": "Backend running"
    })


@app.route("/api/traffic/log", methods=["POST"])
def log_traffic():
    try:
        data = request.get_json()

        if not data:
            return jsonify({
                "status": "error",
                "message": "No JSON data received"
            }), 400

        log_traffic_status(data)

        return jsonify({
            "status": "success",
            "message": "Traffic status logged successfully"
        })

    except Exception as e:
        return jsonify({
            "status": "error",
            "message": str(e)
        }), 500


if __name__ == "__main__":
    app.run(
        host="0.0.0.0",
        port=5000,
        debug=True
    )