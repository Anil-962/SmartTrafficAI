const url = "http://192.168.137.99/status";

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
// Log traffic data to Flask
// -------------------------

    fetch("http://127.0.0.1:5000/api/traffic/log", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(data)
    })
    .catch(error => {
        console.error("Traffic logging error:", error);
    });

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

updateDashboard();

setInterval(updateDashboard,1000);