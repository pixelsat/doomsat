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

server:
    cd crates/doomstm && cargo run --example host

debug-server:
    cd crates/doomstm && cargo build --example host
    cd crates/doomstm && lldb -o 'settings set stop-line-count-before 2' -o 'settings set stop-line-count-after 3' -o 'process handle --stop true --notify true --pass true SIGSEGV SIGBUS SIGILL SIGFPE SIGABRT' -o run -o bt -- target/debug/examples/host

client:
    cd crates/doomclient && cargo run --example host

run:
    pnpm dlx concurrently -n "stm,client" "just server" "just client"

check:
    cd doomsat && cargo check

fmt:
    cd doom && clang-format -i *.c *.h
    cd doomsat && cargo fmt

clean:
    rm -rf doom/build
    cd doomsat && cargo clean
