# $FreeBSD$

.include <bsd.own.mk>

MAN=
PROG_CXX=ld.lld

LLD_SRCS=${SRCTOP}/contrib/llvm/tools/lld

CXXFLAGS+= -I${LLD_SRCS}/include

SRCDIR=	tools/lld/tools/lld
SRCS=	lld.cpp

LLD_LIBS=\
	lldlibConfig \
	lldlibCore \
	lldlibDriver \
	lldELF \
#	lldlibReaderWriter

.for lib in ${LLD_LIBS}
DPADD+=	${.OBJDIR:H:H:H}/lib/clang/lib${lib}/lib${lib}.a
LDADD+=	${.OBJDIR:H:H:H}/lib/clang/lib${lib}/lib${lib}.a
.endfor

LIBDEPS=\
	llvmoption \
	llvmobject \
	llvmmcparser \
	llvmbitreader \
	llvmmc \
	llvmcore \
	llvmsupport \

.include "../clang.prog.mk"
