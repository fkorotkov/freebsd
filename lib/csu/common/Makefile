# $FreeBSD$

.include <src.opts.mk>
MK_SSP=		no

# From gnu/usr.bin/cc/Makefile.tgt
.if defined(TARGET_ARCH)
TARGET_CPUARCH=${TARGET_ARCH:C/mips(n32|64)?(el)?/mips/:C/arm(v6)?(eb|hf)?/arm/:C/powerpc64/powerpc/}
.else
TARGET_CPUARCH=${MACHINE_CPUARCH}
.endif

SRCS=		crtbegin.c crtend.c
OBJS=		crtbegin.o crtend.o
SOBJS=		crtbeginS.o
CFLAGS.gcc+=	-finhibit-size-directive -fno-toplevel-reorder
CFLAGS+=	-fno-inline-functions -fno-exceptions \
		-fno-zero-initialized-in-bss -fno-asynchronous-unwind-tables \
		-fno-omit-frame-pointer

CRTS_CFLAGS=	-DSHARED ${PICFLAG}

BEGINSRC?=	crtbegin.c
ENDSRC?=	crtend.c

all: ${OBJS} ${SOBJS} ${TGTOBJS}
${OBJS} ${SOBJS}: ${SRCS:M*.h}

CLEANFILES=	${OBJS} ${SOBJS} ${TGTOBJS}

crtbegin.o:	${BEGINSRC}
	${CC} ${CFLAGS} -c -o ${.TARGET} ${.ALLSRC:N*.h:u}

crtbeginS.o:	${BEGINSRC}
	${CC} ${CFLAGS} ${CRTS_CFLAGS} -c -o ${.TARGET} ${.ALLSRC:N*.h:u}

crtend.o:	${ENDSRC}
	${CC} ${CFLAGS} -c -o ${.TARGET} ${.ALLSRC:N*.h:u}

SYMLINKS+=	crtbegin.o ${LIBDIR}/crtbeginT.o
SYMLINKS+=	crtend.o ${LIBDIR}/crtendS.o

realinstall:
.for file in ${OBJS} ${SOBJS} ${TGTOBJS}
	${INSTALL} -o ${LIBOWN} -g ${LIBGRP} -m ${LIBMODE} \
	    ${file} ${DESTDIR}${LIBDIR}/${file}
.endfor

.include <bsd.lib.mk>
