import pygame, socket, time

HOST = '0.0.0.0'  # IP address
PORT = 60000  # Port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Socket created')

try:
	s.bind((HOST, PORT))
except socket.error:
	print("Bind failed")
	s.close()

s.listen(1)
print('Socket awaiting messages')
(conn, addr) = s.accept()
print("Connected")

pygame.init()
pygame.joystick.init()
count = pygame.joystick.get_count()
clock = pygame.time.Clock()
joystick = pygame.joystick.Joystick(0)
joystick.init()

axes = joystick.get_numaxes()
buttons = joystick.get_numbuttons()
hats = joystick.get_numhats()

done = False
while not done:
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
	data = ""
	for i in range(axes):
		axis = joystick.get_axis(i)
		data += str(axis * 1000) + " "

	for i in range(buttons):
		button = joystick.get_button(i)
		data += str(button) + " "

	for i in range(hats):
		hat = joystick.get_hat(i)
		data += str(hat[0]) + " "
		data += str(hat[1])

	conn.send(bytes(data, "utf-8"))
	print("Sent from server: " + str(bytes(data, "utf-8")))
	clock.tick(10)
	time.sleep(0.1)
pygame.quit()
