## Files:
1. `mqtt_uart_test.py` goes on the pi3
	+ This code takes data from the PC, and converts it to CSV format
2. Upload `4DOF_test.ino` to ard mega
	+ Takes data from CSV format and sends PWM to servos
3. Run `workstation.py` on the PC for debugging. (check in the prev directory)
	+ Sends dummy signals to rpi for testing the connection
4. Run `uart_test.ino` for testing ard uart

## Hardware setup:
+ USB-TTL module is connected to the raspi.
+ Rx, TX, 5V, GND wires are connected to mega.( **Rx-Tx -> Serial1**)

