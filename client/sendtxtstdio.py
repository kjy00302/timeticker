import serial
import struct
import time
import sys

ser = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(3)

def sendtxt(t):
    ser.write(b'\xff\x23')
    ser.write(struct.pack('B', len(t)))
    for c in t:
        ser.write(struct.pack('<H', ord(c)))
    assert ser.read(2) == b'\xff\x01'
    if ser.read(2) == b'\xff\x10':
        return True

while (TEXT := sys.stdin.readline()) != '':
    TEXT = TEXT.strip()
    i = 0
    while (i < len(TEXT) and TEXT != ''):
        sendtxt(TEXT[i:i+30])
        i += 30
