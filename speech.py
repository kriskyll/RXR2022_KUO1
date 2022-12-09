#!/usr/bin/env python3

# NOTE: this example requires PyAudio because it uses the Microphone class

import time

import speech_recognition as sr

from ctypes import *
from contextlib import contextmanager
import pyaudio

ERROR_HANDLER_FUNC = CFUNCTYPE(None, c_char_p, c_int, c_char_p, c_int, c_char_p)

def py_error_handler(filename, line, function, err, fmt):
    pass

c_error_handler = ERROR_HANDLER_FUNC(py_error_handler)

@contextmanager
def noalsaerr():
    asound = cdll.LoadLibrary('libasound.so')
    asound.snd_lib_error_set_handler(c_error_handler)
    yield
    asound.snd_lib_error_set_handler(None)


# TÄNNE MUUTOKSET

# TODO KWARGS THREADIN KÄYNNISTYKSESTÄ

def listen():
    global mode
    print(mode)
    
    r = sr.Recognizer()
    m = sr.Microphone(device_index=2)

    with noalsaerr() as n, m as source:
        r.adjust_for_ambient_noise(source)  # we only need to calibrate once, before we start listenin
        audio = r.listen(source)

    try:
        res = r.recognize_google(audio)
        # to use another API key, use `r.recognize_google(audio, key="GOOGLE_SPEECH_RECOGNITION_API_KEY")`
        print("Google Speech Recognition thinks you said " + res)
        if ( "come" in res):
            print("Mode 1")
            mode = 1
        elif ("stop" in res):
            mode = 0
              
    except sr.UnknownValueError:
        print("Google Speech Recognition could not understand audio")
    except sr.RequestError as e:
        print("Could not request results from Google Speech Recognition service; {0}".format(e))
