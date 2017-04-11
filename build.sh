#!/bin/bash

rootdir="$(cd $(dirname "$0"); pwd)"
outdir="$rootdir/out"
srcdir="$rootdir"

declare -a cmake_opts
cmake_opts+=("-GNinja")
cmake_opts+=("-DCMAKE_C_COMPILER=clang")
cmake_opts+=("-DCMAKE_CXX_COMPILER=clang++")
cmake_opts+=("-DLIB_GMP=gmp")

# add local GMP information
cmake_opts+=("-DGMP_LIB_DIRS=")
cmake_opts+=("-DGMP_INCLUDE_DIRS=")

mkdir -p $outdir
cd $outdir
cmake "${cmake_opts[@]}" $srcdir
ninja "$@"
