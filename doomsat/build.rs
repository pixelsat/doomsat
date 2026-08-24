use std::path::PathBuf;

fn main() {
    let lib_dir = PathBuf::from("../doom/build").canonicalize().unwrap();

    println!("cargo:rerun-if-changed=../doom");
    println!("cargo:rustc-link-search=native={}", lib_dir.display());
    println!("cargo:rustc-link-lib=static=doomsat");
    println!("cargo:rustc-link-lib=m");
}
