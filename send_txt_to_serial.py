# -*- coding: utf-8 -*-
"""
Copyright: Zissis Papadopoulos
"""

import serial
import time
import sys

args = sys.argv[1:] # get arguments
path = "" # path of file to be sent

# configure the serial port
ser = serial.Serial()
ser.baudrate = 115200

if len(args) < 1:
    print("Please enter a valid file path")
    sys.exit(1)
path = args[0]
ser.port = args[1] if len(args) > 1 else '/dev/ttyUSB0' # replace with your serial port name (e.g. '/dev/ttyUSB0' on Linux)
delay = float(args[2]) if len(args) > 2 else 0.01  # delay for each line
print("Sending file " + path + " to " + ser.port + " at ", 1/delay, " lines per second.")

ser.open()
counter = 0
# send data to the serial port
#ser.write(b'Hello, world!')  # send a byte string
with open(path, 'r') as f:
    # Read the file character by character
    for char in f.read():
        if(char == '\n'):
            ser.write(char.encode())
            counter = 0
            # Wait for a certain amount of time before sending the next character
            time.sleep(delay) # Change the delay time to whatever you want
        else:
            counter += 1
            if(counter >= 21):
                ser.write(b'\n')
                counter = 0
                # Wait for a certain amount of time before sending the next character
                time.sleep(delay) # Change the delay time to whatever you want
            # Send each character to the serial port
            ser.write(char.encode())
# close the serial port when finished
ser.close()
print("DONE")