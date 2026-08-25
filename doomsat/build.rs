use std::{
    env, fs,
    path::{Path, PathBuf},
    process::Command,
};

#[derive(Debug)]
struct SerdeDerives;

impl bindgen::callbacks::ParseCallbacks for SerdeDerives {
    fn add_derives(&self, info: &bindgen::callbacks::DeriveInfo<'_>) -> Vec<String> {
        match info.name {
            "doomsat_mobj" | "doomsat_sector" | "doomsat_side" | "doomsat_line"
            | "doomsat_psprite" => {
                vec!["serde::Serialize".into(), "serde::Deserialize".into()]
            }
            _ => vec![],
        }
    }
}

fn main() {
    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    let doom_dir = manifest_dir.join("../doom").canonicalize().unwrap();
    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap());
    let meson_build_dir = out_dir.join("doom-build");

    track_doom_sources(&doom_dir);

    let meson = env::var_os("MESON").unwrap_or_else(|| "meson".into());
    println!("cargo:rerun-if-env-changed=MESON");

    if !meson_build_dir.join("meson-private/coredata.dat").exists() {
        run(Command::new(&meson)
            .arg("setup")
            .arg(&meson_build_dir)
            .arg(&doom_dir));
    }

    run(Command::new(&meson)
        .arg("compile")
        .arg("-C")
        .arg(&meson_build_dir));

    let bindings = bindgen::Builder::default()
        .header(doom_dir.join("doomgeneric.h").display().to_string())
        .header(doom_dir.join("doomsat.h").display().to_string())
        .allowlist_type(r"^(pixel_t|doomsat_.*)$")
        .allowlist_function(r"^(doomgeneric_(Create|Tick)|doomsat_GetState|doomsat_Draw)$")
        .allowlist_var(r"^DG_ScreenBuffer$")
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .parse_callbacks(Box::new(SerdeDerives))
        .clang_arg("-DDOOMSAT_DOOMSTM")
        .clang_arg("-DDOOMSAT_DOOMCLIENT")
        .generate()
        .expect("failed to generate Doom bindings");

    bindings
        .write_to_file(out_dir.join("bindings.rs"))
        .expect("failed to write Doom bindings");

    println!(
        "cargo:rustc-link-search=native={}",
        meson_build_dir.display()
    );
    println!("cargo:rustc-link-lib=static=doomsat");
    println!("cargo:rustc-link-lib=m");
}

fn track_doom_sources(directory: &Path) {
    for entry in fs::read_dir(directory).expect("failed to read Doom source directory") {
        let path = entry.expect("failed to read Doom source entry").path();

        if path.is_dir() {
            let name = path.file_name().and_then(|name| name.to_str());

            // Do not watch generated files or Git internals.
            if name != Some("build") && name != Some(".git") {
                track_doom_sources(&path);
            }

            continue;
        }

        let name = path.file_name().and_then(|name| name.to_str());
        let extension = path.extension().and_then(|extension| extension.to_str());

        if matches!(extension, Some("c" | "h" | "inc"))
            || matches!(name, Some("meson.build" | "meson_options.txt"))
        {
            println!("cargo:rerun-if-changed={}", path.display());
        }
    }
}

fn run(command: &mut Command) {
    let description = format!("{command:?}");
    let status = command
        .status()
        .unwrap_or_else(|error| panic!("failed to run {description}: {error}"));

    assert!(status.success(), "command failed: {description}");
}
