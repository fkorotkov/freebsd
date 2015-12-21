# $FreeBSD$
# Originally from	$NetBSD: Makefile,v 1.21 1997/10/26 22:08:38 lukem Exp $
#
# Notes:
# - We don't use the libc strerror/sys_errlist because the string table is
#   quite large.
#

MAN=

.include <bsd.own.mk>
MK_SSP=		no

LIBSTAND_SRC=	${.CURDIR}/../../../../lib/libstand
LIBC_SRC=	${LIBSTAND_SRC}/../libc

.PATH:		${LIBSTAND_SRC}
LIB=		stand
INTERNALLIB=
MK_PROFILE=	no
NO_PIC=

WARNS?=		0

.include "${LIBSTAND_SRC}/Makefile.inc"

.if ${MACHINE_CPUARCH} == "arm"
.PATH: ${LIBC_SRC}/arm/gen
SRCS+= divsi3.S
.endif
.if ${MACHINE_CPUARCH} == "powerpc"
.PATH: ${LIBC_SRC}/quad
SRCS+=	ashldi3.c ashrdi3.c
.PATH: ${LIBC_SRC}/powerpc/gen
SRCS+=	syncicache.c
.endif

# uuid functions from libc
.PATH: ${LIBC_SRC}/uuid
SRCS+= uuid_equal.c uuid_is_nil.c

# _setjmp/_longjmp
.if ${MACHINE_CPUARCH} == "amd64"
.PATH: ${LIBSTAND_SRC}/amd64
.elif ${MACHINE_ARCH} == "powerpc64"
.PATH: ${LIBSTAND_SRC}/powerpc
.else
.PATH: ${LIBSTAND_SRC}/${MACHINE_CPUARCH}
.endif
SRCS+=	_setjmp.S

.include <bsd.stand.mk>
.include <bsd.lib.mk>
