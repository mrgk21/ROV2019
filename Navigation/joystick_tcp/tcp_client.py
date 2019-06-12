import socket

HOST = '192.168.1.121'  # Enter IP or Hostname of your server
print(HOST)
PORT = 8080  # Pick an open Port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while True:
	received = s.recv(1024).decode("utf-8").strip()
	if received == 'Terminate':
		s.close()
		break
	print(received)
