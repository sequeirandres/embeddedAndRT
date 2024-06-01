import network
import time
from machine import Pin
from machine import Timer
from umqtt.simple import MQTTClient

# Leds setup
ledRed = Pin(18, Pin.OUT)
ledRed.value(1)
ledBlue = Pin(17, Pin.OUT)
ledBlue.value(1)
ledGreen = Pin(16, Pin.OUT)
ledGreen.value(1)

# Sensor setup
sensor = Pin(15, Pin.IN)

#Setup Wifi
SSID = "your-SSID"
PASS = "your-PASS"

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(SSID,PASS)
time.sleep(5)
print("Connected to :{} ".format(SSID, wlan.isconnected()))

# Setup MQTT

mqtt_server = '192.168.1.114'
client_id = 'rpipico'
topic_pub = b'sensor'

def sub_cb(topic, msg):
    msg = msg.decode('utf-8')
    topic = topic.decode('utf-8')
    print("New message on topic {}, value {}".format(topic, msg))
    
    led = ledRed if topic == "red" else ledGreen if topic == "green" else ledBlue
    if msg == "on":
        led.off()
    elif msg == "off":
        led.on()
        
def mqtt_connect():
    client = MQTTClient(client_id, mqtt_server, keepalive=3600)
    client.set_callback(sub_cb)
    client.connect()

    print('Connected to %s MQTT Broker'%(mqtt_server))
    return client

connected = False
while (connected == False):
    try:
        client = mqtt_connect()
        connected = True
    except OSError as e:
       print("Could not connect error :{} ".format(e))
       time.sleep(5)
       
client.subscribe(b"blue")
client.subscribe(b"red")
client.subscribe(b"green")

time.sleep(1)

def read_sensor(timer):
    msg = "connected" if sensor.value() == 1 else "disconnected"
    print("Sending to topic {} status {}".format(topic_pub, msg))
    client.publish(topic_pub, msg) 

tim = Timer()
tim.init(period=4000, mode=Timer.PERIODIC, callback=read_sensor)


