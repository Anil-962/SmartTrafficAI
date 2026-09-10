const url = "http://192.168.137.192/status";
const loggingUrl = "http://10.24.210.224:5000/api/traffic/log";
const emergencyUrl =
    "http://192.168.137.192/emergency";

const clearEmergencyUrl =
    "http://192.168.137.192/clear-emergency";
async function updateDashboard()
{
    try
    {
        const response = await fetch(url);

        if(!response.ok)
        {
            throw new Error("HTTP Error " + response.status);
        }

        const data = await response.json();

        // -------------------------
        // System Information
        // -------------------------

      document.getElementById("currentLane").textContent =
         data.currentLane;

        document.getElementById("signalState").textContent =
         data.signalState;

        document.getElementById("trafficEvent").textContent =
          data.trafficEvent || "NONE";

        document.getElementById("rssi").textContent =
             data.rssi;
        document.getElementById("uptime").textContent = data.uptime;
        document.getElementById("version").textContent = data.version;

        // -------------------------
        // Reset all cards
        // -------------------------

        for(let i=0;i<4;i++)
        {
            document
                .getElementById("lane"+i)
                .classList
                .remove("active");
        }


        data.lanes.forEach((lane,index)=>
        {
            document.getElementById("name"+index).textContent =
                lane.name;

            document.getElementById("vehicles"+index).textContent =
                lane.vehicles;

            document.getElementById("waiting"+index).textContent =
                lane.waiting;

            document.getElementById("priority"+index).textContent =
                lane.priority;
            const laneCard =
                document.getElementById(
                    "lane" + index
                );

            if(laneCard)
            {
                if(lane.emergency === true)
                {
                    laneCard.classList.add(
                        "lane-emergency"
                    );
                }
                else
                {
                    laneCard.classList.remove(
                        "lane-emergency"
                    );
                }
            }
            const emergencyBadge =
                document.getElementById(
                    "emergency" + index
                );

            if(emergencyBadge)
            {
                if(lane.emergency === true)
                {
                    emergencyBadge.classList.add(
                        "active"
                    );
                }
                else
                {
                    emergencyBadge.classList.remove(
                        "active"
                    );
                }
            }

            if(lane.name===data.currentLane)
            {
                document
                    .getElementById("lane"+index)
                    .classList
                    .add("active");
            }
        });
        
        // Synchronize Emergency Status
        updateEmergencyStatus(data.lanes);

    }
    catch(error)
    {
        console.error(error);

        document.getElementById("currentLane").textContent="Disconnected";
    }
}
async function logTrafficStatus()
{
    try
    {
        const response = await fetch(url);

        if(!response.ok)
        {
            throw new Error("ESP32 HTTP Error " + response.status);
        }

        const data = await response.json();

        const logResponse = await fetch(loggingUrl, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(data)
        });

        if(!logResponse.ok)
        {
            throw new Error(
                "Logging HTTP Error " + logResponse.status
            );
        }

        const result = await logResponse.json();

        console.log("Traffic log:", result.message);
    }
    catch(error)
    {
        console.error("Traffic logging failed:", error);
    }
}
/* =========================================================
   ANALYTICS DATA
   ========================================================= */

async function updateAnalytics()
{
    try
    {
        const response = await fetch(
            "http://10.24.210.224:5000/api/traffic/analytics"
        );

        if(!response.ok)
        {
            throw new Error(
                "Analytics HTTP Error " + response.status
            );
        }

        const data = await response.json();


        /* =========================
           TODAY
           ========================= */

        document.getElementById("todayTotal").textContent =
            data.today.total_vehicle_observations;

        document.getElementById("todayAverage").textContent =
            data.today.average_vehicle_count;

        document.getElementById("todayLogs").textContent =
            data.today.log_count;


        /* =========================
           THIS WEEK
           ========================= */

        document.getElementById("weekTotal").textContent =
            data.week.total_vehicle_observations;

        document.getElementById("weekAverage").textContent =
            data.week.average_vehicle_count;

        document.getElementById("weekLogs").textContent =
            data.week.log_count;


        /* =========================
           THIS MONTH
           ========================= */

        document.getElementById("monthTotal").textContent =
            data.month.total_vehicle_observations;

        document.getElementById("monthAverage").textContent =
            data.month.average_vehicle_count;

        document.getElementById("monthLogs").textContent =
            data.month.log_count;


        /* =========================
           LANE ANALYSIS
           ========================= */

        document.getElementById("northAverage").textContent =
            data.month.lanes.North.average;

        document.getElementById("eastAverage").textContent =
            data.month.lanes.East.average;

        document.getElementById("southAverage").textContent =
            data.month.lanes.South.average;

        document.getElementById("westAverage").textContent =
            data.month.lanes.West.average;


        console.log(
            "Analytics updated successfully."
        );
        updateTrafficLaneChart(data);
        updateTrafficPeriodChart(data);
        updateTrafficMonthChart(data);
        updateTrafficTrendChart(data);
        updateEventTypeChart(data);
        updateEventSeverityChart(data);
        updateEventHistory(data);
    }
    catch(error)
    {
        console.error(
            "Analytics update failed:",
            error
        );
    }
}

