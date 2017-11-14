#!/bin/bash

export IMAGE_NAME='starryoss/sysadmin-build'
export VERSION='latest'
export BASE_IMAGE="${IMAGE_NAME}:${VERSION}"

show_help() {
cat << EOF
Usage: ${0##*/} [-hbti]
 -h          display this help and exit
 -b          build images
 -t          build and test. Non-interactive
 -i          run container interactively
EOF
}

while getopts ":btih*:" opt; do
  case $opt in
    b)  docker pull "${BASE_IMAGE}" || true
        docker build -t "${IMAGE_NAME}" docker_builds/
        docker build --build-arg=USERID=`id -u` . -t sysadmin_tester
        exit $?
      ;;
    t)  docker run -t --rm -v `pwd`:/home/user/sysadmin -u user --workdir /home/user/sysadmin sysadmin_tester \
          /bin/bash -c \
          "rm -rf /home/user/sysadmin/build && \
           mkdir -p /home/user/sysadmin/build && \
           cd /home/user/sysadmin/build && \
           cmake .. && make check && make "
        exit $?
      ;;
    i)  docker run -it --rm -v `pwd`:/home/user/sysadmin -u user \
          --workdir /home/user/sysadmin sysadmin_tester /bin/bash
        exit $?
      ;;
    h)  show_help
        exit 0
      ;;
    *)  show_help
         exit 1
      ;;
  esac
done
show_help
exit 1
