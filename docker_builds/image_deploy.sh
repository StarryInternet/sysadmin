#!/bin/bash
set +x
set -e

TAG=$1
DOCKER_USERNAME=$2
DOCKER_PASSWORD=$3

if [ -z "${DOCKER_PASSWORD}" ]; then
    echo "Not enough args"
    exit 1
fi

# retag the image we made earlier
docker image tag starryoss/sysadmin-build:latest "starryoss/sysadmin-build:${TAG}"

docker login --password="$DOCKER_PASSWORD" --username="$DOCKER_USERNAME"

docker push "starryoss/sysadmin-build:${TAG}"
