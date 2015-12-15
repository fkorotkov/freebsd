#!/bin/sh
set -e

export MAKEOBJDIRPREFIX=/tank/emaste/obj

args=
#args="$args -DNO_CLEAN -DNO_DEPEND"
args="$args WITHOUT_RESCUE=1"
args="$args WITHOUT_LIB32=1"
jflag=-j4
build()
{
    time nice make $jflag $args buildworld 2>&1
    time nice make $jflag $args buildkernel 2>&1
}

truncate -s0 build.log
build | tee -a build.log
