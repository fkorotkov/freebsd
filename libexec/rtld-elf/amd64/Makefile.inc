# $FreeBSD$

CFLAGS+=	${CFLAGS_NO_SIMD} -msoft-float
# PR 26731 lld linker script support needs to handle arithmetic expressions
# Uncomment this to build the dynamic linker as an executable instead
# of a shared library:
#LDSCRIPT=	${.CURDIR}/${MACHINE_CPUARCH}/elf_rtld.x
