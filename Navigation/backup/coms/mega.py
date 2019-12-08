import serial, time

ser = serial.Serial(

	port='/dev/ttyS0',
	baudrate=9600,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=1
)
i=0
address="a"
data_a = "1,2,3,4,5"
data_b = "9,6,7,8,3"

while i<9:
	data=""
#	print(data)
	try:
		if address=="a":
			ser.write(bytes(address,'utf-8'))
			ser.write(bytes(data_a, 'utf-8'))
			data = data_a
			print("Packet: " + str(i) + " Sent: " + data)
			address="ra"
		elif address=="ra":
			ser.write(bytes(address,'utf-8'))
		#	time.sleep(0.1)
			data = ser.readline()
			print(data)
			address="b"
		elif address=="b":
			ser.write(bytes(address,'utf-8'))
			ser.write(bytes(data_b, 'utf-8'))
			data = data_b
			print("Packet: " + str(i) + " Sent: " + data)
			address = "rb"
		elif address == "rb":
			ser.write(bytes(address,'utf-8'))
		#	time.sleep(0.1)
			data = ser.readline();
			print(data)
			address="a"
		i = i + 1
		time.sleep(0.1)
	except KeyboardInterrupt:
		ser.close()
