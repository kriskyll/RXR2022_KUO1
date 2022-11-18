# Main application loop and gpio connections

import serial_conn as ser
import numpy as np
import cv2

if __name__ == "__main__":

    # initialize the HOG descriptor/person detector
    hog = cv2.HOGDescriptor()
    hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())
       
    cv2.startWindowThread()
         
    # open webcam video stream
    cap = cv2.VideoCapture(0)   
    
    while(True):
        # Capture frame-by-frame
        ret, frame = cap.read()
    
        # resizing for faster detection
        frame = cv2.resize(frame, (400, 200))
        # using a greyscale picture, also for faster detection
        gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
     
        # detect people in the image
        # returns the bounding boxes for the detected objects
        boxes, weights = hog.detectMultiScale(frame, winStride=(8,8) )
    
        boxes = np.array([[x, y, x + w, y + h] for (x, y, w, h) in boxes])


        for (xA, yA, xB, yB) in boxes:
            # display the detected boxes in the colour picture
            cv2.rectangle(frame, (xA, yA), (xB, yB), (0, 255, 0), 2)
    
        # Write the output video 
        # Display the resulting frame
        cv2.imshow('frame',frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        if len(boxes) == 0:
            ser.write(bytearray("x 0".encode()))

        if len(boxes) > 0:
            print("1")

            #box[x1, y1, x2, y2]
            box = boxes[0]
            x1 = box[0]
            x2 = box[2]
            print(x1, x2)
            direction = (x1+x2)/2

            # 
            
            if 175 < direction < 225:
                ser.write(bytearray("w 200".encode()))

            elif direction > 205:
                ser.write(bytearray(f"e {direction}".encode()))

            elif direction < 195:
                ser.write(bytearray(f"q {direction}".encode()))
        
        
        #if cv2.waitKey(1) & 0xFF == ord('q'):
        #    break
    
    # When everything done, release the capture
    cap.release()
     
    # finally, close the window
    cv2.destroyAllWindows()
    cv2.waitKey(1)
    
