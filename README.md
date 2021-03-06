# sysadmin

[![Build Status](https://travis-ci.org/StarryInternet/sysadmin.svg?branch=master)](https://travis-ci.org/StarryInternet/sysadmin)

Like system administrators in big companies, `sysadmin` configures things

## About

sysadmin's main goal is to be a system configuration manager. It accomplishes
that by providing a typed key value database, on which you the user can create
"hooks".  These hooks allow you to do one or both of the following things, in
response to keys or groups of keys changing their values:

1. Render jinja2 based templates and save them to the system
2. Run arbitrary scripts

For example, say you have a set of keys whose values reflect DHCP settings:

```bash
network.dhcp.endip = "192.168.99.200"
network.dhcp.lease = 720
network.dhcp.router_ip = "192.168.99.1"
network.dhcp.router_netmask = "255.255.255.0"
network.dhcp.startip = "192.168.99.100"
network.dhcp.static_assignments = []
```

Two hooks exist to service these keys:

1. A jinja2 template which renders a [dnsmasq](http://www.thekelleys.org.uk/dnsmasq/doc.html)
   configuration file which reflects the current values
2. A simple redirection script which knows to restart the dnsmasq service
   via systemd

sysadmin allows you to change the value of one or more of these keys, then
`commit` them all at once, at which point the 2 hooks above are run. From a
user's perspective, they need only know what values they want, and sysadmin
takes care of the messy, system level details via its hooks.

## Dependencies

sysadmin's dependencies are codified in its `CMakeLists.txt` files, but the key
ones are as follows:

- decibel-cpp, which is included with sysadmin and built when sysadmin is
  built. It is a C++ wrapper around libuv, and additional abstractions built
  with folly
- [libuv](https://github.com/libuv/libuv) is a C event loop library
- [boost](http://www.boost.org/) is boost
- [folly](https://github.com/facebook/folly) is a C++ library, primarily
  used for its excellent futures code
- [protobufs](https://developers.google.com/protocol-buffers/) provides the API
  to sysadmin
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) provides YAML config file
  support

## Systemd Integration

sysadmin can integrate with systemd as a `notify` type service which can
prevent races between other services that rely on sysadmin for configuration.
In order to build sysadmin with systemd notify support you need to pass
`-DSYSADMIN_USE_SD_NOTIFY` during step 3 outlined below. When building sysadmin
in this configuration `libsystemd` becomes a required dependency.

See [sysadmin-notify.service](configs/prod/sysadmin-notify.service) for a
systemd service definition that incorporates this functionality.

## Development

Generally speaking, build as follows:

```bash
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make check
make
```

`make check` runs only sysadmin's tests. If you wish to run the `decibel-cpp`
tests, run `make decibel-check`.

## Dockerized Development

For consistency it's recommended to build and test in Docker. We will try to
keep the dependencies in the image on [Dockerhub](https://hub.docker.com/r/starryoss/sysadmin-build/)
up to date.

The docker commands are wrapped in the `docker_control.sh` script.

```bash
# Make sure you have submodules
git submodule update --init --recursive

# Pull the dependency image and build the user image.
./docker_control.sh -b

# You should not need to do this, but if you really want
# to build the dependency image you can with:
./docker_control.sh -d latest
```

This pulls the dependency image and creates a "user" image for running builds.
To build Sysadmin inside the container you will start a container with the
code mounted in. Note that you run as a non-root user with your host UID
inside the container so you shouldn't have to worry about permissions on
files you create.

```bash
# build and test in a single command. Non-interactive
./docker_control.sh -t

# Run the container interactively
./docker_control.sh -i
```

## Non-Dockerized Development

If you're on linux, the included `third-party-build.sh` will pull and install
all the necessary dependencies.

On macOS, `brew` is generally able to install all the necessary libraries, at
least at the time of writing this. You'll at least need to do the following the
first time:

```bash
brew install folly # Folly is the key dependency, and brings boost with it
brew install amqp-cpp
brew install libuv
brew install yaml-cpp
brew install protobuf
```
