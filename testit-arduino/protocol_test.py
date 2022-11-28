import serial_conn_2 as ser
import time

msg = bytearray(2)

def main():
    print()
    print("Protokollatesti (anna z lopettaaksesi)")
    print()
    testi = int(input("Testimuoto: [1] string parsing [2] bytes \n"))
    
    if testi == 1:
        while True:
            command = input()
            action = command[:1]
            if action == "z":
                break
            speed = command[2:]

            next_command = bytearray(f"{action} {speed}".encode())

            tic = time.perf_counter()
            ser.write(next_command)
            line = ser.ser.readline()
            toc = time.perf_counter()

            line = line.decode('utf-8').rstrip()
            print("Arduino palautti: ")
            print(line)

            print(f"Kellotus: {toc-tic:0.5f} s")

    elif testi == 2:
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
            
            tic = time.perf_counter()
            ser.write(msg)            
            ans = ser.read(2)
            toc = time.perf_counter()
            
            print("Arduino palautti: ", end="\t")
            print(type(ans), len(ans), end="\t")
            print(ans)
            try:
                print(chr(ans[0]), ans[1])
            except:
                print()
            print(f"Kellotus: {toc-tic:0.5f} s")

    else:
        print("Ei oo pakko.")

if __name__ == "__main__":
    main()