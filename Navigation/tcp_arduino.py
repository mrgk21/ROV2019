import socket, serial, time

HOST = '192.168.0.14'
PORT = 60000
BUFFER_SIZE = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

ser = serial.Serial()
ser.port = "/dev/ttyACM0"
ser.baudrate = 9600
ser.timeout = 10

if ser.open():
	print(ser.portstr)
preData = 0
# s.close()
while True:
	data = s.recv(BUFFER_SIZE).decode("utf-8").strip()
	# print(data)
	if preData != data:
		ser.write(bytes(data, "utf-8"))
		print("Sent: " + data)
	preData = data
