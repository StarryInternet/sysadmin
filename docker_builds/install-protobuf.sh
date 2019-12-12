#!/usr/bin/env bash
# Copyright 2019. Starry, Inc. All Rights Reserved.
set -ex

CPU_COUNT=$(nproc)
MAKE="make -j${CPU_COUNT}"

WORKDIR=${1-/var/tmp/third_party_build/tmp}
INSTALL_PREFIX=${2-/usr/local}
mkdir -p "${WORKDIR}"

if [ -z "${FORCE_REINSTALL}" ]; then
    FORCE_REINSTALL=0
fi

# protobuf
PROTOBUF_VERSION="3.8.0"

if [[ "${PROTOBUF_VERSION}" =~ ^3.* ]]; then
    PROTOBUF_ARCHIVE_NAME="protobuf-cpp-${PROTOBUF_VERSION}.tar.gz"
else
    PROTOBUF_ARCHIVE_NAME="protobuf-${PROTOBUF_VERSION}.tar.gz"
fi

PROTOBUF_DIR_NAME="protobuf-${PROTOBUF_VERSION}"
PROTOBUF_URL="https://github.com/protocolbuffers/protobuf/releases/download/v${PROTOBUF_VERSION}/${PROTOBUF_ARCHIVE_NAME}"
if [ "${FORCE_REINSTALL}" -eq 1 ] || \
    [ ! -e "${INSTALL_PREFIX}/bin/protoc" ]
then
    if [ ! -e "${WORKDIR}/${PROTOBUF_ARCHIVE_NAME}" ]; then
        mkdir -p "${WORKDIR}/download"
        wget -q -P "${WORKDIR}/download" "${PROTOBUF_URL}"
        mv "${WORKDIR}/download/${PROTOBUF_ARCHIVE_NAME}" "${WORKDIR}"
    fi
    tar -C "${WORKDIR}" -zxvf "${WORKDIR}/${PROTOBUF_ARCHIVE_NAME}"
    pushd "${WORKDIR}/${PROTOBUF_DIR_NAME}"
    ./configure --prefix="${INSTALL_PREFIX}"
    ${MAKE}
    ${MAKE} install
    ldconfig
    popd
    rm -rf "${WORKDIR?}"
fi
