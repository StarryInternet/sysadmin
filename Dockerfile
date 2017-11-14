FROM starryoss/sysadmin:2
# update this in docker_control.sh

# add user account that should make it easier to work on a synced directory
# in docker
# define USERID on command line or use default 1000
ARG USERID=1000
RUN groupadd -g "${USERID}" user
RUN useradd -c "user" -d /home/user -u "${USERID}" -g "${USERID}" -m user -s /bin/bash
# you shouldn't need to use sudo but we can add it.
RUN echo 'user ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers
