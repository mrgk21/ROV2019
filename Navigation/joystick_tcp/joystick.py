import pygame

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
	for event in pygame.event.get():  # User did something
		if event.type == pygame.QUIT:  # If user clicked close
			done = True
	data = ""
	for i in range(axes):
		axis = joystick.get_axis(i)
		data += str("%.3f" % float(axis)) + ","

	for i in range(buttons):
		button = joystick.get_button(i)
		data += str(button) + ","

	for i in range(hats):
		hat = joystick.get_hat(i)
		data += str(hat[0]) + ","
		data += str(hat[1])

	print(data)
	clock.tick(10)
pygame.quit()


