#!/bin/bash

export IMAGE_NAME='starryoss/sysadmin-build'
export NEW_TAG=":${2}"

show_help() {
cat << EOF
Usage: ${0##*/} [-dbtish*]
 -h          display this help and exit
 -d          build dependency image
 -b          build the user image from the dependency image
 -t          build and test. Non-interactive
 -i          run container interactively
 -s          start local sysadmin
EOF
}

while getopts ":dbtish*:" opt; do
  case $opt in
    d)  docker build -t "${IMAGE_NAME}${NEW_TAG}" docker_builds/
        exit $?
      ;;

    b)  docker pull "${IMAGE_NAME}"  # pull Dockerhub image
        docker build --cache-from="${IMAGE_NAME}" -t "${IMAGE_NAME}:latest" docker_builds/  # build using DH image as cache
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
    i)  docker run -it --rm --net=host -v `pwd`:/home/user/sysadmin -u user \
          --workdir /home/user/sysadmin sysadmin_tester /bin/bash
        exit $?
      ;;

    s)  docker run -it --rm --net=host -v `pwd`:/home/user/sysadmin -u user \
          --workdir /home/user/sysadmin sysadmin_tester \
          bash -i -c 'eval ./build/src/sysadmin ./configs/local/config.yaml'
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
