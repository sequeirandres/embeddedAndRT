from phew import server, connect_to_wifi
import machine
import json
from machine import Pin

#Setup Wifi
SSID = "your-SSID"
PASS = "your-PASS"

ip = connect_to_wifi(SSID, PASS)

# Leds setup
ledRed = Pin(18, Pin.OUT)
ledRed.value(1)
ledBlue = Pin(17, Pin.OUT)
ledBlue.value(1)
ledGreen = Pin(16, Pin.OUT)
ledGreen.value(1)

print("connected to IP ", ip)

@server.route("/api/temperature", methods=["GET"])
def get_temperature(request):
    adc = machine.ADC(4)  # Use ADC pin GP4
    conversion_factor = 3.3 / (65535)  # ADC conversion factor
    sensor_value = adc.read_u16() * conversion_factor
    temperature = 27 - (sensor_value - 0.706) / 0.001721  # Convert sensor value to temperature (formula may vary)
    
    return json.dumps({"temperature" : temperature}), 200, {"Content-Type": "application/json"}

@server.route("/api/control-led", methods=["POST"])
def ledCommand(request):
    ledRed.value(request.data["ledRed"])
    ledGreen.value(request.data["ledGreen"])
    ledBlue.value(request.data["ledBlue"])
    return json.dumps({"message" : "Command sent successfully!"}), 200, {"Content-Type": "application/json"}

@server.catchall()
def catchall(request):
    return json.dumps({"message" : "URL not found!"}), 404, {"Content-Type": "application/json"}

server.run()