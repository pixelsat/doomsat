use std::path::PathBuf;

fn main() {
    let lib_dir = PathBuf::from("../doomstm/build").canonicalize().unwrap();

    println!("cargo:rustc-link-search=native={}", lib_dir.display());
    println!("cargo:rustc-link-lib=static=doomstm");
    println!("cargo:rustc-link-lib=m");
}
