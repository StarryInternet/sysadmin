#!/bin/bash -xe

function show_help() {
	echo "Options:"
	echo "  -s, --simple-deps"
	echo "     Install all the fedora dependencies, generally via dnf"
	echo "  -b, --build-deps"
	echo "     Download, build, and install all the compile dependencies"
}

if [ $EUID -ne 0 ]; then
    echo "ERROR: You must run this script as root."
    exit 1
fi

WORKDIR=/var/tmp/fedora-bootstrap
INSTALL_PREFIX=/usr/local

function install_simple_deps() {
    dnf update -y

    dnf install -y --best --allowerasing \
        autoconf \
        automake \
        bash \
        bc \
        binutils \
        bison \
        byacc \
        bzip2 \
        bzip2-devel \
        cmake \
        cpio \
        cvs \
        doxygen \
        double-conversion-devel \
        expect \
        flex \
        gcc \
        gcc-c++ \
        gcc-c++.i686 \
        gdb \
        gettext \
        git \
        gflags-devel \
        glog-devel \
        golang \
        golang-googlecode-goprotobuf \
        gzip \
        libevent-devel \
        libffi-devel \
        libtool \
        libtool-ltdl-devel \
        make \
        mercurial \
        mtd-utils \
        mtd-utils-ubi \
        ncurses-devel \
        openssl \
        openssl-devel \
        patch \
        perl \
        perl-ExtUtils-MakeMaker \
        perl-Thread-Queue \
        perl-threads \
        procps-ng \
        python \
        python-devel \
        python2-virtualenv \
        python-virtualenvwrapper \
        python-pip \
        redhat-rpm-config \
        rsync \
        sqlite-devel \
        strace \
        sudo \
        texinfo \
        tmux \
        unzip \
        valgrind \
        vim \
        wget \
        which \
        xz \
        zsh

cat << EOF > /etc/ld.so.conf.d/usr_local.conf
/usr/local/lib
/usr/local/lib64
EOF

    wget ftp://195.220.108.108/linux/fedora/linux/releases/23/Everything/x86_64/os/Packages/l/log4cxx-0.10.0-19.fc23.x86_64.rpm
    wget ftp://195.220.108.108/linux/fedora/linux/releases/23/Everything/x86_64/os/Packages/l/log4cxx-devel-0.10.0-19.fc23.x86_64.rpm
    dnf install -y log4cxx-0.10.0-19.fc23.x86_64.rpm log4cxx-devel-0.10.0-19.fc23.x86_64.rpm
    rm log4cxx-0.10.0-19.fc23.x86_64.rpm log4cxx-devel-0.10.0-19.fc23.x86_64.rpm

    pip install cpplint
}

function build_deps() {
    mkdir -p "${WORKDIR}/src"
    mkdir -p "${WORKDIR}/tmp"

    cp -r patches "${WORKDIR}/src"

    sh third-party-build.sh "${WORKDIR}/tmp" "$INSTALL_PREFIX" "${WORKDIR}/src"

    rm -rf "$WORKDIR"

    ldconfig
}

BOOTSTRAP_STAGE="simple-deps"

while [ "x${1}" != "x" ]; do
    case "${1}" in
        "-s" | "--simple-deps")
            BOOTSTRAP_STAGE="simple-deps"
            shift
            ;;
        "-b" | "--build-deps")
            BOOTSTRAP_STAGE="build-deps"
            shift
            ;;
        "-d" | "--debug")
            set -x
            shift
            ;;
        "-h" | "--help")
            show_help
            exit 0
            ;;
        *)
            echo "ERROR: Unrecognized option ${1}"
            exit 1
            ;;
    esac
done

case "${BOOTSTRAP_STAGE}" in
    "simple-deps")
		install_simple_deps
        ;;
    "build-deps")
		build_deps
        ;;
    *)
        echo "ERROR: Unrecognized bootstrap stage: ${BOOTSTRAP_STAGE}"
        exit 1
        ;;
esac
