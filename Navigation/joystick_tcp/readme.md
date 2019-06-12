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
+ The prototype code provided is expected to give a `Port busy Error` because the python code is using it to transmit data. 
+ A static testing of the code was done, and I found that satisfactory.
