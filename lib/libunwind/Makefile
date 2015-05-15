# $FreeBSD$

.include <src.opts.mk>

HDRDIR=		${.CURDIR}/../../contrib/llvm-libunwind/include
SRCDIR=		${.CURDIR}/../../contrib/llvm-libunwind/src

.PATH: ${SRCDIR}

LIB=		unwind
SHLIB_MAJOR=	1

SRCS=		Unwind-EHABI.cpp\
		Unwind-sjlj.c\
		UnwindLevel1-gcc-ext.c\
		UnwindLevel1.c\
		UnwindRegistersRestore.S\
		UnwindRegistersSave.S\
		libunwind.cpp

#		Does not build at the moment
#		Unwind_AppleExtras.cpp

WARNS=		3
CFLAGS+=	-I${HDRDIR} -I${.CURDIR}
.if empty(CXXFLAGS:M-std=*)
CXXFLAGS+=	-std=c++11
.endif

.include <bsd.lib.mk>
