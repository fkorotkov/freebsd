# $FreeBSD$

.include "../Makefile.inc0"
.include <src.opts.mk>

.PATH: ${SRCDIR}/gas/doc \
	${SRCDIR}/ld \
	${SRCDIR}/binutils/doc

MAN=	ld.7 ldint.7 binutils.7
.if ${MK_AS} != "no"
MAN+=	as.7
.endif

.include <bsd.prog.mk>
