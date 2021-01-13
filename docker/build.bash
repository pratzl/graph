#!/bin/bash

for f in $*
do
  docker build -f Dockerfile.${f}  -t ${f}  .  # --no-cache

#  docker tag   ${f} registry.gitlab.com/al75/bgl17/${f}
#  docker push  registry.gitlab.com/al75/bgl17/${f}
done
