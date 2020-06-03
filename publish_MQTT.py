import paho.mqtt.client as paho
import serial
import time

mqttc = paho.Client()

# Settings for connection

host = "192.168.0.103"

topic= "Mbed"

port = 1883

serdev = '/dev/ttyACM1'

s = serial.Serial(serdev, 115200)

# Callbacks

def on_connect(self, mosq, obj, rc):

    print("Connected rc: " + str(rc))


def on_message(mosq, obj, msg):

    print("[Received] Topic: " + msg.topic + ", Message: " + str(msg.payload) + "\n");


def on_subscribe(mosq, obj, mid, granted_qos):

    print("Subscribed OK")


def on_unsubscribe(mosq, obj, mid, granted_qos):

    print("Unsubscribed OK")


# Set callbacks

mqttc.on_message = on_message

mqttc.on_connect = on_connect

mqttc.on_subscribe = on_subscribe

mqttc.on_unsubscribe = on_unsubscribe


# Connect and subscribe

print("Connecting to " + host + "/" + topic)

mqttc.connect(host, port=1883, keepalive=60)

mqttc.subscribe(topic, 0)


while(1):

    mesg = s.readline()

    mqttc.publish(topic, mesg)

    print(mesg)