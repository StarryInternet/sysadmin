#!/usr/bin/env bash

FOLLY_VERSION="v2017.06.12.00"

git clone https://github.com/facebook/folly.git

pushd folly
git checkout $FOLLY_VERSION
./folly/build/deps_ubuntu_12.04.sh
popd

echo "Done installing folly and its deps"

apt-get install liblog4cxx10-dev libuv-dev libprotobuf-dev libyaml-cpp-dev

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