/* =========================================================
   TRAFFIC LANE CHART
   ========================================================= */

let trafficLaneChart = null;

function updateTrafficLaneChart(data)
{
    const canvas =
        document.getElementById("trafficLaneChart");

    if(!canvas)
    {
        return;
    }

    const laneData = data.month.lanes;


    if(trafficLaneChart)
    {
        trafficLaneChart.destroy();
    }


    trafficLaneChart = new Chart(canvas,
    {
        type: "bar",

        data:
        {
            labels:
            [
                "North",
                "East",
                "South",
                "West"
            ],

            datasets:
            [
                {
                    label: "Average Vehicles",

                    data:
                    [
                        laneData.North.average,
                        laneData.East.average,
                        laneData.South.average,
                        laneData.West.average
                    ],

                    borderWidth: 1
                }
            ]
        },

        options:
        {
            responsive: true,

            maintainAspectRatio: false,

            plugins:
            {
                legend:
                {
                    display: true
                }
            },

            scales:
            {
                y:
                {
                    beginAtZero: true,

                    title:
                    {
                        display: true,
                        text: "Average Vehicles"
                    }
                },

                x:
                {
                    title:
                    {
                        display: true,
                        text: "Lane"
                    }
                }
            }
        }
    });
}
/* =========================================================
   TRAFFIC WEEKLY COMPARISON CHART
   ========================================================= */

let trafficPeriodChart = null;

function updateTrafficPeriodChart(data)
{
    const canvas =
        document.getElementById("trafficPeriodChart");

    if(!canvas)
    {
        return;
    }


    if(trafficPeriodChart)
    {
        trafficPeriodChart.destroy();
    }


    const weeklyData =
        data.weekly_comparison;


    trafficPeriodChart = new Chart(canvas,
    {
        type: "bar",

        data:
        {
            labels: weeklyData.map(
                week => week.label
            ),

            datasets:
            [
                {
                    label: "Average Vehicles",

                    data: weeklyData.map(
                        week => week.average_vehicle_count
                    ),

                    borderWidth: 1
                }
            ]
        },

        options:
        {
            responsive: true,

            maintainAspectRatio: false,

            plugins:
            {
                legend:
                {
                    display: true
                }
            },

            scales:
            {
                y:
                {
                    beginAtZero: true,

                    title:
                    {
                        display: true,

                        text: "Average Vehicles"
                    }
                },

                x:
                {
                    title:
                    {
                        display: true,

                        text: "Week"
                    }
                }
            }
        }
    });
}
/* =========================================================
   TRAFFIC MONTHLY COMPARISON CHART
   ========================================================= */

let trafficMonthChart = null;

