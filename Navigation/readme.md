# Description
This contains all all the files used to create the final file `tcp_arduino.py`

## How to use:
1. Run the `tcp_server_joystick.py` code to start the server and send data to the client. The server is configured to transmit a data from joystick in .csv format  [ axes, buttons, hats]
2. Run the `tcp_arduino.py` code to starts sending data from client to arduino.
3. Run the `ard.ino` code.

## Features:
+ Will communicate from pc to arduino
+ Data sent in .csv format

## Limitations:
+ Parsing of data on the arduino side needs to be completed (data reception on arduino side is confirmed)
+ There are still some **PORT No.** inconsistencies which were not resolved. The code works fine, but throws a `BrokenPipe` error once in a while. Solution was to add a `s.close()` in case binding failed. 		*seemed to be resolved*
