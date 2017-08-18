FROM fedora:26

RUN mkdir /var/tmp/setup && mkdir /var/tmp/workdir
COPY docker-setup.sh /var/tmp/setup/docker-setup.sh
RUN /var/tmp/setup/docker-setup.sh
