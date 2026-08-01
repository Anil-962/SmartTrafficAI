const url = "http://192.168.137.68/status";

async function update()
{
    try
    {
        const response = await fetch(url);

        if (!response.ok)
        {
            throw new Error("HTTP " + response.status);
        }

        const data = await response.json();

        console.log(data);

        document.getElementById("lane").textContent = data.currentLane;
        document.getElementById("state").textContent = data.signalState;
    }
    catch(error)
    {
        console.error(error);

        document.getElementById("lane").textContent = "Connection Error";
        document.getElementById("state").textContent = "";
    }
}

update();

setInterval(update, 1000);