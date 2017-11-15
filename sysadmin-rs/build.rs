extern crate protoc_rust;

fn main() {
    protoc_rust::run(protoc_rust::Args {
        out_dir: "src",
        input: &["sysadmin-api/sysadminctl.proto"],
        includes: &["sysadmin-api"],
    }).expect("protoc");
}