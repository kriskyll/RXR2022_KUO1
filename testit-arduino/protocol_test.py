import serial_conn_2 as ser
import time

msg = bytearray(2)

def main():
    print()
    print("Protokollatesti (anna z lopettaaksesi)")
    print()

    while True:
        command = input()
        action = command[:1]
        if action == "z":
            break
        speed = command[2:]
        
        msg[0] = ord(action)
        msg[1] = int(speed)
        print("Lähetetään: ", msg, end="\t")
        print("koko: ", len(msg))
        ser.write(msg)
        time.sleep(0.05)
        print("Arduino palautti: ", end="\t")
        
        # Stringin lukemista
        #line = ser.ser.readline()
        #print(line)

        # Bittien lukemista
        
        ans = ser.read(2)
        print(type(ans), len(ans), end="\t")
        print(ans)
        try:
            print(chr(ans[0]), ans[1])
        except:
            print()

if __name__ == "__main__":
    main()