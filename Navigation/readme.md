# Description
This contains all all the files used to create the final file `tcp_arduino.py`

## How to use:
1. Run the `tcp_server_joystick.py` code to start the server and send data to the client. The server is configured to transmit a data from joystick in .space separated format  [ axes, buttons, hats]
2. Run the `tcp_arduino.py` code to starts sending data from client to arduino.
3. Run the `ard.ino` code.

## Features:
+ Will communicate from pc to arduino
+ Data sent in .space separated format

## Limitations:
+ There are still some **PORT No.** inconsistencies which were not resolved. The code works fine, but throws a `BrokenPipe` error once in a while. Solution was to add a `s.close()` in case binding failed. 		*seemed to be resolved*
