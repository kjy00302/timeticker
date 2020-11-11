
def displayone(byte):
    for y in range(8):
        for x in range(8):
            print('1' if byte[y] & (0x80>>x) else '0', end='')
        print()

def display8(byte):
    for y in range(8*2):
        for x in range(8*4):
            print('██' if byte[(y//8)*32 + y%8 + (x//8)*8] & (0x80>>(x%8)) else '__', end='')
        print()
