# Serial connection here

import serial

ser = serial.Serial('/dev/ttyACM0', 38400, timeout=1)
ser.reset_input_buffer()


def write(command):
    #print(command)
    ser.write(command)
