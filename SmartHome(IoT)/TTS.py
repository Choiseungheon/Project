#-*-coding: utf-8-*-

import os
import time
#play Wav Module
import playsound
#TTS Module
import speech_recognition as sr
from gtts import gTTS

def speak(text):
    tts = gTTS(text=text, lang="ko") #Language Settings => Korean
    filename = "Hello.wav"
    tts.save(filename)

speak("음성인식 시스템이 시작됩니다. 속도가 빠르지 않으니 한 번 명령 후 여유를 갖고 시도해주세요.")
playsound.playsound("Hello.wav")