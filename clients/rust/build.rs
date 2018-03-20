extern crate protoc_rust;

// This is a hack. protoc_rust depends on "protoc" being installed on
// on the path. Without it, the docs.rs documentation build will fail
//
// As far as I can tell there is no way to know if "cargo doc"
// was called vs "cargo build".
//
// The profiles sections of Cargo.toml are supposed to help address this
// but they don't seem to take effect at this stage of the build.
// profiles sections: https://doc.rust-lang.org/cargo/reference/manifest.html#the-profile-sections
//
//
// This workaround allow us to catch a panic from "protoc_rust" if two conditions are met:
// 1. the systemctl.rs file must exist.
// 2. the current dir contains "cratesfyi" since docs.rs builds in /home/cratesfyi/cratesfyi
// https://docs.rs/crate/sysadmin-bindings/0.0.6/builds/82879
//
//  This will work
//  docker run --rm -it -u "$(id -u)":"$(id -g)" -v "$PWD/../../":/cratesfyi -w /cratesfyi/clients/rust rust cargo doc
//  This won't
//  docker run --rm -it -u "$(id -u)":"$(id -g)" -v "$PWD/../../":/myapp -w /myapp/clients/rust rust cargo doc
fn main() {
    use std::env;
    use std::fs;
    use std::panic;
    use std::path::PathBuf;

    let curdir_str = match env::current_dir() {
        Ok(p) => p.to_str().unwrap_or("").to_string(),
        Err(_) => "".to_string(),
    };
    let has_cratesfyi = curdir_str.contains("cratesfyi");

    let manifest_dir = env::var("CARGO_MANIFEST_DIR").unwrap_or(String::new());

    let mut pbuf_rs_file = PathBuf::from(&manifest_dir);
    pbuf_rs_file.push("src");
    pbuf_rs_file.push("sysadminctl.rs");

    let file_exists = match fs::metadata(&pbuf_rs_file) {
        Ok(f) => f.is_file(),
        Err(_) => false,
    };

    // None of this shows up in the logs unless there is a panic
    println!("current dir: {:?}", curdir_str);
    println!("\"cratesfyi\" found in path: {:?}", has_cratesfyi);
    println!("systemctl.rs path: {:?}", pbuf_rs_file);
    println!("systemctl.rs exists:  {:?}", file_exists);

    let args = protoc_rust::Args {
            out_dir: "src",
            input: &["src/sysadminctl.proto"],
            includes: &["src"],
    };

    let result = panic::catch_unwind(|| {
        protoc_rust::run(args).expect("protoc");
    });

    if !(has_cratesfyi && file_exists) {
        println!("This doesn't look like \"cargo doc\" so any \
            panics will be unwound.");
        if let Err(err) = result {
            println!("Panics encountered!");
            panic::resume_unwind(err);
        }
    }
}
