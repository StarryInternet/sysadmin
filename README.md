sysadmin
========

Like system administrators in big companies, `sysadmin` configures things

About
=====

sysadmin's main goal is to be a system configuration manager. It accomplishes that
by providing a typed key value database, on which you the user can create "hooks".
These hooks allow you to do one or both of the following things, in response to
keys or groups of keys changing their values:
    1) Render jinja2 based templates and save them to the system
    2) Run arbitrary scripts

For example, say you have a set of keys whose values reflect dhcp settings:

```
network.dhcp.endip = "192.168.99.200"
network.dhcp.lease = 720
network.dhcp.router_ip = "192.168.99.1"
network.dhcp.router_netmask = "255.255.255.0"
network.dhcp.startip = "192.168.99.100"
network.dhcp.static_assignments = []
```

Two hooks exist to service these keys:
    1) A jinja2 template which renders a [dnsmasq](http://www.thekelleys.org.uk/dnsmasq/doc.html)
       configuration file which reflects the current values
    2) A simple redirection script which knows to restart the dnsmasq
       service via systemd.

sysadmin allows you to change the value of one or more of these keys, then `commit` them
all at once, at which point the 2 hooks above are run. From a user's perspective, they need
only know what values they want, and sysadmin takes care of the messy, system level details
via its hooks.

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

Systemd Integration
===================

sysadmin can integrate with systemd as a `notify` type service which can
prevent races between other services that rely on sysadmin for configuration.
In order to build sysadmin with systemd notify support you need to pass
`-DSYSADMIN_USE_SD_NOTIFY` during step 3 outlined below. When building sysadmin
in this configuration `libsystemd` becomes a required dependency.

There is an a systemd service definition which incorporates this functionality
located [here](configs/prod/sysadmin-notify.service).

Development
===========

If this is your first time, you might have some dependencies which need to be installed
first. You can do this in a docker image (recommended) or on your own system.

Generally speaking, build as follows:

1. git submodule update --init --recursive #  make sure you have submodules
2. mkdir build
3. cd build
4. cmake ..
5. make check
6. make

`make check` runs only sysadmin's tests. If you wish to run the decibel-cpp tests, run
`make decibel-check`.


Dockerized Development
======================

For consistancy it's recommended to build and test in Docker. Building the images takes
some time, but you only need to do it once. Build with:

```bash
git submodule update --init --recursive #  make sure you have submodules
docker build -t sysadmin .
```

This make an image that includes all the 3rd party dependencies. To build Sysadmin
inside the container you will start a container with the code mounted in.
Note that you run as a non-root user inside the container so you shouldn't
have to worry about permissions on files you create.

```bash
docker run -v ${PWD}:/home/user/sysadmin -it --rm sysadmin
cd sysadmin
mkdir build
cd build
cmake ..
make check
make
```

Non-Dockerized Development
==========================

If you're on linux, the included `third-party-build.sh` will pull and install all
the necessary dependencies.

On macOS, `brew` is generally able to install all the necessary
libraries, at least at the time of writing this. You'll at least need to do the following
the first time:
```bash
brew install folly # Folly is the key dependency, and brings boost with it
brew install amqp-cpp
brew install libuv
brew install yaml-cpp
brew install protobuf
```
