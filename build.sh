export MAKEOBJDIRPREFIX=/tank/emaste/obj
set -e
#make buildenv TARGET=arm64

CLEAN="-DNO_CLEAN -DNO_DEPEND"
#CLEAN="-DNO_CLEAN"
#SRCCONF="SRCCONF=/dev/null"
MYPATH=$(cd $(dirname $0) && pwd -L)
SRCCONF="SRCCONF=$MYPATH/src.conf"
J=-j8
ARGS="$J $CLEAN $SRCCONF"

#TGT="TARGET_ARCH=aarch64 TARGET=arm64"
TGT="TARGET=arm64"

build()
{
#    make $ARGS kernel-toolchain TARGET=arm64

#    make $ARGS _includes TARGET=arm64
#    make $ARGS buildkernel MODULES_OVERRIDE= TARGET=arm64

#    make -i $ARGS buildworld TARGET=arm64

    #make -i $ARGS buildworld buildkernel TARGET=arm64
    make $ARGS buildworld $TGT
    make $ARGS buildkernel $TGT

    ls -l $MAKEOBJDIRPREFIX/arm64.arm64/tank/emaste/src/freebsd-arm64/sys/GENERIC/kernel*
}

build_kernel()
{
    make $ARGS kernel-toolchain $TGT
    make $ARGS _includes $TGT
    make $ARGS buildkernel MODULES_OVERRIDE= $TGT
}

build 2>&1 | tee build.log
#build_kernel 2>&1 | tee build.log
