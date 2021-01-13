#!/bin/bash

for f in $*
do
  docker build -f Dockerfile.${f}  -t ${f}  .  # --no-cache

done
