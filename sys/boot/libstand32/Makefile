# $FreeBSD$
# Originally from	$NetBSD: Makefile,v 1.21 1997/10/26 22:08:38 lukem Exp $
#
# Notes:
# - We don't use the libc strerror/sys_errlist because the string table is
#   quite large.
#

MAN=

.include <src.opts.mk>
MK_SSP=		no

LIBSTAND_SRC=	${.CURDIR}/../../../lib/libstand
LIBC_SRC=	${LIBSTAND_SRC}/../libc

.PATH:		${LIBSTAND_SRC}
LIB=		stand
INTERNALLIB=
MK_PROFILE=	no
NO_PIC=
.if ${MACHINE_ARCH} == "amd64" || ${MACHINE_ARCH} == "powerpc64"
CFLAGS+=	-m32 -I.
.endif

WARNS?=		0

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
.if ${MACHINE_CPUARCH} == "powerpc"
.PATH: ${LIBC_SRC}/quad
SRCS+=	ashldi3.c ashrdi3.c
SRCS+=	syncicache.c
.endif

# uuid functions from libc
.PATH: ${LIBC_SRC}/uuid
SRCS+= uuid_equal.c uuid_is_nil.c

# _setjmp/_longjmp
.if ${MACHINE_CPUARCH} == "amd64"
.PATH: ${LIBSTAND_SRC}/i386
.else
.PATH: ${LIBSTAND_SRC}/${MACHINE_CPUARCH}
.endif
SRCS+=	_setjmp.S

# boot filesystems
.if ${MK_NAND} != "no"
SRCS+=	nandfs.c
.endif

.include <bsd.stand.mk>
.include <bsd.lib.mk>

.if ${MACHINE_CPUARCH} == "amd64"
CLEANFILES+= machine
beforedepend ${OBJS}: machine
machine:
	ln -fs ${.CURDIR}/../../i386/include machine
.endif
