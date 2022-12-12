from threading import Thread
import queue
from time import sleep

from speech import listen

mode = 0
q = queue.Queue()
t = Thread(target=listen, args=(q,))
print("starting thread")
t.start()

while True:
    if (not q.empty()):
            mode = q.get()
            print("MOODIN ON : ", mode)
        
    sleep(.05)

