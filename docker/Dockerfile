FROM fedora:25

COPY third-party-build.sh bootstrap.sh /var/tmp/
COPY patches /var/tmp/patches

RUN cd /var/tmp; ./bootstrap.sh --simple-deps
RUN cd /var/tmp; ./bootstrap.sh --build-deps
