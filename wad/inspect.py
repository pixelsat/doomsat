import io
import struct
import sys
from pathlib import Path


class Lump:
    def __init__(self, name: str, data: bytes):
        self.name = name
        self.data = data


class Wad:
    def __init__(self, file: io.BufferedReader):
        self.identification = file.read(4)
        if self.identification not in (b"IWAD", b"PWAD"):
            raise ValueError("Not a WAD file")

        self.numlumps = int.from_bytes(file.read(4), "little")
        self.directory_loc = int.from_bytes(file.read(4), "little")
        self.directory: list[Lump] = []

        file.seek(self.directory_loc)
        entries = []
        for _ in range(self.numlumps):
            pos = int.from_bytes(file.read(4), "little")
            size = int.from_bytes(file.read(4), "little")
            name = file.read(8).rstrip(b"\0").decode("ascii")
            entries.append((name, pos, size))

        for name, pos, size in entries:
            file.seek(pos)
            self.directory.append(Lump(name, file.read(size)))

    def prune_patches(self) -> int:
        in_patches = False
        bytes_removed = 0

        for lump in self.directory:
            if lump.name == "P_START":
                in_patches = True
                continue
            if lump.name == "P_END":
                in_patches = False
                continue
            if not in_patches or not lump.data:
                continue

            if len(lump.data) < 8:
                raise ValueError(f"Patch {lump.name} is too short")

            width = int.from_bytes(lump.data[:2], "little", signed=True)
            if width <= 0 or 8 + width * 4 > len(lump.data):
                raise ValueError(f"Patch {lump.name} has an invalid width")

            column_offset = 8 + width * 4
            stub = lump.data[:8]
            stub += struct.pack("<I", column_offset) * width
            stub += b"\xff"

            bytes_removed += len(lump.data) - len(stub)
            lump.data = stub

        if in_patches:
            raise ValueError("P_START has no matching P_END")

        return bytes_removed

    def write(self, file: io.BufferedWriter):
        payload = bytearray()
        entries = []

        for lump in self.directory:
            padding = -(12 + len(payload)) % 4
            payload.extend(b"\0" * padding)
            entries.append((12 + len(payload), len(lump.data), lump.name))
            payload.extend(lump.data)

        directory_loc = 12 + len(payload)
        file.write(self.identification)
        file.write(len(entries).to_bytes(4, "little"))
        file.write(directory_loc.to_bytes(4, "little"))
        file.write(payload)

        for pos, size, name in entries:
            file.write(pos.to_bytes(4, "little"))
            file.write(size.to_bytes(4, "little"))
            file.write(name.encode("ascii").ljust(8, b"\0"))

def main():
    if len(sys.argv) != 3:
        raise SystemExit(f"Usage: {sys.argv[0]} INPUT.wad OUTPUT.wad")

    input_path = Path(sys.argv[1])
    output_path = Path(sys.argv[2])

    with input_path.open("rb") as file:
        wad = Wad(file)

    bytes_removed = wad.prune_patches()

    with output_path.open("wb") as file:
        wad.write(file)

    print(
        f"Pruned {bytes_removed} bytes from patch lumps: "
        f"{input_path.stat().st_size} -> {output_path.stat().st_size} bytes"
    )


if __name__ == "__main__":
    main()
