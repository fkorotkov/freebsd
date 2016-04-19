# $FreeBSD$

LLVM_SRCS= ${.CURDIR:H:H:H}/contrib/llvm

.include "clang.build.mk"

INTERNALLIB=

.if ${MACHINE_CPUARCH} == "arm"
STATIC_CXXFLAGS+= -mlong-calls
.endif

.include <bsd.lib.mk>