function updateTrafficMonthChart(data)
{
    const canvas =
        document.getElementById("trafficMonthChart");

    if(!canvas)
    {
        console.error(
            "Monthly chart canvas not found."
        );

        return;
    }


    if(!data.monthly_comparison)
    {
        console.error(
            "Monthly comparison data not found."
        );

        return;
    }


    const monthlyData =
        data.monthly_comparison;


    const labels =
        monthlyData.map(
            month => month.label
        );


    const values =
        monthlyData.map(
            month => month.average_vehicle_count
        );


    if(trafficMonthChart)
    {
        trafficMonthChart.destroy();
    }


    trafficMonthChart = new Chart(
        canvas,
        {
            type: "bar",

            data:
            {
                labels: labels,

                datasets:
                [
                    {
                        label: "Average Vehicles",

                        data: values,

                        borderWidth: 1
                    }
                ]
            },

            options:
            {
                responsive: true,

                maintainAspectRatio: false,

                scales:
                {
                    y:
                    {
                        beginAtZero: true,

                        title:
                        {
                            display: true,

                            text: "Average Vehicles"
                        }
                    },

                    x:
                    {
                        title:
                        {
                            display: true,

                            text: "Month"
                        }
                    }
                },

                plugins:
                {
                    legend:
                    {
                        display: true
                    }
                }
            }
        }
    );


    console.log(
        "Monthly traffic chart updated successfully."
    );
}
/* =========================================================
   24-HOUR TRAFFIC TREND CHART
   ========================================================= */

let trafficTrendChart = null;

function updateTrafficTrendChart(data)
{
    const canvas =
        document.getElementById("trafficTrendChart");

    if(!canvas)
    {
        console.error(
            "Traffic trend chart canvas not found."
        );

        return;
    }


    if(!data.hourly_trend)
    {
        console.error(
            "Hourly trend data not found."
        );

        return;
    }


    const trendData =
        data.hourly_trend;


    const labels =
        trendData.map(
            item =>
            {
                const date =
                    new Date(
                        item.timestamp.replace(" ", "T") + "Z"
                    );

                return date.toLocaleTimeString(
                    [],
                    {
                        hour: "2-digit",
                        minute: "2-digit",
                        hour12: false
                    }
                );
            }
        );


    const values =
        trendData.map(
            item => item.average_vehicle_count
        );


    if(trafficTrendChart)
    {
        trafficTrendChart.destroy();
    }


    trafficTrendChart = new Chart(
        canvas,
        {
            type: "line",

            data:
            {
                labels: labels,

                datasets:
                [
                    {
                        label: "Average Vehicles",

                        data: values,

                        borderWidth: 2,

                        tension: 0.3,

                        fill: false,

                        pointRadius: 4
                    }
                ]
            },

            options:
            {
                responsive: true,

                maintainAspectRatio: false,

                scales:
                {
                    y:
                    {
                        beginAtZero: true,

                        title:
                        {
                            display: true,

                            text: "Average Vehicles"
                        }
                    },

                    x:
                    {
                        title:
                        {
                            display: true,

                            text: "Time"
                        }
                    }
                },

                plugins:
                {
                    legend:
                    {
                        display: true
                    }
                }
            }
        }
    );


    console.log(
        "24-hour traffic trend updated successfully."
    );
}
/* =========================================================
   EVENT TYPE CHART
   ========================================================= */

function updateEventTypeChart(data)
{
    const canvas =
        document.getElementById("eventTypeChart");

    if(!canvas)
    {
        return;
    }

    const eventStatistics =
        data.event_statistics || [];

    const eventCounts = {};

    eventStatistics.forEach(item =>
    {
        const eventName = item.event || "UNKNOWN";

        if(!eventCounts[eventName])
        {
            eventCounts[eventName] = 0;
        }

        eventCounts[eventName] += Number(item.count || 0);
    });

    const labels = Object.keys(eventCounts);
    const values = Object.values(eventCounts);

    if(window.eventTypeChartInstance)
    {
        window.eventTypeChartInstance.destroy();
    }

    window.eventTypeChartInstance =
        new Chart(canvas,
        {
            type: "doughnut",

            data:
            {
                labels: labels,
                datasets:
                [
                    {
                        label: "Events",
                        data: values
                    }
                ]
            },

            options:
            {
                responsive: true,
                maintainAspectRatio: false,

                plugins:
                {
                    legend:
                    {
                        position: "bottom"
                    }
                }
            }
        });
}


/* =========================================================
   EVENT SEVERITY CHART
   ========================================================= */

