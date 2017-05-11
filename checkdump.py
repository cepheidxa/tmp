#!/usr/bin/env python3



class Dump:
    def __init__(self, filename):
        self.__file = filename
    def getCharacter(self, outfile):
        fd = open(self.__file, 'rb')
        ofd = open(outfile, 'w')
        contents = b'a'
        while len(contents) > 0:
            contents = fd.read(8192)
            contents = filter(lambda x: ((x >= ord('a') and x <= ord('z')) or (x >= ord('A') and x <= ord('Z')) or (x >= ord('0') and x <= ord('9'))), contents)
            contents = "".join(map(chr, contents))
            ofd.write(contents)
        fd.close()
        ofd.close()


if __name__ == "__main__":
    dump = Dump("rfc.xml")
    dump.getCharacter("a.txt")
