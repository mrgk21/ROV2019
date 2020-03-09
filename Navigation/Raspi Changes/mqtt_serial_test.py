import paho.mqtt.client as mqtt
import time
import json
import serial

#FLAGS FOR TURNING  ON ARMS  AND NAVIGATION BOARDS
NAVIGATION_ON = True
ARMS_ON = False

#FLAGS FOR TURNING ON SERIAL MODULES
SERIAL_WIRE_ON = False
SERIAL_FTDI_ON = True


#Choosing which serial is used for which board
Navigation = 'ftdi'

Arms = ''
if Navigation=="ftdi":
	Arms = "wire"
elif Navigation=="wire":
	Arms="ftdi"

#---------------------------------- MQTT MESSAGES -------------------------------------------------

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



#----------------------------- DATA FRAMES ---------------------------------------------------------------

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


#---------------------- MQTT INITIALISATION ---------------------------------

client = mqtt.Client("pi3")
client.connect("10.42.0.1", 1883, 60)
client.subscribe("link4D", 0)
client.subscribe("link6D", 0)
client.subscribe("system", 0)
client.subscribe("navFeed", 0)
client.subscribe("pumpSolenoid", 0)
client.on_message = on_message
client.loop_start()


#------------------------  SERIAL MODULE INIT -------------------------------

ser1 = serial.Serial(
    port='/dev/ttyS0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

ser2 = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)



#-----------------  LOCAL DATA VARS -------------------------------------

#Navigation data
mega1PrevData = 0
mega1Data = 1

#Arms data
mega2PrevData = 0
mega2Data = 1

mega1Rec = ""
mega2Rec = ""


#---------------- WAIT FOR MPU INIT ------------------------------
if NAVIGATION_ON:
	msg = "is MPU init?"
	while True:
		if Navigation == 'ftdi':
			if SERIAL_FTDI_ON:
				ser2.write(bytes(msg,'utf-8'))
				mega1Rec = (ser2.readline()).decode('utf-8').rstrip('\r\n')
		elif Navigation =='wire':
			if SERIAL_WIRE_ON:
				ser1.write(bytes(msg,'utf-8'))
				mega1Rec = (ser1.readline()).decode('utf-8').rstrip('\r\n')

		if mega1Rec == "HANDSHAKE SUCCESSFUL":
			print("MPU INITIALIZED")
			break




#--------------------------  MAIN COMM LOOP --------------------------------

count=0
boardSwitch=True
if NAVIGATION_ON:
	boardSwitch = True
elif ARMS_ON:
	boardSwitch = False

while True:
#10 to 1 Rx to Tx ratio
	if count<10:
		if NAVIGATION_ON and boardSwitch:
			if Navigation == 'ftdi':
				mega1Rec = ser2.readline()
			elif Navigation == 'wire':
				mega1Rec = ser1.readline()

			print("mega1Rec: ",mega1Rec.decode('utf-8'),end="")
		if ARMS_ON and not boardSwitch:
			if Arms=='ftdi':
				mega2Rec = ser2.readline()
			elif Arms=='wire':
				mega2Rec = ser1.readline()
			print("mega2Rec: ",mega2Rec.decode('utf-8'),end="")
		if NAVIGATION_ON and ARMS_ON:
			boardSwitch= not boardSwitch

		count+=1
	else:

		mega1Data = str(list(navFeed.values())).strip('[]')
		mega1Data += ", " + str(list(pumpSolenoid.values())).strip('[]')
		if mega1PrevData != mega1Data:
			if NAVIGATION_ON:
				if Navigation=="ftdi":
					ser2.write(bytes(mega1Data, 'utf-8'))
				elif Navigation=="wire":
					ser1.write(bytes(mega1Data,'utf-8'))
				print("Mega1 data: ",mega1Data)

		if mega2PrevData != mega2Data:
			if ARMS_ON:
				if Arms=="ftdi":
					ser2.write(bytes(mega2Data, 'utf-8'))
				elif Arms=="wire":
					ser1.write(bytes(mega2Data,'utf-8'))
				print("Mega2 data: ",mega2Data)

		mega1PrevData = mega1Data
		mega2PrevData = mega2Data
		count = 0



	mega1Rec = ""
	mega2Rec = ""

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

