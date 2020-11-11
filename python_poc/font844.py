class Typewriter:
    cho_tbl = [
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0],
        [0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5]
    ]
    joong_tbl = [0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1]
    jong_tbl = [0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1]

    def __init__(self, fname):
        with open(fname, 'rb') as f:
            self._844font = f.read()

    @staticmethod
    def splitjamo(char):
        idx = ord(char) - 0xac00
        cho = idx // (21 * 28)
        joong = idx % (21 * 28) // 28
        jong = idx % 28
        return (cho, joong, jong)

    def render844(self,char):
        pix = [ [False for j in range(16)] for i in range(16)]
        cho, joong, jong = self.splitjamo(char)
        cho += 1
        joong += 1

        t = [
            self.cho_tbl[1 if jong else 0][joong]*20 + cho,
            8*20 + (self.joong_tbl[cho] + (2 if jong else 0)) * 22 + joong,
            8*20 + 4*22 + self.jong_tbl[joong]*28 + jong
        ]


        buf = bytearray(32)
        for fntidx in t:
            for i in range(32):
                buf[i] |= self._844font[fntidx*32 + i]
        return buf
