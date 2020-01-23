import paho.mqtt.client as mqtt
import time
import json
import serial


def on_message(client, userdata, msg):
	global joyArm, joyNav, system, link, navFeed, link4D, link6D, pumpSolenoid
#	print(str(msg.payload.decode('utf-8')))
	if msg.topic == "navFeed":
		navFeed = json.loads(str(msg.payload.decode('utf-8')))
#		print(navFeed)
	if msg.topic == "pumpSolenoid":
		pumpSolenoid = json.loads(str(msg.payload.decode('utf-8')))
	if msg.topic == "link4D":
		link4D = json.loads(str(msg.payload.decode('utf-8')))
#		print(link4D)
	if msg.topic == "link6D":
		link6D = json.loads(str(msg.payload.decode('utf-8')))
#		print(link6D)
	if msg.topic == "joyNav":
        	joyNav = json.loads(str(msg.payload.decode('utf-8')))
	if msg.topic == "system":
        	system = json.loads(str(msg.payload.decode('utf-8')))
	#temp
	if msg.topic == "link":
		link = json.loads(str(msg.payload.decode('utf-8')))

# this is for sending data to arduino
navArd = {
	"thr1": 0.0,
	"thr2": 0.0,
	"thr3": 0.0,
	"thr4": 0.0,
	"thr5": 0.0,
	"thr6": 0.0
}

# this comes from workstation and goes to ard directly
link4D = {
        "1": 0,
        "2": 0,
        "3": 0,
        "4": 0,
	"5": 0
}

# this comes from workstation and goes to ard directly
link6D = {
        "1": 0,
        "2": 0,
        "3": 0,
        "4": 0,
        "5": 0,
        "6": 0
}

# this comes from workstation
navFeed = {
	"x": 0,
	"y": 0,
	"z": 0,
}

#this comes from the workstation
pumpSolenoid = {
	"dir": 0
}

# this goes to the workstation
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

# this comes from workstation
system = {
    "slowArms": False,
    "stopArms": False,
    "startArms": False,
    "slowNav": False,
    "stopNav": False,
    "startNav": False
}

client = mqtt.Client("pi3")
client.connect("10.42.0.1", 1883, 60)
client.subscribe("link4D", 0)
client.subscribe("link6D", 0)
client.subscribe("system", 0)
client.subscribe("navFeed", 0)
client.subscribe("pumpSolenoid", 0)
client.on_message = on_message
client.loop_start()

#ser1 = serial.Serial(
#    port='/dev/ttyS0',
#    baudrate=9600,
#    parity=serial.PARITY_NONE,
#    stopbits=serial.STOPBITS_ONE,
#    bytesize=serial.EIGHTBITS,
#    timeout=1
#)

ser2 = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

mega1PrevData = 0
mega1Data = 1

mega2PrevData = 0
mega2Data = 1

mega1Rec = ""
msg = "is MPU init?"

while True:
	ser2.write(bytes(msg,'utf-8'))
	mega1Rec = (ser2.readline()).decode('utf-8').rstrip('\r\n')
	if mega1Rec == "HANDSHAKE SUCCESSFUL":
		print("MPU INITIALIZED")
		break

count=0

while True:
#10 to 1 Rx to Tx ratio
	if count<10:
		mega1Rec = ser2.readline()
		print("mega1Rec: ",mega1Rec.decode('utf-8'),end="")
		count+=1
	else:
		mega1Data = str(list(navFeed.values())).strip('[]')
		mega1Data += ", " + str(list(pumpSolenoid.values())).strip('[]')
		if mega1PrevData != mega1Data:
			ser2.write(bytes(mega1Data, 'utf-8'))
			print("Mega1 data: ",mega1Data)
		mega1PrevData = mega1Data
		count = 0
	mega1Rec = ""


#	client.publish("ikFeed", "\tiger jinda hai")
#	client.publish("navFeed", json.dumps(navFeed))
#	client.publish("safety", json.dumps(safety))

#	client.publish("link", json.dumps(link))
#	data = str(list(navFeed1.values())).strip('[]')
#	ser1.write(bytes(data, 'utf-8'))



#	if not int(time.time()*100000) % 10000:
#		mega1Data = str(list(navFeed.values())).strip('[]')
#		mega1Data += ", " + str(list(pumpSolenoid.values())).strip('[]')
#		mega2Data = str(list(link4D.values())).strip('[]')
#		mega2Data += ", " + str(list(link6D.values())).strip('[]')
#		print("Mega1 data: ",mega1Data)
#		print(mega2Data)
#		if mega1PrevData != mega1Data:
#			ser2.write(bytes(mega1Data, 'utf-8'))
#			print("Mega1 data: ",mega1Data)
#			while mega1Rec == "":
#				mega1Rec = ser2.readline()
#			ser2.write(bytes(mega1Data, 'utf-8'))
#			mega1PrevData = mega1Data
#			print(mega1Rec.decode('utf-8'), end = '')

#			print(": this was nav")

#		if mega2PrevData != mega2Data:
#			ser2.write(bytes(mega2Data, 'utf-8'))
#			while mega2Rec == "":
#				mega2Rec = ser2.readline()
#				ser2.write(bytes(mega2Data, 'utf-8'))
#			mega2PrevData = mega2Data
#			print(mega2Rec.decode('utf-8'), end = '')
#			print(": this was arm")
#	mega2Rec = ""
#	else:
#		mega1Rec = ser2.readline()
#		print("mega1Rec: ",mega1Rec.decode('utf-8'),end="")

