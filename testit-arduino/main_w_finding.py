# Main application loop and gpio connections

import numpy as np
import cv2
from time import sleep
from threading import Thread

import serial_conn as ser
import led
from speech import start_sr

if __name__ == "__main__":

    led.spin()
    led.spin()    
    led.set_mode_listen()
    
    test = bytearray(2)

    test[0] = ord("x")
    test[1] = int(0)

    ser.write(test)


    # initialize the HOG descriptor/person detector
    hog = cv2.HOGDescriptor()
    hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())
       
    #cv2.startWindowThread()
         
    # open webcam video stream
    cap = cv2.VideoCapture(0)   

    # frame size
    frame_w = 400
    frame_h = 200

    # motor speed
    speed = 200/2
    no_box_count = 0
    ready = True
    next_command = bytearray(2)
    last_command = bytearray(2)     # To get last known direction
    
    while(True):

        if ser.ser.in_waiting > 0:
            ready = True
            answer = ser.read(2)
            print("Arduino: ", end="\t")
            print(chr(answer[0]), answer[1])

        # counts frames with no box detected, softens the stopping threshold
        
        # Capture frame-by-frame
        ret, frame = cap.read()
    
        # resizing for faster detection
        frame = cv2.resize(frame, (frame_w, frame_h))
        # using a greyscale picture, also for faster detection
        gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
     
        # detect people in the image
        # returns the bounding boxes for the detected objects
        boxes, weights = hog.detectMultiScale(frame, winStride=(8,8) )
    
        boxes = np.array([[x, y, x + w, y + h] for (x, y, w, h) in boxes])


        for (xA, yA, xB, yB) in boxes:
            # display the detected boxes in the colour picture
            cv2.rectangle(gray, (xA, yA), (xB, yB), (0, 255, 0), 2)
    
        # Write the output video 
        # Display the resulting 
        #cv2.imshow('frame', gray)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        # Stopping robot if no human detected
        if len(boxes) == 0:
            no_box_count += 1

            # If human lost
            if no_box_count >= 15:
                # Turn to direction last seen
                print(last_command[0])
                if last_command[0] == ord("w"):
                    next_command[0] = ord("x")
                    next_command[1] = int(speed)
                elif last_command[0] == ord("e"):
                    next_command[0] = ord("a")
                    next_command[1] = int(speed)
                elif last_command[0] == ord("q"):
                    next_command[0] = ord("d")
                    next_command[1] = int(speed)

                if no_box_count == 60:
                    next_command[0] = ord("x")
                    next_command[1] = int(0)
                    no_box_count = 0

        if len(boxes) > 0:
            no_box_count = 0
            
            #box[x1, y1, x2, y2]
            box = boxes[0]
            x1 = box[0]
            x2 = box[2]
            direction = (x1+x2)/2


            #print("direction: ", direction)

            # coefficient for dynamic motor control
            deviation = direction - frame_w/2
            if deviation < 0:
                deviation *= -1

            adjusted_speed = int(speed - deviation/4)
            #print("adjusted speed is:", adjusted_speed)
            #print("speed is : ", speed)
            # MOVEMENT CONTROL
            print(adjusted_speed)

            if 195 <= direction <= 205:
                next_command[0] = ord("w")
                last_command[0] = ord("w")
                next_command[1] = int(speed)
                last_command[1] = int(speed)

            elif direction > 205:
                next_command[0] = ord("e")
                last_command[0] = ord("e")
                next_command[1] = int(adjusted_speed)
                last_command[1] = int(adjusted_speed)

            elif direction < 195:
                next_command[0] = ord("q")
                last_command[0] = ord("q")
                next_command[1] = int(adjusted_speed)
                last_command[1] = int(adjusted_speed)

        if ready:
            ready = False
            ser.write(next_command)
        
    # When everything done, release the capture
    cap.release()
     
    # finally, close the window
    cv2.destroyAllWindows()
    cv2.waitKey(1)
    
