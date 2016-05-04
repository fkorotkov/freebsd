# $FreeBSD$

.include <bsd.own.mk>

LIB=    lldELF

SRCDIR=	tools/lld/ELF
SRCS=   Driver.cpp \
	DriverUtils.cpp \
	Error.cpp \
	InputFiles.cpp \
	InputSection.cpp \
	LinkerScript.cpp \
	MarkLive.cpp \
	OutputSections.cpp \
	SymbolTable.cpp \
	Symbols.cpp \
	Target.cpp \
	Writer.cpp

CFLAGS+= -I${.CURDIR}
TGHDRS=	Options

.include "../lld.lib.mk"
