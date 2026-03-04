#ifndef HTML_H
#define HTML_H

const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Temperature Monitor</title>

  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
      font-family: 'Segoe UI', sans-serif;
    }

    body {
      height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
      background: linear-gradient(135deg, #1e3c72, #2a5298);
      color: white;
    }

    .card {
      background: rgba(255, 255, 255, 0.1);
      backdrop-filter: blur(10px);
      padding: 40px;
      border-radius: 20px;
      text-align: center;
      box-shadow: 0 10px 30px rgba(0,0,0,0.3);
      width: 320px;
      transition: 0.3s;
    }

    h1 {
      font-size: 22px;
      margin-bottom: 20px;
      font-weight: 500;
      letter-spacing: 1px;
    }

    .temp {
      font-size: 60px;
      font-weight: bold;
      transition: 0.3s;
    }

    .status {
      margin-top: 10px;
      font-size: 18px;
      opacity: 0.9;
    }

    .cold { color: #00c6ff; }
    .normal { color: #00ff88; }
    .hot { color: #ff4b2b; }

  </style>
</head>

<body>

  <div class="card">
    <h1>ESP32 Temperatura</h1>
    <div id="TempValue" class="temp">°C</div>
    <div id="StatusText" class="status">Connecting...</div>
  </div>

  <script>
    function updateTemp() {
      fetch("/readTemp")
        .then(response => response.text())
        .then(data => {

          let temp = parseFloat(data);
          let tempElement = document.getElementById("TempValue");
          let statusText = document.getElementById("StatusText");

          tempElement.innerHTML = temp.toFixed(1) + " °C";

          if (temp < 20) {
            tempElement.className = "temp cold";
            statusText.innerHTML = "Ambiente Frio";
          } 
          else if (temp >= 20 && temp < 30) {
            tempElement.className = "temp normal";
            statusText.innerHTML = "Temperatura Normal";
          } 
          else {
            tempElement.className = "temp hot";
            statusText.innerHTML = "Temperatura Alta";
          }

        });
    }

    setInterval(updateTemp, 1000);
    updateTemp();
  </script>

</body>
</html>
)rawliteral";

#endif