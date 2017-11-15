#!/bin/bash
set +x

TAG=$1
DOCKER_USERNAME=$2
DOCKER_PASSWORD=$3

if [ ! -z "${DOCKER_PASSWORD+x}" ]; then
    echo "Not enough args"
    exit 1
fi

# retag the image we made earlier
docker image tag starryoss/sysadmin-build:latest "starryoss/sysadmin-build:${TAG}"

echo "$DOCKER_PASSWORD" | docker login --password-stdin -u="$DOCKER_USERNAME"
docker push "starryoss/sysadmin-build:${TAG}"
