import serial
import time
from random import random
print("#### Serial command tester for STM32 ####\n")

class ProtocolSender:
    def __init__(self, command, serial, src = None, dst = None):
        self.command = command
        self.source = src
        self.destination = dst
        if not src:
            self.source = int((random()*100000)+1)%10
        if not dst:
            self.destination = int((random()*100000)+1)%10
        self.datacount = len(self.command)
        self.checksum = 0
        for i in self.command:
            self.checksum += ord(i)
        self.checksum %= 256
        self.message = ""
        self.ser = serial

    def SendAndResp(self):
        if self.datacount < 10:
            self.message = bytes(f"${self.source}{self.destination}0{self.datacount}{self.command}{self.checksum}#",
                                 "ascii")
        if self.datacount >= 10:
            self.message = bytes(f"${self.source}{self.destination}{self.datacount}{self.command}{self.checksum}#",
                                 "ascii")
        print("***Frame parameters***")
        print(f"Message: {self.message}")
        print(f"Source addres: {self.source}, "
              f"Destination address: {self.destination}, "
              f"Data count: {self.datacount}, "
              f"Sent command: {self.command}, "
              f"Calculated checksum: {self.checksum}")
        self.ser.write(self.message)
        time.sleep(0.050)
        print("Response from STM32: ", self.ser.read_all())

    def FrameCheksumErrorTest(self, checksumValue = 0):
        print("***ERROR FRAME TEST***")
        print("Attempt to send frame with invalid parameters")
        self.checksum = checksumValue
        return self

if __name__ == "__main__":
    SerialHandler = serial.Serial(
        port="COM4",
        baudrate=115200)
    print("port settings: ", SerialHandler.baudrate)

    commands = ["D1", "D2", "I", "CLR", "READREG10000000000100"]

    for command in commands:
        ProtocolSender(command=command, serial=SerialHandler, src=1, dst=5).SendAndResp()

    ProtocolSender("D1", SerialHandler).FrameCheksumErrorTest().SendAndResp()
    SerialHandler.close()






