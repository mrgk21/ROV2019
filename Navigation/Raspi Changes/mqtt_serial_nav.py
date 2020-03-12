import paho.mqtt.client as mqtt
import time
import json
import serial


#---------------------------------- MQTT MESSAGES -------------------------------------------------

def on_message(client, userdata, msg):
	global  system, navFeed, pumpSolenoid
	if msg.topic == "navFeed":
		navFeed = json.loads(str(msg.payload.decode('utf-8')))
#		print(navFeed)
	if msg.topic == "pumpSolenoid":
		pumpSolenoid = json.loads(str(msg.payload.decode('utf-8')))
#		print(pumpSolenoid)

	if msg.topic == "system":
        	system = json.loads(str(msg.payload.decode('utf-8')))
	


#----------------------------- DATA FRAMES ---------------------------------------------------------------

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
safetyNav = {
    "leak": True,
    "depth": 0.0,
    "mpu": {
        "Pitch":0.0,
		"Roll":0.0,
		"Yaw":0.0
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


#---------------------- MQTT INITIALISATION ---------------------------------

client = mqtt.Client("pi3")
client.connect("10.42.0.1", 1883, 60)
client.subscribe("system", 0)
client.subscribe("navFeed", 0)
client.subscribe("pumpSolenoid", 0)
client.on_message = on_message
client.loop_start()


#------------------------  SERIAL MODULE INIT -------------------------------
ser1 = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

#-----------------  LOCAL DATA VARS -------------------------------------

#Navigation data
mega1PrevData = ""
mega1Data = ""
mega1Rec = ""



#---------------- WAIT FOR MPU INIT ------------------------------

msg = "is MPU init?"
while True:
	ser1.write(bytes(msg,'utf-8'))
	mega1Rec = (ser1.readline()).decode('utf-8',errors='replace').rstrip('\r\n')

	if mega1Rec == "HANDSHAKE SUCCESSFUL":
		print("MPU INITIALIZED")
		break


#--------------------------  MAIN COMM LOOP --------------------------------
count=0
while True:
#10 to 1 Rx to Tx ratio
	if count<5:
		mega1Rec = (ser1.readline()).decode('utf-8').rstrip('\r\n').split(',')
		mega1Rec = [float(x) for x in mega1Rec]
		print('mega1Rec: ',mega1Rec)
		count += 1
	else:

		mega1Data = str(list(navFeed.values())).strip('[]')
		mega1Data += ", " + str(list(pumpSolenoid.values())).strip('[]')
		if mega1PrevData != mega1Data:
			print('mega1Data: ',mega1Data)
			ser1.write(bytes(mega1Data,'utf-8'))
		mega1PrevData = mega1Data
		count = 0

	mega1Rec = ""
