from gpiozero import RGBLED
from time import sleep


led = RGBLED(17, 27, 22)

def spin():
    for n in range(100):
        led.color = (n/100, 1-n/100, n/200)
        sleep(0.01)
    
    led.color = (.5, 0, 0)
    sleep(.05)
    led.color = (.5, .5, 0)
    sleep(.05)
    led.color = (0, .5, 0)
    sleep(.05)
    led.color = (0, .5, .5)
    sleep(.05)
    led.color = (0, 0, .5)
    sleep(.05)
    led.color = (.5, 0, .5)
    sleep(.05)
    led.color = (.5, 0, 0)
    sleep(.05)
    led.color = (.5, .5, 0)
    sleep(.05)
    led.color = (0, .5, 0)
    sleep(.05)
    led.color = (0, .5, .5)
    sleep(.05)
    led.color = (0, 0, .5)
    sleep(.05)
    led.color = (.5, 0, .5)
    sleep(.05)

def set_mode_stop():
	led.color = (1, 0, 0)

def set_mode_follow():
    led.color = (0.2, 0.2, 0.2)
    
