#ifndef CONFIG_H
#define CONFIG_H

// North Signal
const int NORTH_RED = 25;
const int NORTH_YELLOW = 26;
const int NORTH_GREEN = 27;

// East Signal
const int EAST_RED = 18;
const int EAST_YELLOW = 19;
const int EAST_GREEN = 21;

// South Signal
const int SOUTH_RED = 22;
const int SOUTH_YELLOW = 23;
const int SOUTH_GREEN = 5;

// West Signal
const int WEST_RED = 13;
const int WEST_YELLOW = 14;
const int WEST_GREEN = 32;

#endif
// ==========================
// Traffic Algorithm Settings
// ==========================

// Signal timings (seconds)
const int RED_TIME = 30;
const int YELLOW_TIME = 3;
const int MIN_GREEN_TIME = 10;
const int MAX_GREEN_TIME = 60;

// Scheduler
const int SCHEDULER_INTERVAL = 5;   // Recalculate every 5 seconds

// Priority Weights
const float VEHICLE_WEIGHT = 2.0f;
const float WAITING_WEIGHT = 1.0f;
const float EMERGENCY_WEIGHT = 1000.0f;