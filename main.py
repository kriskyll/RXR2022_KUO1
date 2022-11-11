# Main application loop and gpio connections

import serial_conn as ser

if __name__ == "__main__":

    while True:
        x  = input()
        if x == "f":
            ser.forward()
        elif x == "s":
            ser.stop()
        elif x == "l":
            ser.left()
        elif x == "r":
            ser.right()    
