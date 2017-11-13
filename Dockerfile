FROM fedora:26

# https://bugzilla.redhat.com/show_bug.cgi?id=1483553
RUN dnf update -y libsolv 

RUN dnf distro-sync -y && dnf install -y \
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
    coreutils \
    cpio \
    cvs \
    double-conversion-devel \
    doxygen \
    expect \
    file \
    flex \
    gcc \
    gcc-c++ \
    gdb \
    gettext \
    git \
    gflags-devel \
    glog-devel \
    gzip \
    libevent-devel \
    libffi-devel \
    libtool \
    libtool-ltdl-devel \
    log4cxx-devel \
    make \
    mercurial \
    mtd-utils \
    mtd-utils-ubi \
    nc \
    ncurses-devel \
    nmap-ncat \
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
    python-pip \
    python-virtualenvwrapper \
    python3 \
    python3-virtualenv \
    redhat-rpm-config \
    sed \
    sqlite-devel \
    subversion \
    sudo \
    tar \
    texinfo \
    tmux \
    unzip \
    wget \
    which \
    ccache \
    ccache-swig \
    xz && \
    dnf autoremove -y && \
    dnf clean all

ENV HOME /home/user
RUN groupadd -g 1000 user
RUN useradd -c "user" -d $HOME -u 1000 -g 1000 -m user -s /bin/bash
# you shouldn't need to use sudo but we can add it.
RUN echo 'user ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers

RUN mkdir /tmp/sysadmin
COPY third-party-build.sh /tmp/sysadmin/
COPY patches  /tmp/sysadmin/patches
WORKDIR /tmp/sysadmin
RUN ./third-party-build.sh /tmp/sysadmin/workdir /usr /tmp/sysadmin/patches

USER user
WORKDIR /home/user

CMD /bin/bash
