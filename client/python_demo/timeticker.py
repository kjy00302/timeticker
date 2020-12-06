import struct

class TimeTickerClient:
    def __init__(self, serport):
        self._serport = serport

    def check(self):
        self._serport.write(b'\xff\x01')
        assert self._serport.read(2) == b'\xff\x01'

    def _check(self):
        assert self._serport.read(2) == b'\xff\x01'

    @staticmethod
    def texttoarray(string):
        return b''.join([struct.pack('<H', ord(i)) for i in string])

    def settext(self, string):
        if len(string) >= 32:
            return
        self._serport.write(b'\xff\x04' + struct.pack('B', len(string)) + self.texttoarray(string))
        self._check()

    def sendtext(self, string):
        if len(string) >= 32:
            return
        self._serport.write(b'\xff\x23' + struct.pack('B', len(string)) + self.texttoarray(string))
        self._check()
        self._serport.read(2)

    def setflag(self, flag):
        self._serport.write(b'\xff\x02' + struct.pack('B', flag))
        self._check()

    def clear(self):
        self._serport.write(b'\xff\x21')
        self._check()

    def sendframe(self, frame):
        self._serport.write(b'\xff\x24')
        for i in range(16):
            self._serport.write(frame[i*4 + 3::-1][0:4])
        self._check()

    def setscrollspeed(self, speed):
        self._serport.write(b'\xff\x25' + struct.pack('B', speed))
        self._check()
