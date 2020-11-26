import serial
import time
import sys

ser = serial.Serial('/dev/ttyACM0', 115200)
time.sleep(3)

f = open(sys.argv[1], 'rb')

frame = None
basetime = time.time()
cnt = 0
diff = 0

while (frame := f.read(4*16)):
    ser.write(b'\xff\x24')
    for i in range(16):
        ser.write(frame[i*4 + 3::-1][0:4])
    runtime = time.time() - basetime
    time.sleep(max(0.0333 - diff, 0))
    diff = runtime - cnt*0.0333
    print(diff ,end='\r')
    cnt += 1

