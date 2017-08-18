#!/usr/bin/env bash

WORKDIR="/var/tmp/workdir"
INSTALL_PREFIX="/usr"
MAKE="/usr/bin/make"

function build_amqp {
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
      ${MAKE} install
      ldconfig
      popd
  fi
}

function build_folly {
  FOLLY_VERSION="v2017.08.14.00"
  FOLLY_DIR_NAME="folly-${FOLLY_VERSION}"
  FOLLY_ARCHIVE_NAME="v${FOLLY_VERSION}.tar.gz"
  FOLLY_URL="https://github.com/facebook/folly/archive/v${FOLLY_VERSION}.tar.gz"

  if [ ! -e "${INSTALL_PREFIX}/lib64/libfolly.so" ] && [ ! -e "${INSTALL_PREFIX}/lib/libfolly.so" ]; then
      if [ ! -e "${WORKDIR}/folly" ]; then
          mkdir -p "${WORKDIR}/folly"
          git clone https://github.com/facebook/folly.git "${WORKDIR}/folly"
      fi
      pushd "${WORKDIR}/folly"
      git checkout "${FOLLY_VERSION}"
      pushd folly
      autoreconf -ivf
      ./configure
      ${MAKE}
      ${MAKE} install
      popd
      popd
  fi
}

dnf update -y

dnf install -y --best --allowerasing \
  bash \
  boost \
  boost-devel \
  cmake \
  double-conversion \
  double-conversion-devel \
  gcc-c++ \
  python \
  make \
  protobuf \
  protobuf-devel \
  protobuf-python \
  log4cxx \
  log4cxx-devel \
  yaml-cpp \
  yaml-cpp-devel \
  wget \
  git \
  autoconf \
  automake \
  gflags-devel \
  glog-devel \
  libtool \
  openssl-devel \
  libevent-devel \
  libuv-devel


build_amqp
build_folly
