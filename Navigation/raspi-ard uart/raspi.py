import serial
import time

ser = serial.Serial()
ser.port = "/dev/ttyACM0"
ser.baudrate = 9600
ser.timeout = 10
data = 'P'
if ser.open():
    print(ser.portstr)
while True:
    ser.write(str.encode(data))
    print("Sent: " + data)
    time.sleep(0.5)
