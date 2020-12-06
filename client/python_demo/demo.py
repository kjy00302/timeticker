 
import serial
import time
import struct
import timeticker

ser = serial.Serial('/dev/ttyACM0', 115200)
time.sleep(2)

print("현재 시연: 기본적 시간 출력")
client = timeticker.TimeTickerClient(ser)
client.setflag(0b00110011)
input("대기 중...")


print("현재 시연: 한글 표현")
client.setflag(0b01110011)
input("대기 중...")


print("현재 시연: 사용자 문구")
client.settext("사용자 문구 설정도 돼요 ^^")
input("대기 중...")


print("현재 시연: 텍스트 스크롤러")
client.setflag(0b00000001)
client.clear()
client.setscrollspeed(8)
TEXT = "김수한무 거북이와 두루미 삼천갑자 동방삭 치치카포 사리사리센타 워리워리 세브리깡 무두셀라 구름이 허리케인에 담벼락 담벼락에 서생원 서생원에 고양이 고양이엔 바둑이 바둑이는 돌돌이"
i = 0
while (i < len(TEXT) and TEXT != ''):
    client.sendtext(TEXT[i:i+30])
    i += 30
input("대기 중...")


print("현재 시연: 프레임 전송")
image = bytes([
    0b00000000, 0b00000001, 0b10000000, 0b00010000,
    0b00001100, 0b00000010, 0b01110001, 0b11010000,
    0b00010011, 0b10000100, 0b00001000, 0b00001000,
    0b00100000, 0b01000011, 0b11110000, 0b00000100,
    0b00011111, 0b10000000, 0b00000000, 0b00010011,
    0b00000000, 0b00000000, 0b00000000, 0b00100000,
    0b00000000, 0b00000000, 0b00000000, 0b01000010,
    0b00000000, 0b00111000, 0b00000000, 0b00000010,
    0b00001100, 0b01000100, 0b00000000, 0b00000010,
    0b00010010, 0b10000010, 0b00000000, 0b00000000,
    0b00100001, 0b00000001, 0b10000011, 0b10000000,
    0b01000000, 0b10000000, 0b01110100, 0b01000000,
    0b10000000, 0b01000000, 0b00001000, 0b00110000,
    0b00000000, 0b00100000, 0b00010000, 0b00001000,
    0b00000000, 0b00000000, 0b00000000, 0b00000100,
    0b00000000, 0b00000000, 0b00000000, 0b00000010
    ])

client.setflag(0b00000000)
client.sendframe(image)
input("대기 중...")


print("현재 시연: 영상 재생")
f = open('apple32x16.bin', 'rb')

frame = None
basetime = time.time()
cnt = 0
diff = 0

while (frame := f.read(4*16)):
    client.sendframe(frame)
    runtime = time.time() - basetime
    time.sleep(max(0.0333 - diff, 0))
    diff = runtime - cnt*0.0333
    print(diff ,end='\r')
    cnt += 1
