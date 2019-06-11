import socket

HOST = socket.gethostbyname("localhost") # Enter IP or Hostname of your server
print(HOST)
PORT = 1111  # Pick an open Port (1000+ recommended), must match the server port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while True:
	received = s.recv(1024).decode("utf-8").strip()
	if received == 'Terminate':
		s.close()
		break
	print(received)
