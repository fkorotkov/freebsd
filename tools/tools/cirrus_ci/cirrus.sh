#!/bin/sh

set -e

cpus=$(sysctl -n hw.ncpu)

# bootstrap and installed toolchains take a long time to build -- skip them.
cat >/etc/src.conf <<EOF
WITHOUT_BINUTILS_BOOTSTRAP=yes
WITHOUT_CLANG_BOOTSTRAP=yes
WITHOUT_LLD_BOOTSTRAP=yes
WITHOUT_TOOLCHAIN=yes
EOF

echo "Building world with -j $cpus"
make -j $cpus buildworld
