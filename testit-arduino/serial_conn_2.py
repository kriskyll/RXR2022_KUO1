# Serial connection here

import serial

ser = serial.Serial('COM5', 9600, timeout=1)
ser.reset_input_buffer()
ser.reset_output_buffer()


def write(msg):
    #print(command)
    ser.write(msg)

def read(size):
    return ser.read(size)

def in_waiting():
    return ser.inWaiting()