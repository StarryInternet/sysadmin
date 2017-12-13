#!/bin/bash
set -e

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


NET_OPTS="--name sysadmin --network sysadmin -p 4000:4000"
VOL_OPTS="-v `pwd`:/home/user/sysadmin --workdir /home/user/sysadmin"

# try cleaning up sysadmin network. It fails if a container
# is using it and that is okay
docker network rm sysadmin > /dev/null 2>&1 || true
# make network for sysadmin
docker network inspect sysadmin > /dev/null 2>&1 || \
  docker network create --attachable  sysadmin

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
    t)  docker run -t --rm ${VOL_OPTS} ${NET_OPTS} -u user sysadmin_tester \
          /bin/bash -c \
          "rm -rf /home/user/sysadmin/build && \
           mkdir -p /home/user/sysadmin/build && \
           cd /home/user/sysadmin/build && \
           cmake .. && make check && make "
        exit $?
      ;;
    i)  docker run -it --rm ${NET_OPTS} ${VOL_OPTS} -u user sysadmin_tester \
          /bin/bash
        exit $?
      ;;

    s)  docker run -it --rm ${NET_OPTS} ${VOL_OPTS} -u user sysadmin_tester \
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
