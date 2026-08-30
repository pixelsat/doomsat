import subprocess
from collections import Counter

result = subprocess.run(
    ["meson", "compile", "-C", "doom/build/stm"],
    check=False,
    stdout=subprocess.PIPE,
    text=True,
)

map = {}

total = 0

def try_find_symbol(line):
    patterns = (
        "error: unknown type name '",
        "error: call to undeclared function '",
        "error: use of undeclared identifier '",
    )
    for pattern in patterns:
        if pattern in line:
            return line.split(pattern, 1)[1].split("'", 1)[0]
    return None


for line in result.stdout.splitlines():
    if "error:" in line:
        fi = line.split(": error:")[0].lstrip("../").split(':')[0]  # noqa: B005
        symbol = try_find_symbol(line)
        if symbol is None:
            continue
        total += 1
        if symbol not in map:
            map[symbol] = []
        map[symbol].append(fi)

print(f"{total} errors")
for symbol, files in map.items():
    print(f"{symbol}:")
    c = Counter(files)
    for file, count in c.items():
        print(f"  {file} (x{count})")
