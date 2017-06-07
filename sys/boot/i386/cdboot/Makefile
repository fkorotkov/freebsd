# $FreeBSD$

PROG=	cdboot
STRIP=
BINMODE=${NOBINMODE}
MAN=
SRCS=	${PROG}.S

CFLAGS+=-I${.CURDIR}/../common

ORG=	0x7c00

LDFLAGS=${LDFLAGS_BIN}

.include <bsd.prog.mk>
