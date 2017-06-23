#!/usr/bin/env bash

FOLLY_VERSION="v2017.06.12.00"

git clone https://github.com/facebook/folly.git

pushd folly
git checkout $FOLLY_VERSION

# sed the script to pull glog from the right place
./folly/build/deps_ubuntu_12.04.sh
popd

echo "Done installing folly and its deps"

apt-get install liblog4cxx10-dev libprotobuf-dev

WORKDIR=${~/}
INSTALL_PREFIX=${2-/usr/local}
PATCHESDIR=${3~/}

# AMQP-CPP
AMQP_CPP_VERSION="2.6.2"
AMQP_CPP_DIR_NAME="AMQP-CPP-${AMQP_CPP_VERSION}"
AMQP_CPP_ARCHIVE_NAME="v${AMQP_CPP_VERSION}.tar.gz"
AMQP_CPP_URL="https://github.com/CopernicaMarketingSoftware/AMQP-CPP/archive/v${AMQP_CPP_VERSION}.tar.gz"

if [ ! -e "${INSTALL_PREFIX}/lib/libamqpcpp.so" ]; then
    if [ ! -e "${WORKDIR}/${AMQP_CPP_ARCHIVE_NAME}" ]; then
        mkdir -p "${WORKDIR}/download"
        wget -q -P "${WORKDIR}/download" "${AMQP_CPP_URL}"
        mv "${WORKDIR}/download/${AMQP_CPP_ARCHIVE_NAME}" "${WORKDIR}"
    fi
    tar -C "${WORKDIR}" -zxf "${WORKDIR}/${AMQP_CPP_ARCHIVE_NAME}"
    pushd "${WORKDIR}/${AMQP_CPP_DIR_NAME}"
    ${MAKE}
    ${MAKE} install PREFIX="${INSTALL_PREFIX}"
    ldconfig
    popd
fi

# libuv
LIBUV_VERSION="1.6.1"
LIBUV_DIR_NAME="libuv-v${LIBUV_VERSION}"
LIBUV_ARCHIVE_NAME="libuv-v${LIBUV_VERSION}.tar.gz"
LIBUV_URL="http://dist.libuv.org/dist/v${LIBUV_VERSION}/${LIBUV_ARCHIVE_NAME}"
if [ ! -e "${INSTALL_PREFIX}/lib64/libuv.so" ] && [ ! -e "${INSTALL_PREFIX}/lib/libuv.so" ]; then
    if [ ! -e "${WORKDIR}/${LIBUV_ARCHIVE_NAME}" ]; then
        mkdir -p "${WORKDIR}/download"
        wget -q -P "${WORKDIR}/download" "${LIBUV_URL}"
        mv "${WORKDIR}/download/${LIBUV_ARCHIVE_NAME}" "${WORKDIR}"
    fi
    tar -C "${WORKDIR}" -zxf "${WORKDIR}/${LIBUV_ARCHIVE_NAME}"
    pushd "${WORKDIR}/${LIBUV_DIR_NAME}"
    sh autogen.sh
    ./configure --prefix="${INSTALL_PREFIX}"
    ${MAKE}
    ${MAKE} install
    popd
fi

echo "Finished libuv"

# yaml-cpp
YAML_CPP_VERSION="0.5.2"
YAML_CPP_DIR_NAME="yaml-cpp-release-${YAML_CPP_VERSION}"
YAML_CPP_ARCHIVE_NAME="release-${YAML_CPP_VERSION}.tar.gz"
YAML_CPP_URL="https://github.com/jbeder/yaml-cpp/archive/${YAML_CPP_ARCHIVE_NAME}"

if [ ! -e "${INSTALL_PREFIX}/lib/libyaml-cpp.a" ]; then
    if [ ! -e "${WORKDIR}/${YAML_CPP_ARCHIVE_NAME}" ]; then
        mkdir -p "${WORKDIR}/download"
        wget -q -P "${WORKDIR}/download" "${YAML_CPP_URL}"
        mv "${WORKDIR}/download/${YAML_CPP_ARCHIVE_NAME}" "${WORKDIR}"
    fi
    tar -C "${WORKDIR}" -zxf "${WORKDIR}/${YAML_CPP_ARCHIVE_NAME}"
    pushd "${WORKDIR}/${YAML_CPP_DIR_NAME}"
    mkdir -p patches
    cp ${PATCHESDIR}/patches/yaml-cpp/* patches/
    git apply patches/*
    cmake -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" .
    ${MAKE}
    ${MAKE} install
    popd
fi

echo "Finished dependencies"
