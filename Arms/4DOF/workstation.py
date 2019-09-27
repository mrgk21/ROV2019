import paho.mqtt.client as mqtt
import json
import time


def on_message(client, userdata, msg):
    global safety, ikFeed
    if str(msg.topic) == "safety":
        safety = json.loads(msg.payload.decode('utf-8'))
    if str(msg.topic) == "ikFeed":
        ikFeed = json.loads(msg.payload.decode('utf-8'))


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
        "elexShort": False,
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

client = mqtt.Client("workstation")
client.connect("0.0.0.0", 1883, 60)
client.subscribe("ikFeed", 0)
client.subscribe("safety", 0)
client.loop_start()

while True:
    #client.publish("system", json.dumps(system))
    client.publish("link", json.dumps(link))
    client.on_message = on_message
    time.sleep(0.5)