function updateEventSeverityChart(data)
{
    const canvas =
        document.getElementById("eventSeverityChart");

    if(!canvas)
    {
        return;
    }

    const eventStatistics =
        data.event_statistics || [];

    const severityCounts = {};

    eventStatistics.forEach(item =>
    {
        const severity =
            item.severity || "NORMAL";

        if(!severityCounts[severity])
        {
            severityCounts[severity] = 0;
        }

        severityCounts[severity] +=
            Number(item.count || 0);
    });

    const labels = Object.keys(severityCounts);
    const values = Object.values(severityCounts);

    if(window.eventSeverityChartInstance)
    {
        window.eventSeverityChartInstance.destroy();
    }

    window.eventSeverityChartInstance =
        new Chart(canvas,
        {
            type: "bar",

            data:
            {
                labels: labels,
                datasets:
                [
                    {
                        label: "Event Count",
                        data: values
                    }
                ]
            },

            options:
            {
                responsive: true,
                maintainAspectRatio: false,

                scales:
                {
                    y:
                    {
                        beginAtZero: true,
                        ticks:
                        {
                            precision: 0
                        }
                    }
                },

                plugins:
                {
                    legend:
                    {
                        display: false
                    }
                }
            }
        });
}
/* =========================================================
   EVENT HISTORY TABLE
   ========================================================= */

function updateEventHistory(data)
{
    const tableBody =
        document.getElementById("eventHistoryBody");

    if(!tableBody)
    {
        return;
    }

    const eventHistory =
        data.event_history || [];

    tableBody.innerHTML = "";

    if(eventHistory.length === 0)
    {
        tableBody.innerHTML = `
            <tr>
                <td colspan="3">
                    No event history available.
                </td>
            </tr>
        `;

        return;
    }

    eventHistory.forEach(item =>
    {
        const row =
            document.createElement("tr");

        const timeCell =
            document.createElement("td");

        const eventCell =
            document.createElement("td");

        const severityCell =
            document.createElement("td");

        timeCell.textContent =
            formatEventTime(item.timestamp);

        eventCell.textContent =
            item.event || "UNKNOWN";
        const severity =
            item.severity || "NORMAL";

        severityCell.textContent =
            "";

        const severityBadge =
            document.createElement("span");

        severityBadge.className =
            "severity-badge severity-" +
            severity.toLowerCase();

        severityBadge.textContent =
            severity;

        severityCell.appendChild(
            severityBadge
        );

        row.appendChild(timeCell);
        row.appendChild(eventCell);
        row.appendChild(severityCell);

        tableBody.appendChild(row);
    });
}


/* =========================================================
   EVENT TIME FORMATTER
   ========================================================= */

function formatEventTime(timestamp)
{
    if(!timestamp)
    {
        return "Unknown";
    }

    const date =
        new Date(
            timestamp.replace(" ", "T") + "Z"
        );

    if(isNaN(date.getTime()))
    {
        return timestamp;
    }

    return date.toLocaleString(
        [],
        {
            day: "2-digit",
            month: "short",
            hour: "2-digit",
            minute: "2-digit",
            second: "2-digit",
            hour12: false
        }
    );
}
updateDashboard();
updateAnalytics();
setInterval(updateDashboard,1000);

setInterval(logTrafficStatus,30000);

/* =========================================================
   SECTION NAVIGATION
   ========================================================= */

const navigationButtons =
    document.querySelectorAll(".nav-button");

const dashboardSections =
    document.querySelectorAll(".dashboard-section");


navigationButtons.forEach(button =>
{
    button.addEventListener("click", () =>
    {
        const targetSection =
            button.getAttribute("data-section");

        const selectedSection =
            document.getElementById(targetSection);


        /* Safety check */

        if(!selectedSection)
        {
            console.error(
                "Dashboard section not found:",
                targetSection
            );

            return;
        }


        /* Remove active state from all buttons */

        navigationButtons.forEach(navButton =>
        {
            navButton.classList.remove("active");

            navButton.setAttribute(
                "aria-selected",
                "false"
            );
        });


        /* Hide all sections */

        dashboardSections.forEach(section =>
        {
            section.classList.remove(
                "active-section"
            );
        });


        /* Activate selected button */

        button.classList.add("active");

        button.setAttribute(
            "aria-selected",
            "true"
        );


        /* Show selected section */

        selectedSection.classList.add(
            "active-section"
        );


        /* Return viewport to the top */

        window.scrollTo({
            top: 0,
            behavior: "smooth"
        });
    });
});


/* =========================================================
   INITIAL NAVIGATION STATE
   ========================================================= */

navigationButtons.forEach(button =>
{
    button.setAttribute(
        "aria-selected",
        button.classList.contains("active")
            ? "true"
            : "false"
    );
});
// =====================================================
// Emergency Vehicle Priority
// =====================================================

