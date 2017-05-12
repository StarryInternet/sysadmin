sysadmin
========

Like system administrators in big companies, `sysadmin` configures things

Development
===========

Generally speaking, build as follows:

```
mkdir build
cd build
cmake ..
make check
make
```

`make check` runs only sysadmin's tests. If you wish to run the decibel-cpp tests, run
`make decibel-check`.

If this is your first time, you might have some dependencies which need to be installed
first. The included `third-party-build.sh` will download, build, then install those
dependencies. You will definitely need gcc, g++, and make at the very least in order
to build those dependencies.

Dependencies
============

sysadmin's dependencies are codified in its CMakeLists.txt files, but the key ones are
as follows:
    - decibel-cpp, which is included with sysadmin and built when sysadmin is built. It
      is a C++ wrapper around libuv, and additional abstractions built with folly
    - [libuv](https://github.com/libuv/libuv) is a C event loop library
    - [boost](http://www.boost.org/) is boost
    - [folly](https://github.com/facebook/folly) is a C++ library, primarily used for
      its excellent futures code
    - [protobufs](https://developers.google.com/protocol-buffers/), specifically proto 2,
      provides the API to sysadmin
    - [yaml-cpp](https://github.com/jbeder/yaml-cpp) provides YAML config file support

More docs are to come
