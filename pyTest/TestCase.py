from ProtocolTest import ProtocolSender
import serial
from BColor import *
from Status import Status

class TestCase(ProtocolSender):
    def __init__(self, command="", serial=None, src = None, dst = None):
        super().__init__(command, serial, src, dst)
        self.expected_response = ""
        self.test_status = Status.FAIL

    def FrameCheksumErrorTest(self, checksumValue=0):
        print(f"{BOLD}### CHECKSUM ERROR FRAME TEST ######{ENDC}")
        print("Attempt to send frame with invalid parameters")
        self.checksum = checksumValue
        return self

    def FrameSrcErrorTest(self, src=0):
        print(f"{BOLD}### SOURCE ADDRESS ERROR FRAME TEST ######{ENDC}")
        print("Attempt to send frame with invalid parameters")
        self.source = src
        return self

    def FrameDstErrorTest(self, dst=0):
        print(f"{BOLD}### DESTINATION ADDRESS ERROR FRAME TEST ######{ENDC}")
        print("Attempt to send frame with invalid parameters")
        self.destination = dst
        return self

    def FrameCustomCommandErrorTest(self, command=0):
        print(f"{BOLD}### UNKNOWN COMMAND FRAME TEST ######{ENDC}")
        print("Attempt to send frame with invalid parameters")
        self.command = command
        self.datacount = len(command)
        self.CalculateChecksum()
        return self

    def FrameCustomMessage(self, msg=""):
        print(f"{BOLD}### CUSTOM FRAME FRAME TEST ######{ENDC}")
        print("Attempt to send frame with custom parameters")
        self.message = bytes(msg, "ascii")
        return self

    def CheckResponse(self, expected_resp="", comment=None):
        self.expected_response = bytes(expected_resp, encoding="UTF=8")
        if self.response == self.expected_response:
            print(f"{GREEN}### TEST PASSED #######{ENDC}")
            self.test_status = Status.PASS
        elif comment and len(self.response) > 0:
            print(f"{WARN}### TEST WARNING #######{ENDC}")
            self.test_status = Status.WARNING
        else:
            print(f"{RED}### TEST FAIL #######{ENDC}")
            self.test_status = Status.FAIL
        print(f"EXPECTED RESPONSE: {self.expected_response}")
        print(f"RECEIVED RESPONSE: {self.response}")
        if comment:
            print(f"{WARN}COMMENT: {comment}{ENDC}")
        print("---------------------------------")
        return self.test_status, comment

if __name__ == "__main__":
    print("### SOURCE ADDRESS ERROR FRAME TEST ######")
    SerialHandler = serial.Serial(
        port="COM4",
        baudrate=115200)
    print("Serial port settings: ", SerialHandler.baudrate)

    commands = ["D1", "D2", "I", "CLR"]

    while True:
        if Status.FAIL == TestCase("D1", SerialHandler).FrameCheksumErrorTest().SendAndResp().CheckResponse("Frame Error")[0]:
            break
        if Status.FAIL == TestCase("D1", SerialHandler).FrameSrcErrorTest().SendAndResp().CheckResponse("Frame Error")[0]:
            break
        if Status.FAIL == TestCase("D1", SerialHandler).FrameDstErrorTest().SendAndResp().CheckResponse("Frame Erro", comment="wrong expected resposne done intentionally")[0]:
            break
        if Status.FAIL == TestCase("D1", SerialHandler).FrameDstErrorTest().SendAndResp().CheckResponse("Frame Error")[0]:
            break
        if Status.FAIL == TestCase(serial=SerialHandler).FrameCustomCommandErrorTest("READREG10000000000100").SendAndResp().CheckResponse("Unknown")[0]:
            break
        if Status.FAIL == TestCase(serial=SerialHandler).FrameCustomCommandErrorTest("TO SA DANE RAMKI I RAMKA MOZE MIEC MAKSYMALNIE 99 ZNAKOW I TO JEST W SUMIE DUZO").SendAndResp().CheckResponse("Unknown")[0]:
            break
        if Status.FAIL == TestCase(serial=SerialHandler).FrameCustomMessage("$1203CLR112#").SendRawAndResp().CheckResponse("CLR")[0]:
            pass
        for command in commands:
            if Status.FAIL == TestCase(command=command, serial=SerialHandler).SendAndResp().CheckResponse(command)[0]:
                break
        if Status.FAIL == TestCase(serial=SerialHandler).FrameCustomMessage("$2702D1117#").SendRawAndResp().CheckResponse("D1")[0]:
            break
        if Status.FAIL == TestCase(serial=SerialHandler).FrameCustomMessage("$2702D1118#").SendRawAndResp().CheckResponse("D1", comment="custom frame with wrong checksum")[0]:
            break
        if Status.FAIL == TestCase(serial=SerialHandler).FrameCustomMessage("$2702D1117#").SendRawAndResp().CheckResponse("D2", comment="wrong expected resposne done intentionally")[0]:
            break
        TestCase(serial=SerialHandler).FrameCustomMessage("$2702D1118#").SendRawAndResp()


    SerialHandler.close()