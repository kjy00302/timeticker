import PIL.Image
import os
import struct
import sys

frames = os.listdir(sys.argv[1])
frames.sort()

output = open('apple32x16.bin', 'wb')

for frame in frames:
    with PIL.Image.open(f'{sys.argv[1]}{frame}') as image:
        image = image.convert('L')
        for row in range(16):
            t = 0
            for col in range(20):
                if image.getpixel((col, row)) > 128:
                    t |= 0x02000000 >> col
            output.write(struct.pack('>I', t))

