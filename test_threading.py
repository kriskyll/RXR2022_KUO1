from threading import Thread

from speech import start_sr

t = Thread(target=start_sr)
print("starting thread")
t.start()

