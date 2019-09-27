import paho.mqtt.client as mqtt
import time
import json
import serial


def on_message(client, userdata, msg):
    global joyArm, joyNav, system, link
    if msg.topic == "joyArm":
        joyArm = json.loads(str(msg.payload.decode('utf-8')))
    if msg.topic == "joyNav":
        joyNav = json.loads(str(msg.payload.decode('utf-8')))
    if msg.topic == "system":
        system = json.loads(str(msg.payload.decode('utf-8')))

    #temp
    if msg.topic == "link":
        link = json.loads(str(msg.payload.decode('utf-8')))


navFeed = {
    "thr1": 0.0,
    "thr2": 0.0,
    "thr3": 0.0,
    "thr4": 0.0,
    "thr5": 0.0,
    "thr6": 0.0
}

joyNav = {
    "forward": 0.0,
    "backward": 0.0,
    "strafe left": 0.0,
    "strafe right": 0.0,
    "turn left": 0.0,
    "turn right": 0.0,
    "up": 0.0,
    "down": 0.0
}

joyArm = {
  "buttons": [
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0
    ],
    "axes": [
        0.00,
        0.00,
        0.00,
        0.00
      ]
}

safety = {
    "elexShort": True,
    "depth": 0.0,
    "elexBoxTemp": 0.0,
    "workArm": {
        "pot1": 0,
        "pot2": 0,
        "pot3": 0,
    },
    "grabArm": {
        "pot1": 0,
        "pot2": 0,
        "pot3": 0
    },
    "nav": {
        "thr1": 0.0,
        "thr2": 0.0,
        "thr3": 0.0,
        "thr4": 0.0,
        "thr5": 0.0,
        "thr6": 0.0
    }
}

system = {
    "slowArms": False,
    "stopArms": False,
    "startArms": False,
    "slowNav": False,
    "stopNav": False,
    "startNav": False
}

ikFeed = {

}

link = {
    "1": 1,
    "2": 2,
    "3": 3,
    "4": 4
}

client = mqtt.Client("pi3")
client.connect("10.42.0.1", 1883, 60)
client.subscribe("joyArm", 0)
client.subscribe("joyNav", 0)
client.subscribe("system", 0)
client.subscribe("safety", 0)
client.on_message = on_message
client.loop_start()

ser1 = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

#temp
client.subscribe("link", 0)

prevData = []
data = []
while True:

    #temp
    #client.publish("link", json.dumps(link))
    if(data != prevData):
        data = str(list(link.values())).strip('[]')
        ser1.write(bytes(data, 'utf-8'))
    prevData = data
    time.sleep(0.5)


