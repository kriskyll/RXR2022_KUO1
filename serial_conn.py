# Serial connection here

#import serial

#ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
#ser.reset_input_buffer()

def forward():
    #ser.write(b"f\n")
    print("forward")

def stop():
    #ser.write(b"s\n")
    print("stop")

def left():
    #ser.write(b"l\n")
    print("left")

def right():
    #ser.write(b"r\n")
    print("right")
