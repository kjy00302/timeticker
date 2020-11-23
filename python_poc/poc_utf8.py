import io

t = io.BytesIO('abcèéêë가나다'.encode())

def utf8decoder(stream):
    b = stream.read(1)[0]
    if b < 0x80:
        return b
    elif b < 0xdf:
        b <<= 6
        b += stream.read(1)[0] & 0x3f
        return b & 0x7ff
    elif b < 0xef:
        b <<= 6
        b += stream.read(1)[0] & 0x3f
        b <<= 6
        b += stream.read(1)[0] & 0x3f
        return b & 0xffff

for i in range(10):
    c = utf8decoder(t)
    print(f'{c} - {chr(c)}')
