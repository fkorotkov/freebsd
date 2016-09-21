# $FreeBSD$

.if exists(${.CURDIR}/${MACHINE_ARCH})
SUBDIR+= ${MACHINE_ARCH}
.else
SUBDIR+= ${MACHINE_CPUARCH}
.endif
SUBDIR+= common

.include <bsd.subdir.mk>
