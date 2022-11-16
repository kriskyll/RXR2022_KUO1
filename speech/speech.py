import time
import vlc

import azure.cognitiveservices.speech as speechsdk

from secrets import azure_token

# Config
subscription = azure_token
region = 'swedencentral'

mode = ""


def recognize_from_microphone(language):
    speech_config = speechsdk.SpeechConfig(subscription=subscription, region=region)
    speech_config.speech_recognition_language = language  # for example "fi-FI" or "en-US"

    audio_config = speechsdk.audio.AudioConfig(use_default_microphone=True)
    speech_recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config, audio_config=audio_config)

    print("Speak into your microphone.")
    speech_recognition_result = speech_recognizer.recognize_once_async().get()

    if speech_recognition_result.reason == speechsdk.ResultReason.RecognizedSpeech:
        print("Recognized: {}".format(speech_recognition_result.text))
    elif speech_recognition_result.reason == speechsdk.ResultReason.NoMatch:
        print("No speech could be recognized: {}".format(speech_recognition_result.no_match_details))
    elif speech_recognition_result.reason == speechsdk.ResultReason.Canceled:
        cancellation_details = speech_recognition_result.cancellation_details
        print("Speech Recognition canceled: {}".format(cancellation_details.reason))
        if cancellation_details.reason == speechsdk.CancellationReason.Error:
            print("Error details: {}".format(cancellation_details.error_details))
            print("Did you set the speech resource key and region values?")


def speech_recognize_keyword_from_microphone():
    """performs keyword-triggered speech recognition with input microphone"""
    speech_config = speechsdk.SpeechConfig(subscription=subscription, region=region)

    # Creates an instance of a keyword recognition model. Update this to
    # point to the location of your keyword recognition model.
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
        elif evt.result.reason == speechsdk.ResultReason.RecognizingSpeech:
            print('RECOGNIZING: {}'.format(evt))

    def recognized_cb(evt: speechsdk.SpeechRecognitionEventArgs):
        """callback for recognized event"""
        if evt.result.reason == speechsdk.ResultReason.RecognizedKeyword:
            print('RECOGNIZED KEYWORD: {}'.format(evt))
        elif evt.result.reason == speechsdk.ResultReason.RecognizedSpeech:
            print('RECOGNIZED: {}'.format(evt))
        elif evt.result.reason == speechsdk.ResultReason.NoMatch:
            print('NOMATCH: {}'.format(evt))

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


def speech_recognize_keyword_from_microphone_test():
    """performs keyword-triggered speech recognition with input microphone"""
    speech_config = speechsdk.SpeechConfig(subscription=subscription, region=region)
    speech_config.speech_recognition_language = "en-US"

    # This model is trained in "Azure speech studio" and contains keyword "Hey Raspi"
    model = speechsdk.KeywordRecognitionModel("hey-raspi-keyword-model.table")

    # The phrase your keyword recognition model triggers on.
    keyword = "Hey Raspi"

    speech_recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config)

    done = False

    def stop_cb(evt: speechsdk.SessionEventArgs):
        """callback that signals to stop continuous recognition upon receiving an event `evt`"""
        nonlocal done
        done = True

    def recognizing_cb(evt: speechsdk.SpeechRecognitionEventArgs):
        """callback for recognizing event"""
        pass

    def recognized_cb(evt: speechsdk.SpeechRecognitionEventArgs):
        """callback for recognized event"""
        if evt.result.reason == speechsdk.ResultReason.RecognizedKeyword:
            print("LISTENING: ON")
        elif evt.result.reason == speechsdk.ResultReason.RecognizedSpeech:
            print("LISTENING: OFF SUCCESS")
            process_command(evt.result.text)
        elif evt.result.reason == speechsdk.ResultReason.NoMatch:
            print("LISTENING: OFF FAILED")

    def process_command(text: str):
        global mode
        text = text.lower()
        print(f"Processing: {text}")

        if "virta pois" in text:
            mode = "shutdown"
        elif "paikka" in text:
            mode = "idle"
        elif "seuraa" in text:
            mode = "follow"
        elif "hauku" in text:
            mode = "WOF WOF"

        print(f"Selected mode: {mode}")

    # Connect callbacks to the events fired by the speech recognizer
    speech_recognizer.recognizing.connect(recognizing_cb)
    speech_recognizer.recognized.connect(recognized_cb)
    # stop continuous recognition on either session stopped or canceled events
    speech_recognizer.session_stopped.connect(stop_cb)
    speech_recognizer.canceled.connect(stop_cb)

    # Start keyword recognition
    speech_recognizer.start_keyword_recognition(model)
    print('Say something starting with "{}" followed by whatever you want...'.format(keyword))
    while not done:
        time.sleep(.5)

    speech_recognizer.stop_keyword_recognition()


# Examples
""" 15 Sec voice detection """
# recognize_from_microphone("fi-FI")  # in finnish
# recognize_from_microphone("en-US")  # in english
""" Continuous voice recognition triggered by 'Hey Raspi' """
# speech_recognize_keyword_from_microphone()

while mode != "shutdown":
    # recognize_from_microphone("fi-FI")
    speech_recognize_keyword_from_microphone_test()

