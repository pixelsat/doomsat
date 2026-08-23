use std::path::PathBuf;

fn main() {
    let lib_dir = PathBuf::from("../doomclient/build").canonicalize().unwrap();

    println!("cargo:rustc-link-search=native={}", lib_dir.display());
    println!("cargo:rustc-link-lib=static=doomclient");
    println!("cargo:rustc-link-lib=m");
}
