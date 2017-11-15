FROM fedora:26

RUN dnf update -y && dnf install -y --best --allowerasing \
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
    which \
    git \
    autoconf \
    automake \
    gflags-devel \
    glog-devel \
    libtool \
    openssl-devel \
    libevent-devel \
    python \
    python-devel \
    python-pip \
    python-virtualenvwrapper \
    python3 \
    python3-virtualenv \
    sudo \
    libuv-devel && \
    dnf clean all && \
    mkdir /var/tmp/setup && mkdir /var/tmp/workdir && \
    WORKDIR="/var/tmp/workdir" && \
    INSTALL_PREFIX="/usr" && \
    AMQP_CPP_VERSION="2.6.2" && \
    AMQP_CPP_DIR_NAME="AMQP-CPP-${AMQP_CPP_VERSION}" && \
    AMQP_CPP_ARCHIVE_NAME="v${AMQP_CPP_VERSION}.tar.gz" && \
    AMQP_CPP_URL="https://github.com/CopernicaMarketingSoftware/AMQP-CPP/archive/v${AMQP_CPP_VERSION}.tar.gz" && \
    mkdir -p "${WORKDIR}/download" && \
    wget -q -P "${WORKDIR}/download" "${AMQP_CPP_URL}" && \
    mv "${WORKDIR}/download/${AMQP_CPP_ARCHIVE_NAME}" "${WORKDIR}" && \
    tar -C "${WORKDIR}" -zxf "${WORKDIR}/${AMQP_CPP_ARCHIVE_NAME}" && \
    pushd "${WORKDIR}/${AMQP_CPP_DIR_NAME}" && \
    /usr/bin/make && \
    /usr/bin/make install && \
    ldconfig && \
    popd && \
    FOLLY_VERSION="v2017.08.14.00" && \
    FOLLY_DIR_NAME="folly-${FOLLY_VERSION}" && \
    FOLLY_ARCHIVE_NAME="v${FOLLY_VERSION}.tar.gz" && \
    FOLLY_URL="https://github.com/facebook/folly/archive/v${FOLLY_VERSION}.tar.gz" && \
    mkdir -p "${WORKDIR}/folly" && \
    git clone https://github.com/facebook/folly.git "${WORKDIR}/folly" && \
    pushd "${WORKDIR}/folly" && \
    git checkout "${FOLLY_VERSION}" && \
    pushd folly && \
    autoreconf -ivf && \
    ./configure && \
    /usr/bin/make && \
    /usr/bin/make install && \
    popd && \
    popd && \
    rm -rf /var/tmp
