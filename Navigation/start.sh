#!/bin/sh

./startServer.sh
sleep 2s
./startArd.sh
#python3 joystick_tcp/tcp_server_joystick.py
#python3 tcp_arduino.py
echo("All ready")
