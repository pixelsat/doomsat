default:
    @just --list

# configure the meson build for libdoomsat
setup:
    meson setup doom/build/stm doom -Dmode=stm
    meson setup doom/build/client doom -Dmode=client

# build libdoomsat.a
build:
    meson compile -C doom/build/stm
    meson compile -C doom/build/client

# estimate peak ram for libdoomsat.a
size: build
    ./size.sh doom/build/stm/libdoomsat.a

client:
    cd doomsat && cargo run --bin client

server:
    cd doomsat && cargo run --bin server

check:
    cd doomsat && cargo check

fmt:
    cd doom && clang-format -i *.c *.h
    cd doomsat && cargo fmt

clean:
    rm -rf doom/build
    cd doomsat && cargo clean
