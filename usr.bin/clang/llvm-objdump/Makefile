# $FreeBSD$

PROG_CXX=	llvm-objdump
LINKS=		${BINDIR}/llvm-objdump	${BINDIR}/objdump
MLINKS=		llvm-objdump.1		objdump.1

SRCDIR=		tools/llvm-objdump
SRCS+=		COFFDump.cpp
SRCS+=		ELFDump.cpp
SRCS+=		MachODump.cpp
SRCS+=		WasmDump.cpp
SRCS+=		llvm-objdump.cpp

LIBADD+=	z

.include "../llvm.prog.mk"
