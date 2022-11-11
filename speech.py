import azure.cognitiveservices.speech as speechsdk

from secrets import azure_token

api_endpoint = f"https://swedencentral.api.cognitive.microsoft.com/sts/v1.0/{azure_token}"


def recognize_from_mic():
    # Find your key and resource region under the 'Keys and Endpoint' tab in your Speech resource in Azure Portal
    # Remember to delete the brackets <> when pasting your key and region!
    speech_config = speechsdk.SpeechConfig(subscription=azure_token, region="swedencentral")
    speech_recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config)

    # Asks user for mic input and prints transcription result on screen
    print("Speak into your microphone.")
    result = speech_recognizer.recognize_once_async().get()
    print(result.text)


recognize_from_mic()

