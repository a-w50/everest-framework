#!/bin/sh

cmake \
    -B build \
    -S "$EXT_MOUNT/source" \
    -G Ninja \
    -DEVC_ENABLE_CCACHE=1 \
    -DCOMPILER_WARNING_OPTIONS="-Wall;-Wextra;-Wpedantic;-Wshadow" \
    -DCMAKE_INSTALL_PREFIX="$WORKSPACE_PATH/dist"

export NINJA_STATUS="ninja: [%f/%t] "

ninja -j$(nproc) -C build | tee ninja_build.log
cat ninja_build.log | grep -v "^ninja" > gcc_warnings.txt
