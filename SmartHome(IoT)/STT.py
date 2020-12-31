#!/usr/bin/env python
#-*-coding: utf-8-*-

# [START speech_transcribe_streaming_mic]
from __future__ import division

import re
import sys
import io
import os
os.environ['GOOGLE_APPLICATION_CREDENTIALS'] = r"/home/pi/gentle-bounty-295213-b94f6046db54.json"
os
from google.cloud import speech
import playsound
import pyaudio
from six.moves import queue
#TCP Header
import socket

# Audio recording parameters
RATE = 44100
CHUNK = int(RATE / 10)  # 100ms


class MicrophoneStream(object):
    """Opens a recording stream as a generator yielding the audio chunks."""
    def __init__(self, rate, chunk):
        self._rate = rate
        self._chunk = chunk

        self._buff = queue.Queue()
        self.closed = True

    def __enter__(self):
        self._audio_interface = pyaudio.PyAudio()
        self._audio_stream = self._audio_interface.open(
            format=pyaudio.paInt16,
            channels=1, rate=self._rate,
            input=True, frames_per_buffer=self._chunk,
            stream_callback=self._fill_buffer,
        )

        self.closed = False

        return self

    def __exit__(self, type, value, traceback):
        self._audio_stream.stop_stream()
        self._audio_stream.close()
        self.closed = True
        self._buff.put(None)
        self._audio_interface.terminate()

    def _fill_buffer(self, in_data, frame_count, time_info, status_flags):
        self._buff.put(in_data)
        return None, pyaudio.paContinue

    def generator(self):
        while not self.closed:
            chunk = self._buff.get()
            if chunk is None:
                return
            data = [chunk]

            while True:
                try:
                    chunk = self._buff.get(block=False)
                    if chunk is None:
                        return
                    data.append(chunk)
                except queue.Empty:
                    break

            yield b''.join(data)

def listen_print_loop(responses):
    playsound.playsound("Hello.wav")
    num_chars_printed = 0
    for response in responses:
        if not response.results:
            continue

        result = response.results[0]
        if not result.alternatives:
            continue

        transcript = result.alternatives[0].transcript
        
        overwrite_chars = ' ' * (num_chars_printed - len(transcript))

        if not result.is_final:
            sys.stdout.write(transcript + overwrite_chars + '\r')
            sys.stdout.flush()

            num_chars_printed = len(transcript)

        else:
            print(transcript + overwrite_chars)
            
            # transcript to Sendstring Conversion
            sendString = transcript.replace(' ', '')
            
            print(sendString)
            # Example (Light)
            commandLight = '불'
            commandDescLight = '전체등'
            commandOn = '켜줘'
            commandOff = '꺼줘'
            # Example (FAN)
            commandFan = '환기'
            # Example (LED)
            commandFeel = '분위기'
            commandRomantic = '로맨틱'
            commandWarm = '따뜻한'
            commandFunny = '신나는'
            Description = '설명'
            # Find Result Storage
            resultLight = sendString.find(commandLight)
            resultFan = sendString.find(commandFan)
            resultFeel = sendString.find(commandFeel)
            resultDescription = sendString.find(Description)
            resultSelection = 0
            # Example <-> SendString
            # 1. Light Language Check
            if resultLight >= 0:
                if sendString.find(commandOn) >= 0:
                    resultSelection = 1
                elif sendString.find(commandOff) >= 0:
                    resultSelection = 2
            if resultDescription >= 0:
                if sendString.find(commandDescLight) >= 0:
                    resultSelection = 10
            # 2. Fan Language Check
            if resultFan >= 0:
                if sendString.find(commandOn) >= 0:
                    resultSelection = 3
                elif sendString.find(commandOff) >= 0:
                    resultSelection = 4
                elif resultDescription >= 0:
                    resultSelection = 11
            # 4. Mood Language Check
            if resultFeel >= 0:
                if sendString.find(commandRomantic) >= 0:
                    resultSelection = 5
                elif sendString.find(commandWarm) >= 0:
                    resultSelection = 6
                elif sendString.find(commandFunny) >= 0:
                    resultSelection = 7
                elif resultDescription >= 0:
                    resultSelection = 12
            # Client Server Run
            HOST = '192.168.0.32'
            PORT = 10000
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((HOST, PORT))
            print(resultSelection)
            if resultSelection == 1:
                sendData = bytearray(b'1')
                client_socket.sendall(sendData)
            elif resultSelection == 2:
                sendData = bytearray(b'2')
                client_socket.sendall(sendData)
            elif resultSelection == 3:
                sendData = bytearray(b'3')
                client_socket.sendall(sendData)
            elif resultSelection == 4:
                sendData = bytearray(b'4')
                client_socket.sendall(sendData)
            elif resultSelection == 5:
                sendData = bytearray(b'5')
                client_socket.sendall(sendData)
                playsound.playsound("MusicRomantic.wav")
            elif resultSelection == 6:
                sendData = bytearray(b'6')
                client_socket.sendall(sendData)
                playsound.playsound("MusicWarm.wav")
            elif resultSelection == 7:
                sendData = bytearray(b'7')
                client_socket.sendall(sendData)
                playsound.playsound("MusicFunny.wav")
            elif resultSelection == 8:
                sendData = bytearray(b'8')
                client_socket.sendall(sendData)
            elif resultSelection == 10:
                playsound.playsound("DescLight.wav")
            elif resultSelection == 11:
                playsound.playsound("DescFan.wav")
            elif resultSelection == 12:
                playsound.playsound("DescMood.wav")
            client_socket.close()
            

                    



                
                    
            # Exit recognition if any of the transcribed phrases could be
            # one of our keywords.
            if re.search(r'\b(exit|quit)\b', transcript, re.I):
                print('Exiting..')
                break

            num_chars_printed = 0

                

def main():
    # See http://g.co/cloud/speech/docs/languages
    # for a list of supported languages.
    language_code = 'ko-KR'  # a BCP-47 language tag

    client = speech.SpeechClient()
    config = speech.RecognitionConfig(
        encoding=speech.RecognitionConfig.AudioEncoding.LINEAR16,
        sample_rate_hertz=RATE,
        language_code=language_code)
    streaming_config = speech.StreamingRecognitionConfig(
        config=config,
        interim_results=True)

    with MicrophoneStream(RATE, CHUNK) as stream:
        audio_generator = stream.generator()
        requests = (speech.StreamingRecognizeRequest(audio_content=content)
                    for content in audio_generator)

        responses = client.streaming_recognize(streaming_config, requests)

        # Now, put the transcription responses to use.
        listen_print_loop(responses)
        


if __name__ == '__main__':
    main()
# [END speech_transcribe_streaming_mic]
