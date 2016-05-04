# $FreeBSD$

LLD_SRCS= ${SRCTOP}/contrib/llvm/tools/lld

CFLAGS+=-I${LLD_SRCS}/include

.include "clang.lib.mk"
