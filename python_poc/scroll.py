import time
import font844
import max7219arrsim

buf = bytearray(8*4*2)
newbuf = bytearray(8*4)

tw = font844.Typewriter('APPLE.HAN')

t = 0

def tonewdispbuf(txt):
    for y in range(16):
        newbuf[(y//8)*16 + (y%8)] |= txt[y*2]
        newbuf[(y//8)*16 + 8+(y%8)] |= txt[y*2+1]

def shift():
    for i in range(8):
        buf[i+0] = (buf[i+0] << 1 | buf[i+8] >> 7) & 0xff
        buf[i+8] = (buf[i+8] << 1  | buf[i+16] >> 7) & 0xff
        buf[i+16] = (buf[i+16] << 1  | buf[i+24] >> 7) & 0xff
        buf[i+24] = (buf[i+24] << 1  | newbuf[i+0] >> 7) & 0xff
        newbuf[i+0] = (newbuf[i+0] << 1  | newbuf[i+8] >> 7) & 0xff
        newbuf[i+8] = (newbuf[i+8] << 1) & 0xff
        buf[i+32] = (buf[i+32] << 1 | buf[i+40] >> 7) & 0xff
        buf[i+40] = (buf[i+40] << 1  | buf[i+48] >> 7) & 0xff
        buf[i+48] = (buf[i+48] << 1  | buf[i+56] >> 7) & 0xff
        buf[i+56] = (buf[i+56] << 1  | newbuf[i+16] >> 7) & 0xff
        newbuf[i+16] = (newbuf[i+16] << 1  | newbuf[i+24] >> 7) & 0xff
        newbuf[i+24] = (newbuf[i+24] << 1) & 0xff


BUFSIZE = 32
txtbuff = [''] * BUFSIZE

txtbuff_front = 0
txtbuff_end = 0

def isbufferempty():
    global txtbuff_front, txtbuff_end
    if txtbuff_end == txtbuff_front:
        return True
    return False

def enqueuebuffer(c):
    global txtbuff_front, txtbuff_end
    if (txtbuff_front+1)%BUFSIZE != txtbuff_end:
        txtbuff[txtbuff_front] = c
        txtbuff_front = (txtbuff_front + 1)%BUFSIZE

def dequeuebuffer():
    global txtbuff_front, txtbuff_end
    if txtbuff_end != txtbuff_front:
        t = txtbuff[txtbuff_end]
        txtbuff_end = (txtbuff_end+1)%BUFSIZE
        return t

NUMH = '열두,한,두,세,네,다섯,여섯,일곱,여덟,아홉,열,열한'.split(',')
NUMM = ',일,이,삼,사,오,육,칠,팔,구'.split(',')

def timetohan():
    t = []
    H = time.localtime().tm_hour
    M = time.localtime().tm_min
    t.append(NUMH[H%12])
    t.append('시')
    if M == 0:
        t.append('정각')
    elif M < 10:
        t.append(NUMM[M])
        t.append('분')
    else:
        t.append(NUMM[M//10] if M//10 != 1 else '')
        t.append('십')
        t.append(NUMM[M%10])
        t.append('분')
    return ''.join(t)

isarmed = True

buff_left = 0
print('\n'*16)
while True:
    if isarmed and (int(time.time()%30) == 0):
        isarmed = False
        for i in f'지금은{timetohan()}입니다':
            enqueuebuffer(i)
    elif (not isarmed) and (int(time.time()%30) != 0):
        isarmed = True
    
    if (buff_left == 0) and (not isbufferempty()):
        txt = tw.render844(dequeuebuffer())
        tonewdispbuf(txt)
        buff_left += 16
    print('\033[16A',end='')
    print('\033[31m', end='')
    max7219arrsim.display8(buf)
    print('\033[0m', end='')
    shift()
    time.sleep(0.06)
    if buff_left > 0:
        buff_left -= 1
