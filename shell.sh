#!/bin/bash
export LOCAL_IMAGE=linux-image
# https://sott0n.github.io/posts/docker_gdb/
docker run --rm -it --cap-add=SYS_PTRACE --security-opt="seccomp=unconfined" -v $PWD/:/home/user/jcc $LOCAL_IMAGE
