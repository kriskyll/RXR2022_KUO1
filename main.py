# Main application loop and gpio connections

import serial_conn as ser

if __name__ == "__main__":

    while True:
        x  = input()
        ser.write(bytearray(x.encode()))  
