"""Fake doomclient: connects to the stm host socket, drains state frames,
and scripts key presses to start a new game and play for a bit."""

import socket
import struct
import sys
import threading
import time

SOCK = "/tmp/doomsat.sock"

KEY_ESCAPE = 27
KEY_ENTER = 13
KEY_TAB = 9
KEY_UP = 0xAD
KEY_RIGHT = 0xAE
KEY_FIRE = 0xA3
KEY_USE = 0xA2

frames = 0
frames_lock = threading.Lock()


def reader(s):
    global frames
    while True:
        hdr = b""
        while len(hdr) < 4:
            chunk = s.recv(4 - len(hdr))
            if not chunk:
                return
            hdr += chunk
        (length,) = struct.unpack("<I", hdr)
        remaining = length
        while remaining:
            chunk = s.recv(min(65536, remaining))
            if not chunk:
                return
            remaining -= len(chunk)
        with frames_lock:
            frames += 1


def key(s, code, hold=0.1):
    s.sendall(bytes([1, code]))
    time.sleep(hold)
    s.sendall(bytes([0, code]))
    time.sleep(0.15)


def frame_count():
    with frames_lock:
        return frames


def main():
    deadline = time.time() + 10
    while True:
        try:
            s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            s.connect(SOCK)
            break
        except OSError:
            if time.time() > deadline:
                print("FAIL: could not connect", flush=True)
                sys.exit(1)
            time.sleep(0.2)

    t = threading.Thread(target=reader, args=(s,), daemon=True)
    t.start()

    time.sleep(2.0)  # let the demo loop spin up
    n0 = frame_count()

    # demo screen -> main menu -> new game -> episode 1 -> skill -> play
    key(s, KEY_ESCAPE)
    for _ in range(4):
        key(s, KEY_ENTER, hold=0.15)
    time.sleep(1.0)

    # walk forward, turn, use, fire, toggle automap
    s.sendall(bytes([1, KEY_UP]))
    time.sleep(1.5)
    s.sendall(bytes([0, KEY_UP]))
    key(s, KEY_RIGHT, hold=0.5)
    key(s, KEY_USE)
    for _ in range(3):
        key(s, KEY_FIRE, hold=0.2)
    key(s, KEY_TAB)
    time.sleep(0.5)
    key(s, KEY_TAB)

    # ensure frames are still flowing after all that
    n1 = frame_count()
    time.sleep(2.0)
    n2 = frame_count()

    print(f"frames: after-demo={n0} after-input={n1} final={n2}", flush=True)
    if n0 < 10:
        print("FAIL: no frames during demo", flush=True)
        sys.exit(1)
    if n2 <= n1:
        print("FAIL: frames stopped after input", flush=True)
        sys.exit(1)
    print("PASS", flush=True)


if __name__ == "__main__":
    main()
