# $FreeBSD$

.include <src.opts.mk>

.if exists(${.CURDIR}/${MACHINE_ARCH})
SUBDIR+= ${MACHINE_ARCH}
.else
SUBDIR+= ${MACHINE_CPUARCH}
.endif
.if ${MK_GCC_CSU} == "no"
SUBDIR+= common
.endif

.include <bsd.subdir.mk>
