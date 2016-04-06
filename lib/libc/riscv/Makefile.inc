# $FreeBSD$
#
# Machine dependent definitions for the RISC-V architecture.
#

# Long double is quad precision
GDTOASRCS+=strtorQ.c
MDSRCS+=machdep_ldisQ.c
SYM_MAPS+=${LIBC_SRCTOP}/riscv/Symbol.map
ARCH_LIB_MIN_COMPAT=11
