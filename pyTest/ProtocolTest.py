import time
from random import random
from BColor import *

class ProtocolSender:
    def __init__(self, command, serial, src = None, dst = None):
        self.command = command
        self.source = src
        self.destination = dst
        while not self.source:
            self.source = int((random()*100000)+1)%10
        while not self.destination:
            self.destination = int((random()*100000)+1)%10
        self.datacount = len(self.command)
        self.checksum = 0
        self.CalculateChecksum()
        self.message = ""
        self.ser = serial
        self.response = ""

    def CalculateChecksum(self):
        self.checksum = 0
        for i in self.command:
            self.checksum += ord(i)
        self.checksum %= 256

    def SendAndResp(self):
        if self.datacount < 10:
            self.message = bytes(f"${self.source}{self.destination}0{self.datacount}{self.command}{self.checksum}#",
                                 "ascii")
        if self.datacount >= 10:
            self.message = bytes(f"${self.source}{self.destination}{self.datacount}{self.command}{self.checksum}#",
                                 "ascii")
        print(f"{HEADER}*** FRAME PARAMETERS ***{ENDC}")
        print(f"Message: {BLUE}{self.message}{ENDC}")
        print(f"Source addres: {self.source}, "
              f"Destination address: {self.destination}, "
              f"Data count: {self.datacount}, "
              f"Sent command: {self.command}, "
              f"Calculated checksum: {self.checksum}")
        self.ser.write(self.message)
        time.sleep(0.050)
        self.response = self.ser.read_all()
        print("Response from STM32: ", self.response)
        return self

    '''allows to send any string as bytes'''
    def SendRawAndResp(self):
        print(f"{HEADER}*** FRAME PARAMETERS ***{ENDC}")
        print(f"Message: {BLUE}{self.message}{ENDC}")
        self.ser.write(self.message)
        time.sleep(0.050)
        self.response = self.ser.read_all()
        print("Response from STM32: ", self.response)
        return self