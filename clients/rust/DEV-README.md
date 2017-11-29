# Development

The `src/sysadminctl.rs` file is generated from `src/sysadminctl.proto` by
the build.rs [build script](http://doc.crates.io/build-script.html).
You may need the `protoc` on your path. See details
[here](https://github.com/stepancheg/rust-protobuf#how-to-generate-rust-code).

To experiment locally, start Sysadmin in one terminal
from the root dir of Sysadmin with:

`./docker_control.sh -s`

Note you may need to build Sysadmin first.

Next run tests (which builds the examples) and run the example with:

`cargo test && target/debug/examples/sysadmin-example`

