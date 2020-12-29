FROM fedora:31

COPY install-system-deps.sh /var/tmp
RUN /var/tmp/install-system-deps.sh

COPY install-protobuf.sh /var/tmp
RUN /var/tmp/install-protobuf.sh

COPY install-folly.sh /var/tmp
RUN /var/tmp/install-folly.sh

COPY install-ampq-cpp.sh /var/tmp
RUN /var/tmp/install-ampq-cpp.sh
