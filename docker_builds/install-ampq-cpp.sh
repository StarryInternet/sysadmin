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

# AMQP-CPP
AMQP_CPP_VERSION="2.6.0"
AMQP_CPP_DIR_NAME="AMQP-CPP-${AMQP_CPP_VERSION}"
AMQP_CPP_ARCHIVE_NAME="v${AMQP_CPP_VERSION}.tar.gz"
AMQP_CPP_URL="https://github.com/CopernicaMarketingSoftware/AMQP-CPP/archive/v${AMQP_CPP_VERSION}.tar.gz"

if [ "${FORCE_REINSTALL}" -eq 1 ] || \
    [ ! -e "${INSTALL_PREFIX}/lib/libamqpcpp.so.${AMQP_CPP_VERSION}" ]
then
    if [ ! -e "${WORKDIR}/${AMQP_CPP_ARCHIVE_NAME}" ]; then
        mkdir -p "${WORKDIR}/download"
        wget -q -P "${WORKDIR}/download" "${AMQP_CPP_URL}"
        mv "${WORKDIR}/download/${AMQP_CPP_ARCHIVE_NAME}" "${WORKDIR}"
    fi
    tar -C "${WORKDIR}" -zxvf "${WORKDIR}/${AMQP_CPP_ARCHIVE_NAME}"
    pushd "${WORKDIR}/${AMQP_CPP_DIR_NAME}"
    ${MAKE}
    ${MAKE} install PREFIX="${INSTALL_PREFIX}"
    ldconfig
    popd
    rm -rf "${WORKDIR?}"
fi
