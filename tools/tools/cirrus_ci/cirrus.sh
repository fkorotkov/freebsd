#!/bin/sh

cpus=$(sysctl -n hw.ncpu)

# bootstrap and installed toolchains take a long time to build -- skip them.
cat >/etc/src.conf <<EOF
WITHOUT_BINUTILS_BOOTSTRAP=yes
WITHOUT_CLANG_BOOTSTRAP=yes
WITHOUT_TOOLCHAIN=yes
EOF

make -j $(cpus) buildworld
