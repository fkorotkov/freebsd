# $FreeBSD$

PROG_CXX=	objdump
LINKS=		${BINDIR}/llvm-objdump	${BINDIR}/objdump

SRCDIR=		tools/llvm-objdump
SRCS+=		COFFDump.cpp
SRCS+=		ELFDump.cpp
SRCS+=		MachODump.cpp
SRCS+=		WasmDump.cpp
SRCS+=		llvm-objdump.cpp

LIBADD+=	z

.include "../llvm.prog.mk"
