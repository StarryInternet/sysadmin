#!/bin/bash
set -ex

PROJROOT=$(git rev-parse --show-toplevel)
export PROJROOT
cd "${PROJROOT}"

export IMAGE_NAME='starryoss/sysadmin-build'

if [ -n "${2}" ]; then
  export NEW_TAG=":${2}"
else
  export NEW_TAG=":latest"
fi

show_help() {
cat << EOF
Usage: ${0##*/} [-dbtisch*]
 -h          display this help and exit
 -d          build dependency image
 -b          build the user image from the dependency image
 -t          build and test. Non-interactive
 -i          run container interactively
 -s          start local sysadmin
 -c          clean Sysadmin tmp and history files
EOF
}

NET_OPTS="--name sysadmin --network sysadmin -p 4000:4000"
VOL_OPTS="-v ${PROJROOT}:/home/user/sysadmin:Z --workdir /home/user/sysadmin"

# try cleaning up sysadmin network. It fails if a container
# is using it and that is okay
docker network rm sysadmin > /dev/null 2>&1 || true

# make docker network for sysadmin
docker network inspect sysadmin > /dev/null 2>&1 || \
  docker network create --attachable  sysadmin

function clean_state {
  rm -rfv historyTmp tmp
}

while getopts ":dbtisch*:" opt; do
    case $opt in
      d)
        docker build -t "${IMAGE_NAME}${NEW_TAG}" docker_builds/
        exit $?
        ;;
      b)
        docker pull "${IMAGE_NAME}"  # pull Dockerhub image
        # build using DH image as cache
        docker build --cache-from="${IMAGE_NAME}" -t "${IMAGE_NAME}:latest" \
            docker_builds/
        docker build --build-arg=USERID="$(id -u)" . -t sysadmin_tester
        exit $?
        ;;
      t)
        docker run -it --rm ${VOL_OPTS} ${NET_OPTS} \
            -u user sysadmin_tester /bin/bash -i -c \
            'set -x && rm -rf /home/user/sysadmin/build &&
             mkdir -p /home/user/sysadmin/build &&
             cd /home/user/sysadmin/build &&
             cmake .. && make check -j $(nproc) && make -j $(nproc)'
        exit $?
        ;;
      i)
        docker run -it --rm ${NET_OPTS} ${VOL_OPTS} -u user sysadmin_tester \
            /bin/bash
        exit $?
        ;;

      s)
        docker run -it --rm ${NET_OPTS} ${VOL_OPTS} -u user sysadmin_tester \
            bash -i -c 'eval ./build/src/sysadmin ./configs/local/config.yaml'
        exit $?
        ;;
      c)
        clean_state
        exit 0
        ;;
      h)
        show_help
        exit 0
        ;;
      *)
        show_help
        exit 1
        ;;
    esac
done

show_help
exit 1
