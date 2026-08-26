build_dir := "doom/build"

default:
    @just --list

# configure the meson build for libdoomsat
setup:
    meson setup {{build_dir}} doom

# build libdoomsat.a
build:
    @[ -f {{build_dir}}/build.ninja ] || just setup
    meson compile -C {{build_dir}}

# estimate peak ram for libdoomsat.a
size: build
    ./size.sh {{build_dir}}/libdoomsat.a

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
    rm -rf {{build_dir}}
    cd doomsat && cargo clean
