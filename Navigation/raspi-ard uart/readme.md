# Description
It contains 2 files:
1. raspi.py
	+ This initialises the UART connection from raspi side, writes data to arduino
2. ard.ino
	+ This receives data from UART and identifies certain characters

### Errors/Issues - raspi:
+ `serial.port` takes the port address (this is seen in the `tools -> port` in arduino IDE)
+ data is kept as a character, but a string can also be sent

### Errors/Issues - ard:
+ Nope
