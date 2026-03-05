import serial
import paho.mqtt.client as mqtt
import time

# Configurações
serial_port = 'COM5' 
baud_rate = 115200  # Ajustado de 115000 para 115200
broker_address = "broker.hivemq.com"
broker_port = 1883
topic = "SensorUltrassonicoJG"
client_id = "SensorUltrassonicoJG_Python" # ID único é boa prática

client = mqtt.Client(client_id=client_id, protocol=mqtt.MQTTv311)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("✅ Conectado ao broker MQTT!")
    else:
        print(f"❌ Falha na conexão, código: {rc}")

client.on_connect = on_connect

# Conecta e inicia o loop
print("Conectando ao broker...")
client.connect(broker_address, broker_port, 60)
client.loop_start()

# Aguarda a conexão ser estabelecida de fato antes de seguir
while not client.is_connected():
    time.sleep(0.5)



try:
    ser = serial.Serial(serial_port, baud_rate, timeout=1)
    print(f"Porta serial {serial_port} aberta. Lendo dados...")

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line:
                print(f"Dados da serial: {line}")
                
                # Publica sem travar o código se a conexão oscilar
                result = client.publish(topic, line)
                
                # Em vez de wait_for_publish (que causa o crash), verificamos o status
                if result.rc != mqtt.MQTT_ERR_SUCCESS:
                    print(f"Erro ao agendar publicação: {result.rc}")

        time.sleep(0.1)

except serial.SerialException as e:
    print(f"Erro na serial: {e}")
except KeyboardInterrupt:
    print("\nEncerrando...")
finally:
    client.loop_stop()
    client.disconnect()
    if 'ser' in locals() and ser.is_open:
        ser.close()
    print("Conexões fechadas.")