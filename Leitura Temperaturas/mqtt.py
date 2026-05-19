import serial
import time
import paho.mqtt.client as mqtt

# ======================
# Serial configuration
# ======================
SERIAL_PORT = "COM7"   # change to your port (Linux: /dev/ttyUSB0)
BAUD_RATE = 115200

# ======================
# Public MQTT broker
# ======================
BROKER = "broker.hivemq.com"
PORT = 1883

TOPIC1 = "senai/temperatura/sensor1"
TOPIC2 = "senai/temperatura/sensor2"

# ======================
# MQTT setup
# ======================
client = mqtt.Client()
client.connect(BROKER, PORT, 60)

# ======================
# Open serial connection
# ======================
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

print("Reading ESP32 data and publishing to MQTT...")

while True:
    try:
        line = ser.readline().decode().strip()

        # Example expected lines:
        # Sensor 1 (GPIO 27): 25.34 °C
        # Sensor 2 (GPIO 33): 26.10 °C

        if "Sensor 1" in line and "°C" in line:
            temp1 = line.split(":")[1].replace("°C", "").strip()
            client.publish(TOPIC1, temp1)
            print("Published Sensor 1:", temp1)

        elif "Sensor 2" in line and "°C" in line:
            temp2 = line.split(":")[1].replace("°C", "").strip()
            client.publish(TOPIC2, temp2)
            print("Published Sensor 2:", temp2)

        time.sleep(0.1)

    except Exception as e:
        print("Error:", e)
        break