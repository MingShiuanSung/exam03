import serial
import matplotlib.pyplot as plt
import numpy as np
import serial
import time
import paho.mqtt.client as paho

# MQTT setting

mqttc = paho.Client()

host = "172.20.10.8"

topic= "velocity"

port = 1883

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


# XBee setting

serdev = '/dev/ttyUSB2'

s = serial.Serial(serdev, 9600)


s.write("+++".encode())

s.write("ATMY 0x39\r\n".encode())

s.write("ATDL 0x40\r\n".encode())

s.write("ATID 0x1\r\n".encode())

s.write("ATWR\r\n".encode())

s.write("ATCN\r\n".encode())

print("start receiving velocity data...")


while True:

    # the format is " " + ".4f" or "-" + ".4f"
    line = s.read(7)

    mesg = str(line.decode())

    mqttc.publish(topic, mesg)

    print(mesg)


s.close()
