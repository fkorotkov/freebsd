export MAKEOBJDIRPREFIX=/tank/emaste/obj
#make buildenv TARGET=arm64

CLEAN="-DNO_CLEAN -DNO_DEPEND"
#CLEAN="-DNO_CLEAN"
#SRCCONF="SRCCONF=/dev/null"
MYPATH=$(cd $(dirname $0) && pwd -L)
SRCCONF="SRCCONF=$MYPATH/src.conf"
J=-j8
ARGS="$J $CLEAN $SRCCONF"

build()
{
#    make $ARGS kernel-toolchain TARGET=arm64

#    make $ARGS _includes TARGET=arm64
#    make $ARGS buildkernel MODULES_OVERRIDE= TARGET=arm64

#    make -i $ARGS buildworld TARGET=arm64

    #make -i $ARGS buildworld buildkernel TARGET=arm64
    make $ARGS buildworld buildkernel TARGET=arm64
#    make $ARGS buildkernel TARGET=arm64

    ls -l $MAKEOBJDIRPREFIX/arm64.arm64/tank/emaste/src/freebsd-arm64/sys/GENERIC/kernel*
}

build_kernel()
{
    make $ARGS kernel-toolchain TARGET=arm64
    make $ARGS _includes TARGET=arm64
    make $ARGS buildkernel MODULES_OVERRIDE= TARGET=arm64
}

build 2>&1 | tee build.log
#build_kernel 2>&1 | tee build.log
