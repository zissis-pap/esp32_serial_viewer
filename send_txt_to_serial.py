# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import serial
import time
import sys

# configure the serial port
ser = serial.Serial()
ser.baudrate = 115200
ser.port = '/dev/ttyUSB0'  # replace with your serial port name (e.g. '/dev/ttyUSB0' on Linux)
ser.open()
counter = 0
delay = 0.002
# send data to the serial port
#ser.write(b'Hello, world!')  # send a byte string
with open('/home/zissis/Desktop/bible.txt', 'r') as f:
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