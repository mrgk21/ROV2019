# Description
This contains all all the files used to create the final file `tcp_arduino.py`

## How to use:
1. Run the `tcp_server.py` code to start the server. The server is configured to transmit a random number every 0.5s.
2. Run the `tcp_arduino.py` code to start sending data from server to arduino.
3. Run the `ard.ino` code.

## Features:
+ Will communicate from pc to arduino
+ Can send multiple characters by changing the `BUFFER_SIZE`  (will have to change arduino code accordingly)

## Limitations:
+ I didn't get time to integrate the joystick code into this.
+ There are still some **PORT No.** inconsistencies which were not resolved. The code works fine, but throws a `BrokenPipe` error once in a while.
