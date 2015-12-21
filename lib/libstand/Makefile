# $FreeBSD$
# Originally from	$NetBSD: Makefile,v 1.21 1997/10/26 22:08:38 lukem Exp $
#
# Notes:
# - We don't use the libc strerror/sys_errlist because the string table is
#   quite large.
#

MK_PROFILE=	no
MK_SSP=		no

.include <src.opts.mk>

LIBSTAND_SRC=	${.CURDIR}
LIBC_SRC=	${LIBSTAND_SRC}/../libc

LIB=		stand
NO_PIC=
INCS=		stand.h
MAN=		libstand.3

WARNS?=		0

CFLAGS+= -I${LIBSTAND_SRC}

.include "${LIBSTAND_SRC}/Makefile.inc"

.if ${MACHINE_CPUARCH} == "arm"
.PATH: ${LIBC_SRC}/arm/gen

# Compiler support functions
.PATH: ${LIBSTAND_SRC}/../../contrib/compiler-rt/lib/builtins/
# __clzsi2 and ctzsi2 for various builtin functions
SRCS+=	clzsi2.c ctzsi2.c
# Divide and modulus functions called by the compiler
SRCS+=	 divmoddi4.c  divmodsi4.c  divdi3.c  divsi3.c  moddi3.c  modsi3.c
SRCS+=	udivmoddi4.c udivmodsi4.c udivdi3.c udivsi3.c umoddi3.c umodsi3.c

.PATH: ${LIBSTAND_SRC}/../../contrib/compiler-rt/lib/builtins/arm/
SRCS+=	aeabi_idivmod.S aeabi_ldivmod.S aeabi_uidivmod.S aeabi_uldivmod.S
SRCS+=	aeabi_memcmp.S aeabi_memcpy.S aeabi_memmove.S aeabi_memset.S
.endif

.if ${MACHINE_CPUARCH} == "aarch64"
.PATH: ${LIBC_SRC}/aarch64/gen
.endif

.if ${MACHINE_CPUARCH} == "powerpc"
.PATH: ${LIBC_SRC}/quad
SRCS+=	ashldi3.c ashrdi3.c
SRCS+=	syncicache.c
.endif

# uuid functions from libc
.PATH: ${LIBC_SRC}/uuid
SRCS+= uuid_create_nil.c uuid_equal.c uuid_from_string.c uuid_is_nil.c uuid_to_string.c

# _setjmp/_longjmp
.PATH: ${LIBSTAND_SRC}/${MACHINE_CPUARCH}
SRCS+=	_setjmp.S

# boot filesystems
SRCS+=	ufs.c nfs.c cd9660.c tftp.c gzipfs.c bzipfs.c
SRCS+=	dosfs.c ext2fs.c
SRCS+=	splitfs.c
SRCS+=	pkgfs.c
.if ${MK_NAND} != "no"
SRCS+=	nandfs.c
.endif

.include <bsd.stand.mk>
.include <bsd.lib.mk>
