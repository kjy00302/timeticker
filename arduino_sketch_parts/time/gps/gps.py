import struct

def nmea_chksum(msg): # NMEA checksum calculation
    assert msg[0] == "$"
    t = 0
    for i in msg[1:]:
        t ^= ord(i)
    return f'*{hex(t)[2:].upper()}'

def packubx(class_, id_, payload): # UBX packet packer
    body = struct.pack('<2BH', class_, id_, len(payload)) + payload
    ck_a = 0
    ck_b = 0
    for i in body:
        ck_a += i
        ck_b += ck_a
    return b'\xb5\x62' + body + struct.pack('BB', ck_a & 0xff, ck_b & 0xff)

# packubx(6, 4, b'\x00\x00\x00\x00').hex() # Reboot gps

data = [
    packubx(6, 1, b'\xf0\x05\x00'), # disable vtg
    packubx(6, 1, b'\xf0\x00\x00'), # disable gga
    packubx(6, 1, b'\xf0\x02\x00'), # disable gsa
    packubx(6, 1, b'\xf0\x03\x00'), # disable gsv
    packubx(6, 1, b'\xf0\x01\x00'), # disable gll
    packubx(6, 1, b'\xf0\x04\x00'), # disable rmc
    packubx(6, 1, b'\xf0\x08\x01'), # enable zda
]
print(list(b''.join(data)))
