#!/bin/sh

export MAKEOBJDIRPREFIX=/tank/emaste/obj
DASHJ="-j8"

TOPDIR=$(cd $(dirname $0) && pwd -L)


make_args="$make_args -DNO_CLEAN -DNO_DEPEND"
make_args="$make_args MODULES_OVERRIDE="

#make_args="$make_args TARGET=aarch64"
make_args="$make_args TARGET=arm64"
make_args="$make_args SRCCONF=$TOPDIR/src.conf"




buildlog=build.log
truncate -s0 $buildlog

set -e
make $make_args $DASHJ buildworld 2>&1 | tee -a $buildlog
make $make_args $DASHJ buildkernel 2>&1 | tee -a $buildlog

# other targets that may be interesting
#make $make_args kernel-toolchain
#make $make_args buildenv
