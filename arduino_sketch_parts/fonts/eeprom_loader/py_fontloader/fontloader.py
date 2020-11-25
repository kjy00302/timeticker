import serial
import time
import struct


ser = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(2)

ser.write(b'\xff\x01')
if ser.read(2) == b'\xff\x01':
    print("Comm OK")

def loaddata(filename, length, offset):
    print(f'Uploading {filename}')
    f = open(filename, 'rb')
    assert (offset % 16) == 0, "Offset is not aligned (offset % 16 != 0)"
    for i in range(length//16):
        ser.write(b'\xff\x32'+struct.pack('<H', offset+i*16)+f.read(16))
        assert ser.read(2) == b'\xff\x01'
        print(f'{i*16/length * 100:.2f}%', end='\r')
    print('100.00%')
    print("Done!")
    f.close()

loaddata('E3.ENG', 4096, 0)
loaddata('H04.HAN', 11520, 4096)
