# $FreeBSD$

.include <src.opts.mk>
.if ${MK_LLVM_AR_IS_AR} != "no"
MAN=		llvm-ar.1
PROG_CXX=	ar
.else
PROG_CXX=	llvm-ar
.endif

SRCDIR=		tools/llvm-ar
SRCS+=		llvm-ar.cpp

LIBADD+=	z

LINKS+=		${BINDIR}/${PROG_CXX} ${BINDIR}/llvm-ranlib

.include "../llvm.prog.mk"
