# $FreeBSD$

.include <bsd.own.mk>

LIB=	lldb

CFLAGS+=-I${.CURDIR:H:H:H}/contrib/llvm/tools/lldb/source/Plugins/Process/POSIX

SRCDIR=	tools/lldb/source
SRCS=	lldb.cpp


TGHDRS=	AttrList \
	Attrs \
	DiagnosticCommonKinds \
	DeclNodes \
	StmtNodes \
	CommentCommandList

.include "../lldb.lib.mk"
