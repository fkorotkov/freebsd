# $FreeBSD$

SHLIBDIR?=	/lib/casper

.include <src.opts.mk>

PACKAGE=libcasper

SHLIB_MAJOR=	0
INCSDIR?=	${INCLUDEDIR}/casper

.if ${MK_CASPER} != "no"
SHLIB=	cap_fileargs

SRCS=	cap_fileargs.c
.endif

INCS=	cap_fileargs.h

LIBADD=	nv

CFLAGS+=-I${.CURDIR}

.include <bsd.lib.mk>