const emergencyButtons =
    document.querySelectorAll(".emergency-button");

const emergencyStatus =
    document.getElementById("emergencyStatus");

const emergencyPriorityLane =
    document.getElementById("emergencyPriorityLane");

emergencyButtons.forEach(button =>
{
    button.addEventListener("click", async function()
    {
        const laneName =
            this.dataset.lane;

        const laneMap =
        {
            North: 0,
            East: 1,
            South: 2,
            West: 3
        };

        const lane =
            laneMap[laneName];

        if(lane === undefined)
        {
            console.error(
                "Invalid emergency lane:",
                laneName
            );

            return;
        }

        try
        {
            const response =
                await fetch(
                    emergencyUrl,
                    {
                        method: "POST",

                        headers:
                        {
                            "Content-Type":
                                "application/json"
                        },

                        body: JSON.stringify(
                            {
                                lane: lane
                            }
                        )
                    }
                );

            if(!response.ok)
            {
                throw new Error(
                    "Emergency HTTP Error " +
                    response.status
                );
            }

            const result =
                await response.json();

            console.log(
                "Emergency response:",
                result
            );

            emergencyStatus.textContent =
                "ACTIVE EMERGENCY";

            emergencyStatus.classList.remove(
                "inactive"
            );

            emergencyStatus.classList.add(
                "active"
            );

            emergencyPriorityLane.textContent =
                laneName.toUpperCase();
        }
        catch(error)
        {
            console.error(
                "Emergency activation failed:",
                error
            );
        }
    });
});
// =====================================================
// Clear Emergency
// =====================================================

const clearEmergencyButton =
    document.getElementById("clearEmergencyButton");

if(clearEmergencyButton)
{
    clearEmergencyButton.addEventListener(
        "click",
        async function()
        {
            try
            {
                const response =
                    await fetch(
                        clearEmergencyUrl,
                        {
                            method: "POST"
                        }
                    );

                if(!response.ok)
                {
                    throw new Error(
                        "Clear Emergency HTTP Error " +
                        response.status
                    );
                }

                const result =
                    await response.json();

                console.log(
                    "Clear emergency response:",
                    result
                );

                emergencyStatus.textContent =
                    "NO ACTIVE EMERGENCY";

                emergencyStatus.classList.remove(
                    "active"
                );

                emergencyStatus.classList.add(
                    "inactive"
                );

                emergencyPriorityLane.textContent =
                    "NONE";
            }
            catch(error)
            {
                console.error(
                    "Clear emergency failed:",
                    error
                );
            }
        }
    );
}
function updateEmergencyStatus(lanes)
{
    const dashboardEmergencyCard =
    document.querySelector(
        ".emergency-overview-card"
    );
    if(!lanes)
    {
        return;
    }

    const activeLane =
        lanes.find(lane => lane.emergency === true);

    if(activeLane)
    {
        emergencyStatus.textContent =
            "ACTIVE EMERGENCY";

        emergencyStatus.classList.remove(
            "inactive"
        );

        emergencyStatus.classList.add(
            "active"
        );

        emergencyPriorityLane.textContent =
            activeLane.name.toUpperCase();
        if(dashboardEmergencyCard)
{
        dashboardEmergencyCard.classList.add(
            "emergency-active"
        );
}
        const dashboardEmergencyStatus =
            document.getElementById(
                "dashboardEmergencyStatus"
            );

        if(dashboardEmergencyStatus)
        {
            dashboardEmergencyStatus.textContent =
                activeLane.name.toUpperCase();
        }
    }
    else
    {
        emergencyStatus.textContent =
            "NO ACTIVE EMERGENCY";

        emergencyStatus.classList.remove(
            "active"
        );

        emergencyStatus.classList.add(
            "inactive"
        );

        emergencyPriorityLane.textContent =
            "NONE";
        if(dashboardEmergencyCard)
        {
            dashboardEmergencyCard.classList.remove(
                "emergency-active"
            );
        }
        const dashboardEmergencyStatus =
            document.getElementById(
                "dashboardEmergencyStatus"
            );

        if(dashboardEmergencyStatus)
        {
            dashboardEmergencyStatus.textContent =
                "NONE";
        }
    }
}