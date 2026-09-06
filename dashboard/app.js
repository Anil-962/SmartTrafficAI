const url = "http://192.168.137.231/status";
const loggingUrl = "http://10.226.61.224:5000/api/traffic/log";

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

            if(lane.name===data.currentLane)
            {
                document
                    .getElementById("lane"+index)
                    .classList
                    .add("active");
            }
        });

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
updateDashboard();

setInterval(updateDashboard,1000);

setInterval(logTrafficStatus,30000);