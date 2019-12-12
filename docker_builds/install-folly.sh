#!/usr/bin/env bash
# Copyright 2019. Starry, Inc. All Rights Reserved.
set -ex

CPU_COUNT=$(nproc)

WORKDIR=${1-/var/tmp/third_party_build/tmp}
INSTALL_PREFIX=${2-/usr/local}
SCRIPTDIR="$(cd "$(dirname "$0")" && pwd -P)"
mkdir -p "${WORKDIR}"

if [ -z "${FORCE_REINSTALL}" ]; then
    FORCE_REINSTALL=0
fi

# folly
FOLLY_VERSION="v2019.12.09.00"

# shellcheck disable=SC2166
if [ "${FORCE_REINSTALL}" -eq 1 ] || \
    [ ! -e "${INSTALL_PREFIX}/lib64/libfolly.so" -a ! -e "${INSTALL_PREFIX}/lib/libfolly.so" ]
then

    if [ ! -e "${WORKDIR}/folly" ]; then
        mkdir -p "${WORKDIR}/folly"
        git clone --branch "${FOLLY_VERSION}"  --depth=1 \
            https://github.com/facebook/folly.git "${WORKDIR}/folly"
    fi

    pushd "${WORKDIR}/folly"

    cp -r "${SCRIPTDIR}/patches/folly" patches
    git apply patches/*

    mkdir -p _build
    pushd _build

    cmake .. \
        -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
        -DBUILD_SHARED_LIBS=TRUE \
        -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE

    cmake --build . -j"${CPU_COUNT}"
    cmake --build . --target install
    ldconfig
    popd
    popd
    rm -rf "${WORKDIR?}"
fi
