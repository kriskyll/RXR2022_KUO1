from gpiozero import RGBLED, LED
from time import sleep

DEFAULT_COLOR = (1, 1, 1)

led_r = LED(17)
led_g = LED(27)
led_b = LED(22)

def spin():
    led_r.on()
    led_g.on()
    sleep(.1)
    led_r.off()
    led_b.on()
    sleep(.1)
    led_g.off()
    led_r.on()
    sleep(.1)

def set_mode_listen():
	led_r.on()
	led_g.on()
	led_b.on()
	pass
