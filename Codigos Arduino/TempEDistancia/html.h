#ifndef HTML_H
#define HTML_H

const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Monitor</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; font-family: 'Segoe UI', sans-serif; }
    body { 
      min-height: 100vh; 
      display: flex; 
      flex-direction: column;
      justify-content: center; 
      align-items: center; 
      background: linear-gradient(135deg, #1e3c72, #2a5298); 
      color: white; 
      gap: 20px;
    }
    .card {
      background: rgba(255, 255, 255, 0.1);
      backdrop-filter: blur(10px);
      padding: 30px;
      border-radius: 20px;
      text-align: center;
      box-shadow: 0 10px 30px rgba(0,0,0,0.3);
      width: 320px;
    }
    h1 { font-size: 18px; margin-bottom: 15px; opacity: 0.8; }
    .value { font-size: 50px; font-weight: bold; }
    .unit { font-size: 20px; }
    .status { margin-top: 10px; font-size: 16px; }
    .cold { color: #00c6ff; }
    .normal { color: #00ff88; }
    .hot { color: #ff4b2b; }
    .dist { color: #ffeb3b; }
  </style>
</head>
<body>

  <div class="card">
    <h1>TEMPERATURA</h1>
    <div id="TempValue" class="value">--</div>
    <div id="StatusText" class="status">Iniciando...</div>
  </div>

  <div class="card">
    <h1>DISTÂNCIA DO OBJETO</h1>
    <div id="DistValue" class="value dist">--</div>
    <div class="unit">centímetros</div>
  </div>

  <script>
    function updateData() {
      fetch("/readData")
        .then(response => response.text())
        .then(data => {
          let values = data.split(",");
          let temp = parseFloat(values[0]);
          let dist = parseInt(values[1]);

          // Atualiza Temperatura
          let tempElem = document.getElementById("TempValue");
          let statusElem = document.getElementById("StatusText");
          tempElem.innerHTML = temp.toFixed(1) + "<span class='unit'>°C</span>";

          if (temp < 20) { tempElem.className = "value cold"; statusElem.innerHTML = "Ambiente Frio"; }
          else if (temp < 30) { tempElem.className = "value normal"; statusElem.innerHTML = "Temperatura Normal"; }
          else { tempElem.className = "value hot"; statusElem.innerHTML = "Temperatura Alta"; }

          // Atualiza Distância
          document.getElementById("DistValue").innerHTML = dist;
        })
        .catch(err => console.error(err));
    }

    setInterval(updateData, 1000);
    updateData();
  </script>
</body>
</html>
)rawliteral";

#endif