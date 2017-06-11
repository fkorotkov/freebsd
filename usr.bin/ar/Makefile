# $FreeBSD$

.include <src.opts.mk>

.if ${MK_LLVM_AR_IS_AR} == "no"
PROG=	ar
.else
PROG=	bsdar
.endif
SRCS=	ar.c acplex.l acpyacc.y read.c util.c write.c y.tab.h

LIBADD=	archive elf

NO_WMISSING_VARIABLE_DECLARATIONS=

CFLAGS+=-I. -I${.CURDIR}

.if ${MK_SHARED_TOOLCHAIN} == "no"
NO_SHARED?=	yes
.endif
LINKS=	${BINDIR}/ar ${BINDIR}/ranlib
MLINKS= ar.1 ranlib.1

.include <bsd.prog.mk>
