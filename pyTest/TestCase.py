from ProtocolTest import ProtocolSender
import serial

class FrameTests(ProtocolSender):
    def __init__(self, command=None, serial=None, src = None, dst = None):
        super().__init__(command, serial, src, dst)

    def FrameCheksumErrorTest(self, checksumValue=0):
        print("### CHECKSUM ERROR FRAME TEST ######")
        print("Attempt to send frame with invalid parameters")
        self.checksum = checksumValue
        return self

    def FrameSrcErrorTest(self, src=0):
        print("### SOURCE ADDRESS ERROR FRAME TEST ######")
        print("Attempt to send frame with invalid parameters")
        self.source = src
        return self

    def FrameDstErrorTest(self, dst=0):
        print("### DESTINATION ADDRESS ERROR FRAME TEST ######")
        print("Attempt to send frame with invalid parameters")
        self.destination = dst
        return self

    def FrameCustomCommandErrorTest(self, command=0):
        print("### UNKNOWN COMMAND FRAME TEST ######")
        print("Attempt to send frame with invalid parameters")
        self.command = command
        self.datacount = len(command)
        self.CalculateChecksum()
        return self

    def CheckResponse(self):
        if self.response == bytes("Frame Error", encoding="UTF=8"):
            print("ERROR FRAME PARAMETER TEST PASSED")
        elif self.response == bytes(f"{self.command}", encoding="UTF=8"):
            print("FRAME SEND AND RESPOSNE TEST PASSED")
        elif self.response == bytes(f"Unknown", encoding="UTF=8"):
            print("UNKNOWN SEND AND RESPOSNE TEST PASSED")
        else:
            print("TEST FAILED")
        print("---------------------------------")

if __name__ == "__main__":
    SerialHandler = serial.Serial(
        port="COM4",
        baudrate=115200)
    print("port settings: ", SerialHandler.baudrate)

    commands = ["D1", "D2", "I", "CLR"]

    for command in commands:
        FrameTests(command=command, serial=SerialHandler).SendAndResp().CheckResponse()

    FrameTests("D1", SerialHandler).FrameCheksumErrorTest(1177).SendAndResp().CheckResponse()
    FrameTests("D1", SerialHandler).FrameSrcErrorTest().SendAndResp().CheckResponse()
    FrameTests("D1", SerialHandler).FrameDstErrorTest().SendAndResp().CheckResponse()
    FrameTests("D1", SerialHandler).FrameCustomCommandErrorTest("READREG10000000000100").SendAndResp().CheckResponse()
    SerialHandler.close()