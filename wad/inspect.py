import io

class Lump:
    def __init__(self, name: str, size: int, loc: int, data: bytes):
        self.name = name
        self.size = size
        self.loc = loc
        self.data = data

class Wad:
    def __init__(self, file: io.BufferedReader):
        self.file = file
        if file.read(4) not in [b'IWAD', b'PWAD']:
            raise ValueError("Not a WAD file")
        self.numlumps = int.from_bytes(file.read(4), 'little')
        self.directory_loc = int.from_bytes(file.read(4), 'little')
        self.directory = None

    def build_directory(self):
        self.file.seek(self.directory_loc)
        self.directory = []
        for _ in range(self.numlumps):
            pos = int.from_bytes(self.file.read(4), 'little')
            size = int.from_bytes(self.file.read(4), 'little')
            name = self.file.read(8).decode('utf-8')
            print(name)
            self.directory.append(Lump(name, size, pos, b''))
        for item in self.directory:
            self.file.seek(item.loc)
            item.data = self.file.read(item.size)

with open('doom1.wad', 'rb') as f:
    wad = Wad(f)
    print(wad.numlumps, wad.directory_loc)
    wad.build_directory()
    if wad.directory is not None:
        for item in wad.directory:
            print(item.name)

