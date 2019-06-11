import socket, random, time

# HOST = socket.gethostbyname("localhost")  # Server IP
HOST = '0.0.0.0'
PORT = 1111  # Port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Socket created')
# s.close()
try:
	s.bind((HOST, PORT))
except socket.error:
	print('Bind failed')

s.listen(2)
print('Socket awaiting messages')
(conn, addr) = s.accept()
print('Connected')

while True:
	x = str(random.randint(1, 99) % 2)
	print(x)
	conn.send(bytes(x, "utf-8"))
	time.sleep(0.5)
