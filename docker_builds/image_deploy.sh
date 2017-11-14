#!/bin/env bash
set +x

TRAVIS_TAG=$1
DOCKER_USERNAME=$2
DOCKER_PASSWORD=$3

docker login -u="$DOCKER_USERNAME" -p "$DOCKER_PASSWORD"
docker push "starryoss/sysadmin-build:${TRAVIS_TAG}"
