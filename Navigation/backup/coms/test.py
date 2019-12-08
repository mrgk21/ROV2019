import serial, time

#ser1 = serial.Serial(
#	port='/dev/ttyS0',
#	baudrate=9600,
#	parity=serial.PARITY_NONE,
#	stopbits=serial.STOPBITS_ONE,
#	bytesize=serial.EIGHTBITS,
#	timeout=1
#)

ser2 = serial.Serial(
        port='/dev/ttyUSB0',
        baudrate=9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)

#while not ser1.open():
#	pass

#while not ser2.open():
#	pass

speed = 0
while True:
	ser2.write(bytes("a ", 'utf-8'))
#	time.sleep(0.5)
#	while True:
	mega1 = ser2.readline()
#		if mega1 != "":
#			break
	print(str(mega1.decode('utf-8')), end = '')
#	time.sleep(speed)
#	mega2 = ser2.readline()
#	print(str(mega2.decode('utf-8')), end = '')
#	time.sleep(speed)
#	ser2.write(bytes("b", 'utf-8'))
#	mega2 = ser.readline()
#	print(str(mega2))
#	time.sleep(0.1)
