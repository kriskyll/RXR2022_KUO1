import time
from playsound import playsound  # TODO: Vaihda parempaan kirjastoon


#     Error 263 for command:
#         open bark.mp3
#     The specified device is not open or is not recognized by MCI.
#
#     Error 263 for command:
#         close bark.mp3
#     The specified device is not open or is not recognized by MCI.
# Failed to close the file: bark.mp3

import azure.cognitiveservices.speech as speechsdk

from secrets import azure_token


# Config
subscription = azure_token
region = 'swedencentral'

mode = ""


def speech_recognize_keyword_from_microphone():
    """performs keyword-triggered speech recognition with input microphone"""
    speech_config = speechsdk.SpeechConfig(subscription=subscription, region=region)
    speech_config.speech_recognition_language = "fi-FI"
    # speech_config.set_property(
    #     property_id=speechsdk.PropertyId.SpeechServiceConnection_InitialSilenceTimeoutMs, value="10000"
    # )
    # speech_config.set_property(
    #     property_id=speechsdk.PropertyId.SpeechServiceConnection_EndSilenceTimeoutMs, value="10000"
    # )
    # # speech_config.set_property(
    # #     property_id=speechsdk.PropertyId.Speech_SegmentationSilenceTimeoutMs, value="10000"
    # # )
    # speech_config.set_property(
    #     property_id=speechsdk.PropertyId.Conversation_Initial_Silence_Timeout, value="10000"
    # )

    # This model is trained in "Azure speech studio" and contains keyword "Hey Raspi"
    model = speechsdk.KeywordRecognitionModel("hey-raspi-keyword-model.table")

    # The phrase your keyword recognition model triggers on.
    keyword = "Hey Raspi"

    speech_recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config)

    done = False

    def stop_cb(evt: speechsdk.SessionEventArgs):
        """callback that signals to stop continuous recognition upon receiving an event `evt`"""
        print('CLOSING on {}'.format(evt))
        nonlocal done
        done = True

    def recognizing_cb(evt: speechsdk.SpeechRecognitionEventArgs):
        """callback for recognizing event"""
        if evt.result.reason == speechsdk.ResultReason.RecognizingKeyword:
            print('RECOGNIZING KEYWORD: {}'.format(evt))
            bark()
            bark()
            print("SPEAK")
        elif evt.result.reason == speechsdk.ResultReason.RecognizingSpeech:
            print('RECOGNIZING: {}'.format(evt))

    def recognized_cb(evt: speechsdk.SpeechRecognitionEventArgs):
        """callback for recognized event"""
        if evt.result.reason == speechsdk.ResultReason.RecognizedKeyword:
            print('RECOGNIZED KEYWORD: {}'.format(evt))
        elif evt.result.reason == speechsdk.ResultReason.RecognizedSpeech:
            print('RECOGNIZED: {}'.format(evt))
            process_command(evt.result.text)
        elif evt.result.reason == speechsdk.ResultReason.NoMatch:
            print('NOMATCH: {}'.format(evt))

    def process_command(text: str):
        global mode
        text = text.lower()
        print(f"Processing: {text}")

        if "virta pois" in text:
            mode = "shutdown"
        elif "paikka" in text:
            mode = "idle"
        elif "pyöri" in text:
            mode = "spin"
        elif "seuraa" in text:
            mode = "follow"
        elif "hauku" in text:
            mode = "WOF WOF"
            bark()

        print(f"Selected mode: {mode}")

    def bark():
        playsound("bark.mp3")

    # Connect callbacks to the events fired by the speech recognizer
    speech_recognizer.recognizing.connect(recognizing_cb)
    speech_recognizer.recognized.connect(recognized_cb)
    # stop continuous recognition on either session stopped or canceled events
    speech_recognizer.session_started.connect(lambda evt: print('SESSION STARTED'))
    speech_recognizer.session_stopped.connect(lambda evt: print('SESSION STOPPED'))
    speech_recognizer.canceled.connect(lambda evt: print('CANCELED'))
    speech_recognizer.session_stopped.connect(stop_cb)
    speech_recognizer.canceled.connect(stop_cb)

    # Start keyword recognition
    speech_recognizer.start_keyword_recognition(model)
    print('Say something starting with "{}" followed by whatever you want...'.format(keyword))
    while not done:
        time.sleep(.5)

    speech_recognizer.stop_keyword_recognition()


""" Continuous voice recognition triggered by 'Hey Raspi' """
# speech_recognize_keyword_from_microphone()
while mode != "shutdown":
    speech_recognize_keyword_from_microphone()


